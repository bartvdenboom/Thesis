#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include "openthread-core-config.h"

#include <openthread/instance.h>
#include <openthread/platform/misc.h>
#include <openthread/platform/settings.h>

#include "openthread-instance.h"
#include "openthread-single-instance.h"
#include "common/logging.hpp"
#include "common/new.hpp"
#include <stdlib.h>


otInstance *otGetInstance(void);

namespace ot
{
ThreadNetif &otGetThreadNetif(void);


MeshForwarder &otGetMeshForwarder(void);

TaskletScheduler &otGetTaskletScheduler(void);


Ip6::Ip6 &otGetIp6(void);


void otInstancePostConstructor(otInstance *aInstance);

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES

otInstance *otInstanceInit(void *aInstanceBuffer, size_t *aInstanceBufferSize);

bool otInstanceIsInitialized(otInstance *aInstance);

#else // #if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES

otInstance *otInstanceInitSingle(void);


bool otInstanceIsInitialized(otInstance *aInstance);


#endif // #if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES

void otInstanceFinalize(otInstance *aInstance);

otError otSetStateChangedCallback(otInstance *aInstance, otStateChangedCallback aCallback, void *aCallbackContext);

void otRemoveStateChangeCallback(otInstance *aInstance, otStateChangedCallback aCallback, void *aCallbackContext);

void otInstanceReset(otInstance *aInstance);

void otInstanceFactoryReset(otInstance *aInstance);

otError otInstanceErasePersistentInfo(otInstance *aInstance);

otLogLevel otGetDynamicLogLevel(otInstance *aInstance);

otError otSetDynamicLogLevel(otInstance *aInstance, otLogLevel aLogLevel);


}
