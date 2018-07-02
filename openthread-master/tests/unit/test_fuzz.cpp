#include "test_platform.h"
#include <mac/mac_frame.hpp>
#include "net/icmp6.hpp"
#include "common/message.hpp"
#include "net/icmp6.hpp"
#include "thread/thread_netif.hpp"
#include "net/netif.hpp"
#include "net/socket.hpp"
#include "crypto/aes_ccm.hpp"


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

#include <inttypes.h>

#include <mapper_platform.cpp>
#include <openthread/instance.h>
#include "timer.cpp"
#include "messages.cpp" 


// Platform variables
int ResetCounter = 0;               // Number of reset operations.
bool g_fRadioEnabled = false;
uint8_t g_RecvChannel = 0;
uint8_t g_TransmitPsdu[128];
otRadioFrame g_TransmitRadioFrame;
bool g_fTransmit = false;


bool testFuzzRadioIsEnabled(otInstance *)
{

    return g_fRadioEnabled;
}

otError testFuzzRadioEnable(otInstance *)
{

            #ifdef DBG_FUZZ
    Log("Radio enabled");
            #endif
    g_fRadioEnabled = true;
    return OT_ERROR_NONE;
}

otError testFuzzRadioDisable(otInstance *)
{

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
    aExtAddr.m8[0]=0x4a;
    aExtAddr.m8[1]=0xec;
    aExtAddr.m8[2]=0x29;
    aExtAddr.m8[3]=0xcd;
    aExtAddr.m8[4]=0xba;
    aExtAddr.m8[5]=0xab;
    aExtAddr.m8[6]=0xf2;
    aExtAddr.m8[7]=0xfb;

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


void learn()
{
    // Initialise Instance
    size_t otInstanceBufferLength = 20480;
    uint8_t *otInstanceBuffer = NULL;
    otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
    otInstance *aInstance;
    aInstance = resetInstance(otInstanceBuffer,otInstanceBufferLength); 
    char *receivebuffer;    
    setPrefixes();  

    int statelearnercommand = -1;       // Number of commands received from statelearner.
    int SUTTransmitCounter = 0;         // Number of incoming packets from the SUT-instance.
    int responsenr = 0;                 // Number of incoming packets from the SUT-instance processed by learner.
    bool sutstarted = false;            // Indicates the SUT-instance has started.
    bool reset = false;                 // Indicates a reset operation on both the Learner-instance and SUT-instance is in process.
    bool framereceived = false;         // Indicates a frame has been received but has to be processed.
    bool statelearnertask = false;      // Indicates whether a command by the statelearner has been received.
    bool discoveryInProgress = false;   // Indicates whether a discovery process is in progress. Needed because of callback functions present in the OT code.
    bool addrsolInProgress = false;     // Indicates whether a address solicit progress is in progress. Needed because of callback functions present in the OT code.
    bool pending = false;
    bool error = false;

    // Timers
    timer t;
    timer resetTimer;
    resetTimer.start();
    unsigned long resetTimeout = 70000;
    unsigned long timerTimeout = 30000;
    
    // Set up connection to statelearner
    int client, server;
    int portNum = 8888;
    int bufsize = 1024; 
    struct sockaddr_in server_addr;
    socklen_t size;
    client = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        exit (EXIT_FAILURE);
    }

    listen(client, 1);
    server = accept(client,(struct sockaddr *)&server_addr,&size);
    
    std::string data;               // Command buffer from statelearner.
    char recvData[bufsize+1]={0};   // 
    std::string response;           // Answer buffer to statelearner.
    setStateLearnerCommand(true);   //  Signals to the OpenThread instance that it is in statelearner-mode, so it only sends packets as instructed by the statelearner.

    while (true)
    {
        
        otTaskletsProcess(aInstance);

        if(!sutstarted){
            printf("starting SUT...\n");
            int result = system("gnome-terminal -x sh -c ./test-message");
            (void)result;
            sutstarted = true;
        }

        if (g_testPlatAlarmSet && otPlatAlarmMilliGetNow() >= g_testPlatAlarmNext)
        {
            g_testPlatAlarmSet = false;
            otPlatAlarmMilliFired(aInstance);
        }
    
        if(reset && resetTimer.isTimeout(resetTimeout)){
            reset = false; 
        }

        if(!aInstance->mThreadNetif.GetMle().IsAddressSolicitPending()){
            addrsolInProgress = false;
        }

        if(!aInstance->mThreadNetif.GetMle().IsDiscoverInProgress()){
            discoveryInProgress = false;
        }

        if(framereceived && !t.isTimeout(timerTimeout)){
            error = true;
            
            if(strlen(getMessageType())==0){
                framereceived = true;
            } 
            else{
                framereceived = false;
                response += getMessageType();
                t.reset();
                error = false;
                emptyMessageType();
                responsenr++;
                if(!pending){
                    pending = true; 
                }     
            }
        } 
        if(t.isTimeout(timerTimeout)){
            if(error){ //If a Frame is not handled in time by the mapper for some reason the Response can be given manually.
                printf("give response:\n");
                std::getline( std::cin, response );
                framereceived = false;
            }
            if(response == ""){
                response = "TIMEOUT\n";
            }
            else{
                response += (std::string)"\n"; 
            }
            printf("%s",response.c_str());
            send(server,response.data(), response.size(),0);
            t.stop();
            t.reset();
            if(aInstance->mThreadNetif.GetMle().IsAddressSolicitPending()){
                aInstance->mThreadNetif.GetMle().StateLearnerTimeout();
            }
            statelearnertask = false;
            response = "";
            pending = false;
        }
        
        if(sutstarted && !statelearnertask && !reset && !discoveryInProgress && !addrsolInProgress && !framereceived){
            int lenRead = recv(server, recvData, bufsize, 0);
            if (lenRead == -1)
            {
                recvData[lenRead] = '\n';
            }
            else if (lenRead == 0)
            {
                data = "";
            }
            else
            {
                statelearnertask = true;
                recvData[bufsize] = '\n'; 
                data="";
                data.assign(strtok(recvData,"\n"),0,strlen(recvData+1));
                printf("-----------------------------------------------------\n");
                printf("Command %s  # %i received from statelearner.\n", data.c_str(), statelearnercommand);
                if(data == "RESET"){ 
                    //Reset this instance to clear framecounters
                    otInstanceFactoryReset(aInstance);
                    aInstance = NULL;
                    aInstance = resetInstance(otInstanceBuffer,otInstanceBufferLength);
                    reset = true;
                    resetTimer.reset();
                    printf("Transmit of Resetframe %i begun\n", ResetCounter);
                    setRESETFrame(ResetCounter);
                    response = "DONE\n";
                    setStateLearnerCommand(true);
                    aInstance->mThreadNetif.GetMle().emptyCaches();
                    send(server,response.data(), response.size(),0);
                    response = "";
                    ResetCounter++;
                    statelearnertask = false;
                    SUTTransmitCounter = 0;
                }  
                else if(data == "DISC_REQ"){ 
                    if(aInstance->mThreadNetif.GetMle().IsDiscoverInProgress()){
                        discoveryInProgress = true;
                        statelearnertask = false;
                    }
                    else{
                        DISC_REQ(aInstance); 
                        discoveryInProgress = true;
                    }
                }
                else if(data=="ADDR_SOL"){
                    if(aInstance->mThreadNetif.GetMle().IsAddressSolicitPending()){
                        addrsolInProgress = true;
                        statelearnertask = false;
                    }
                    else{
                        ADDR_SOL(aInstance);
                    }
                }
                else{   
                    PerformTask(aInstance, data);       
                } 
            }
        }
        
        if(discoveryInProgress){
            if(!aInstance->mThreadNetif.GetMle().IsDiscoverInProgress()){
                discoveryInProgress = false;
                statelearnertask = true;
                DISC_REQ(aInstance);
            }
        }
        if(addrsolInProgress){
            if(!aInstance->mThreadNetif.GetMle().IsAddressSolicitPending()){
                addrsolInProgress = false;
                statelearnertask = true;
                ADDR_SOL(aInstance);
            }
        }    
        if (g_fRadioEnabled && !framereceived && !reset) 
        { 
            if (g_fTransmit){ 
                statelearnercommand++;
                printf("Transmit of frame %i begun\n", statelearnercommand);
                setTransmitFrame(testFuzztRadioGetTransmitBuffer(aInstance), statelearnercommand);
                otPlatRadioTxDone(aInstance, &g_TransmitRadioFrame, NULL, OT_ERROR_NONE); 
                t.start();
                g_fTransmit = false;
            }
            if(!t.isTimeout(timerTimeout) && t.isRunning()){                
                if(transmit_exists(SUTTransmitCounter))
                {
                    if((statelearnertask || pending)){//} && g_RecvChannel != 0){
                        printf("Receive\n");
                        if(pending){
                            response += (std::string) " - ";
                        }
                        otRadioFrame frame;
                        memset(&frame, 0, sizeof(frame));
                        freeMessageBuffer();
                        receivebuffer = getTransmitBuffer(SUTTransmitCounter);
                        frame.mPsdu = (uint8_t*)receivebuffer;
                        frame.mLength = getLength();
                        frame.mChannel = g_RecvChannel;
                        SUTTransmitCounter++;
                        g_RecvChannel = 0;
                        otPlatRadioReceiveDone(aInstance, &frame, OT_ERROR_NONE);
                        framereceived = true;
                        // Hack to get a receive poll immediately
                        aInstance->mThreadNetif.GetMac().SetChannel(11);
                    } 
                }
            }
        }
    }
    testFreeInstance(aInstance);
}


int main(void)
{
    learn();
    return 0;
}