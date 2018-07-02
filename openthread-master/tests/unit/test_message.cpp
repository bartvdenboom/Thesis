#include "test_platform.h"
#include <mac/mac_frame.hpp>
#include "net/icmp6.hpp"
#include "common/message.hpp"
#include "net/icmp6.hpp"
#include "thread/thread_netif.hpp"
#include "net/netif.hpp"
#include "net/socket.hpp"
#include "crypto/aes_ccm.hpp"

#include "api/message_api.cpp"
#include <thread/mle.hpp>
#include <thread/mle_tlvs.hpp>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include <inttypes.h>

#include <SUT_platform.cpp>
#include <openthread/instance.h>
#include "timer.cpp"


// Platform variables

bool g_fRadioEnabled = false;
uint8_t g_RecvChannel = 0;
uint8_t g_TransmitPsdu[128];
otRadioFrame g_TransmitRadioFrame;
bool g_fTransmit = false;

bool testFuzzRadioIsEnabled(otInstance *)
{
    //printf("testFuzzRadioIsEnabled\n");

    return g_fRadioEnabled;
}

otError testFuzzRadioEnable(otInstance *)
{
    //printf("testFuzzRadioEnable\n");

        #ifdef DBG_FUZZ
    Log("Radio enabled");
        #endif
    g_fRadioEnabled = true;
    return OT_ERROR_NONE;
}

otError testFuzzRadioDisable(otInstance *)
{
    //printf("testFuzzRadioDisable\n");

        #ifdef DBG_FUZZ
    Log("Radio disabled");
        #endif
    g_fRadioEnabled = false;
    return OT_ERROR_NONE;
}

otError testFuzzRadioReceive(otInstance *, uint8_t aChannel)
{
    #ifdef DBG_FUZZ
    Log("==> receive");
    #endif
    g_RecvChannel = aChannel;

    return OT_ERROR_NONE;
}

otError testFuzzRadioTransmit(otInstance *)
{
    #ifdef DBG_FUZZ
    Log("==> transmit");
    #endif

    g_fTransmit = true;
    return OT_ERROR_NONE;
}

otRadioFrame *testFuzztRadioGetTransmitBuffer(otInstance *)
{
    //printf("testFuzztRadioGetTransmitBuffer\n");

    return &g_TransmitRadioFrame;
}


otInstance *resetInstance(uint8_t *otInstanceBuffer, size_t otInstanceBufferLength)
{
    // Set the radio capabilities to disable any Mac related timer dependencies
    g_testPlatRadioCaps = (otRadioCaps)(OT_RADIO_CAPS_ACK_TIMEOUT | OT_RADIO_CAPS_TRANSMIT_RETRIES );

    // Set the platform function pointers
    g_TransmitRadioFrame.mPsdu = g_TransmitPsdu;
    g_testPlatRadioIsEnabled = testFuzzRadioIsEnabled;
    g_testPlatRadioEnable = testFuzzRadioEnable;
    g_testPlatRadioDisable = testFuzzRadioDisable;
    g_testPlatRadioReceive = testFuzzRadioReceive;
    g_testPlatRadioTransmit = testFuzzRadioTransmit;
    g_testPlatRadioGetTransmitBuffer = testFuzztRadioGetTransmitBuffer;

    otInstance *aInstance;

    VerifyOrQuit(otInstanceBuffer != NULL, "Failed to allocate otInstance");
    memset(otInstanceBuffer, 0, otInstanceBufferLength);

    // Initialize OpenThread with the buffer
    aInstance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength);
    // External address
    otExtAddress aExtAddr;
    aExtAddr.m8[0]=0x72;
    aExtAddr.m8[1]=0x51;
    aExtAddr.m8[2]=0xff;
    aExtAddr.m8[3]=0x4a;
    aExtAddr.m8[4]=0xec;
    aExtAddr.m8[5]=0x29;
    aExtAddr.m8[6]=0xcd;
    aExtAddr.m8[7]=0xba;

    VerifyOrQuit(aInstance != NULL, "Failed to initialize otInstance");

    // Start the Thread network
    otLinkSetPanId(aInstance, (otPanId)0xFACE);
    // Make sure the external address of the Thread instance stays the same after a reset.
    const otExtAddress linklocal = static_cast<const otExtAddress>(aExtAddr);
    otLinkSetExtendedAddress(aInstance, &linklocal);
    otIp6SetEnabled(aInstance, true);
    otThreadSetEnabled(aInstance, true);
    return aInstance;
}


void run_SUT()
{
    size_t otInstanceBufferLength = 20480;
    uint8_t *otInstanceBuffer = NULL;
    otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
    otInstance *aInstance;
    aInstance = resetInstance(otInstanceBuffer,otInstanceBufferLength); 
    char *receivebuffer;    
    setPrefixes();

    int TeacherTransmitCounter = 0;         // Amount of frames as specified by the statelearner.
    int SUTTransmitCounter = -1;            // Amount of response frames to frames given by the statelearner.
    int ResetCounter = 0;                   // AMount of RESET operations as given by the statelearner.
    bool reset = false;                     // True if reset operation in progress.
    timer t;                                // Timer to ensure the SUT is in a Leader state before processing any frames.
    timer pollTimer;                        // Timer to poll for new incoming frames every milisecond.
    unsigned long resetTimeout = 50000;     //
    unsigned long pollTimeout = 1000;       //
    pollTimer.start();

    while (true)
    {
        if(otTaskletsArePending(aInstance)){
            otTaskletsProcess(aInstance);
        }
        
        if (g_testPlatAlarmSet && otPlatAlarmMilliGetNow() >= g_testPlatAlarmNext)
        {
            g_testPlatAlarmSet = false;
            otPlatAlarmMilliFired(aInstance);
        }
        if(reset && t.isTimeout(resetTimeout)){
            reset = false; 
            t.stop();
            t.reset();
        }
        if (g_fRadioEnabled)
        {
            if (g_fTransmit)
            {
                if(reset)
                {
                    if(!t.isTimeout(resetTimeout)){
                        printf("Discarding frame \n");
                        otPlatRadioTxDone(aInstance, &g_TransmitRadioFrame, NULL, OT_ERROR_NONE); 
                        g_fTransmit = false;
                    }
                }                
                else{
                    if(SUTTransmitCounter == -1){
                        printf("Discarding frame \n");
                        otPlatRadioTxDone(aInstance, &g_TransmitRadioFrame, NULL, OT_ERROR_NONE);
                        g_fTransmit = false;
                    }
                    else{
                        printf("Transmit of frame %i begun\n", SUTTransmitCounter);
                        otPlatRadioTxDone(aInstance, &g_TransmitRadioFrame, NULL, OT_ERROR_NONE);
                        setTransmitFrame(testFuzztRadioGetTransmitBuffer(aInstance),SUTTransmitCounter);
                        g_fTransmit = false;
                        SUTTransmitCounter++;
                    }
                }
            }
            if(pollTimer.isTimeout(pollTimeout)){
                
                if(reset_exists(ResetCounter) && !reset) 
                {
                    otInstanceFactoryReset(aInstance);
                    aInstance = NULL;
                    aInstance = resetInstance(otInstanceBuffer,otInstanceBufferLength);
                    printf("Instance RESET\n");
                    SUTTransmitCounter = -1;
                    reset = true;
                    t.start();
                    removeReset(ResetCounter);
                    ResetCounter++;
                }
                if(transmit_exists(TeacherTransmitCounter)&& !reset && g_RecvChannel != 0)
                {
                    if(SUTTransmitCounter == -1)
                    {
                        SUTTransmitCounter = 0;
                    }
                    printf("Frame %i received\n",TeacherTransmitCounter);
                    otRadioFrame frame;
                    memset(&frame, 0, sizeof(frame));
                    receivebuffer = getTransmitBuffer(TeacherTransmitCounter);
                    frame.mPsdu = (uint8_t*)receivebuffer;
                    frame.mLength = getLength();
                    frame.mChannel = g_RecvChannel;
                    g_RecvChannel = 0;
                    otPlatRadioReceiveDone(aInstance, &frame, OT_ERROR_NONE);
                    // Hack to get a receive poll immediately
                    aInstance->mThreadNetif.GetMac().SetChannel(11);
                    TeacherTransmitCounter++;
                }
                pollTimer.reset();
            } 
        }
    }
    testFreeInstance(aInstance);
}

int main(void)
{
    run_SUT();  
    return 0;
}