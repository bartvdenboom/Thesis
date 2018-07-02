//MLE MESSAGE SET

#include "api/message_api.cpp"
#include <thread/mle.hpp>
#include <thread/mle_tlvs.hpp>


//Probleem met veel paketten die worden verzonden, wacht totdat alle paketten binnen zijn is de oplossing.
otError ANNO_U(otInstance *aInstance){
	ot::Mle::setInstruction("ANNO");
    aInstance->mThreadNetif.SubscribeAllRoutersMulticast();
	bool aOrphanAnnounce = false;
	otError error = aInstance->mThreadNetif.GetMle().SendDummyAnnounce(aOrphanAnnounce);
    if(error != OT_ERROR_NONE){
	   printf("Sending Announce, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError ANNO_O(otInstance *aInstance){
	ot::Mle::setInstruction("ANNO");
    aInstance->mThreadNetif.SubscribeAllRoutersMulticast();
    bool aOrphanAnnounce = true;
    otError error = aInstance->mThreadNetif.GetMle().SendDummyAnnounce(aOrphanAnnounce);
    if(error != OT_ERROR_NONE){
        printf("Sending Announce, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}


otError CHIL_REQ(otInstance *aInstance){
	ot::Mle::setInstruction("CHIL_REQ");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildIdRequest();
    if(error != OT_ERROR_NONE){
	   printf("Sending dummy Child ID request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}


otError CHIL_RES(otInstance *aInstance){
	ot::Mle::setInstruction("CHIL_RES");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildIdResponse();
    if(error != OT_ERROR_NONE){
	   printf("Sending Child ID response, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}


otError PARE_REQ_R(otInstance *aInstance){
	ot::Mle::setInstruction("PARE_REQ_R");
	otError error = aInstance->mThreadNetif.GetMle().SendParentRequestR();
    if(error != OT_ERROR_NONE){
        printf("Sending parent request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError PARE_REQ_C(otInstance *aInstance){
	ot::Mle::setInstruction("PARE_REQ_C");
	otError error = aInstance->mThreadNetif.GetMle().SendParentRequestC();
	if(error != OT_ERROR_NONE){
        printf("Sending parent request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;

}

otError DISC_REQ(otInstance *aInstance){
	ot::Mle::setInstruction("DISC_REQ");
	otError error = aInstance->mThreadNetif.GetMle().DummyDiscover();
	if(error != OT_ERROR_NONE){
        printf("Sending discovery request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError PARE_RES(otInstance *aInstance){
	ot::Mle::setInstruction("PARE_RES");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyParentResponse();
 	if(error != OT_ERROR_NONE){
        printf("Sending Parent response, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_REQ_I(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_REQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkRequestI();
 	if(error != OT_ERROR_NONE){
        printf("Sending Link request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_REQ_N(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_REQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkRequestN();
 	if(error != OT_ERROR_NONE){
        printf("Sending Link request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_REQ_R(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_REQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkRequestR();
 	if(error != OT_ERROR_NONE){
        printf("Sending Link request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_ACC_I(otInstance *aInstance){
	ot::Mle::setInstruction("LINK_ACC");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkAcceptI();
	if(error != OT_ERROR_NONE){
        printf("Sending Link accept, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_ACC_R(otInstance *aInstance){
	ot::Mle::setInstruction("LINK_ACC");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkAcceptR();
	if(error != OT_ERROR_NONE){
        printf("Sending Link accept, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_ACR_I(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_ACR");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkAcceptAndRequestI();
	if(error != OT_ERROR_NONE){
        printf("Sending Link accept request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_ACR_R(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_ACR");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkAcceptAndRequestR();
	if(error != OT_ERROR_NONE){
        printf("Sending Link accept request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError LINK_REJ(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_REJ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyLinkReject();
	if(error != OT_ERROR_NONE){
        printf("Sending Link Reject, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }
//Ignore
 otError LINK_ADV(otInstance *aInstance){
 	ot::Mle::setInstruction("LINK_ADV");
    return OT_ERROR_NONE;
 	(void)aInstance;
 }

otError DATA_REQ(otInstance *aInstance){
 	ot::Mle::setInstruction("DATA_REQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyDataRequest();
	if(error != OT_ERROR_NONE){
        printf("Sending Data Request, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }

otError DATA_RES(otInstance *aInstance){
 	ot::Mle::setInstruction("DATA_RES");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyDataResponse();
	if(error != OT_ERROR_NONE){
        printf("Sending Data Response, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }

otError CHIL_URQ_C(otInstance *aInstance){
 	ot::Mle::setInstruction("CHIL_URQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateRequestFromChild();
	if(error != OT_ERROR_NONE){
        printf("Sending Child update request from Child, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError CHIL_URQ_P(otInstance *aInstance){
 	ot::Mle::setInstruction("CHIL_URQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateRequestFromParent();
	if(error != OT_ERROR_NONE){
        printf("Sending Child update request from Parent, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError CHIL_URQ_R(otInstance *aInstance){
 	ot::Mle::setInstruction("CHIL_URQ");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateRequestRejectFromParent();
	if(error != OT_ERROR_NONE){
        printf("Sending Child update request Rejection from Parent, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}


otError CHIL_URS_R(otInstance *aInstance){
    ot::Mle::setInstruction("CHIL_URS");
    otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateResponseRejectFromParent();
    if(error != OT_ERROR_NONE){
        printf("Sending Child update response Rejection from Parent, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;

}
otError CHIL_URS_C(otInstance *aInstance){
 	ot::Mle::setInstruction("CHIL_URS");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateResponseFromParent();
	if(error != OT_ERROR_NONE){
        printf("Sending Child update response to Child, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }

otError CHIL_URS_P(otInstance *aInstance){
 	ot::Mle::setInstruction("CHIL_URS");
 	otError error = aInstance->mThreadNetif.GetMle().SendDummyChildUpdateResponseFromChild();
	if(error != OT_ERROR_NONE){
        printf("Sending Child update response to Parent, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }

otError DISC_RES(otInstance *aInstance){
 	ot::Mle::setInstruction("DISC_RES");
	otError error = aInstance->mThreadNetif.GetMle().SendDummyDiscoveryResponse();
	if(error != OT_ERROR_NONE){
        printf("Sending Discovery Response, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
 }


otError ADDR_SOL(otInstance *aInstance){
	otError error;    
	if(aInstance->mThreadNetif.GetMle().mRole != OT_DEVICE_ROLE_CHILD){
		error = aInstance->mThreadNetif.GetMle().SetStateChild(0xe001);
	}	
	error = aInstance->mThreadNetif.GetMle().DummyBecomeRouter();
	if(error != OT_ERROR_NONE){
        printf("Sending Address Solicit, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}


otError ADDR_SOL_R(otInstance *aInstance){
	otError error = OT_ERROR_NONE;
	error = aInstance->mThreadNetif.GetMle().SetStateLeader(aInstance->mThreadNetif.GetMle().GetRloc16());
	aInstance->mThreadNetif.GetMle().SendDummyAddressSolicitResponse();
    if(error != OT_ERROR_NONE){
	   printf("Sending Address Solicit Response, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError ADDR_R_N(otInstance *aInstance){
	otError error;
	if(aInstance->mThreadNetif.GetMle().mRole != OT_DEVICE_ROLE_CHILD){
		error = aInstance->mThreadNetif.GetMle().SetStateChild(0xe001);
	}	
	error = aInstance->mThreadNetif.GetMle().SendDummyAddressRelease();
	if(error != OT_ERROR_NONE){
        printf("Sending Address Release, errorcode: %s\n", otThreadErrorToString(error));
    }
    return error;
}

otError ADDR_R_R(otInstance *aInstance){
    otError error = aInstance->mThreadNetif.GetMle().SendDummyAddressReleaseResponse();
    if(error != OT_ERROR_NONE){
        printf("Sending Address Release Response, errorcode: %s\n", otThreadErrorToString(error));  
    }
    return error;
}

void PerformTask(otInstance *aInstance, std::string task){
    otError error = OT_ERROR_NONE;
    if(task=="CHIL_REQ"){
        error = CHIL_REQ(aInstance);        
    }
    else if(task=="PARE_REQ_R"){
        error = PARE_REQ_R(aInstance);
    }
    else if(task=="PARE_REQ_C"){
        error = PARE_REQ_C(aInstance);
    }
    else if(task=="DISC_REQ"){
        error = DISC_REQ(aInstance);
    }
    else if(task=="ANNO_U"){
        error = ANNO_U(aInstance);
    }
    else if(task=="ANNO_O"){
        error = ANNO_O(aInstance);
    }
    else if(task=="LINK_REQ_I"){
        error = LINK_REQ_I(aInstance);
    }
    else if(task=="LINK_REQ_N"){
        error = LINK_REQ_N(aInstance);
    }
    else if(task=="LINK_REQ_R"){
        error = LINK_REQ_R(aInstance);
    }
    else if(task=="LINK_ACC_I"){
        error = LINK_ACC_I(aInstance);
    }
    else if(task=="LINK_ACC_R"){
        error = LINK_ACC_R(aInstance);
    }
    else if(task=="LINK_ACR_I"){
        error = LINK_ACR_I(aInstance);
    }
    else if(task=="LINK_ACR_R"){
        error = LINK_ACR_R(aInstance);
    }
    else if(task=="LINK_REJ"){
        error = LINK_REJ(aInstance);
    }
    else if(task=="LINK_ADV"){
        error = LINK_ADV(aInstance);
    }
    else if(task=="DATA_REQ"){
        error = DATA_REQ(aInstance);
    }
    else if(task=="DATA_RES"){
        error = DATA_RES(aInstance);
    }
    else if(task=="PARE_RES"){
        error = PARE_RES(aInstance); 
    }
    else if(task=="CHIL_RES"){
        error = CHIL_RES(aInstance);
    }
    else if(task=="CHIL_URQ_C"){
        error = CHIL_URQ_C(aInstance);
    }
    else if(task=="CHIL_URQ_P"){
        error = CHIL_URQ_P(aInstance);
    }
    else if(task=="CHIL_URQ_R"){
        error = CHIL_URQ_R(aInstance);
    }
    else if(task=="CHIL_URS_R"){
        error = CHIL_URS_R(aInstance);
    }
    else if(task=="CHIL_URS_C"){
        error = CHIL_URS_C(aInstance);
    }
    else if(task=="CHIL_URS_P"){
        error = CHIL_URS_P(aInstance);
    }
    else if(task=="DISC_RES"){
        error = DISC_RES(aInstance);
    }
    else if(task=="ADDR_SOL"){
        error = ADDR_SOL(aInstance);
    }
    else if(task=="ADDR_SOL_R"){
        error = ADDR_SOL_R(aInstance);
    }
    else if(task=="ADDR_R_N"){
        error = ADDR_R_N(aInstance);
    }
    else if(task=="ADDR_R_R"){
        error = ADDR_R_R(aInstance);
    }
    else
    {
        printf("ERROR\n");
    }
    if(error != OT_ERROR_NONE){
        //try again
        PerformTask(aInstance, task);
    }
}