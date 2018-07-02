#include <parser.hpp>
static char *messagetypestr = new char[10];

void setMessagetype(const char *message){
	strcpy(messagetypestr,message);
}

char *getMessageTypeString(){
	return messagetypestr;
}

//void mleMessageReceived(){
//	printf("MLE BINNENGEKREGEN\n");
//}