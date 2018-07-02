#include <sys/stat.h>

#include <string>
#include <limits.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int bufferSize = 0;

char *SUTPrefix = (char*)malloc(sizeof(char)*200);
char *LearnerPrefix = (char*)malloc(sizeof(char)*200);
char *RESETPrefix = (char*)malloc(sizeof(char*)*200);


char Sutprefix[30] = {"/SUT/transmit/transmit_frame\0"};
char Learnerprefix[36] = {"/Learner/transmit/transmit_frame\0"};
char Resetprefix[36] = {"/Learner/transmit/RESET_FRAME\0"};

char *messagebufferptr = NULL;

std::ifstream ifp;
std::ofstream ofp;

void setPrefixes(){
	std::string Dir = get_current_dir_name();
	strcpy(SUTPrefix,Dir.c_str());
	strcat(SUTPrefix, Sutprefix);
	strcpy(LearnerPrefix,Dir.c_str());
	strcat(LearnerPrefix,Learnerprefix);
	strcpy(RESETPrefix, Dir.c_str());
	strcat(RESETPrefix, Resetprefix);
}


void setLength(int i){
	bufferSize = i;
}
uint8_t getLength(){
	uint8_t val;
	if(bufferSize>0){
		val = static_cast<uint8_t>((size_t)bufferSize);
	}
	else{
		val = -1;
	}
	bufferSize = 0;
	return val;
}

void freeMessageBuffer(){
	if(messagebufferptr!=NULL){
		
		delete [] messagebufferptr;
		messagebufferptr = NULL;
	}

}


bool fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}


bool transmit_exists(int i) {    
    char buf[15];
    sprintf(buf,"%d.bin",i);
	char file[200];
	strcpy(file,LearnerPrefix);
	strcat(file,buf);
   	return fileExists(file);
}


char *getTransmitBuffer(int i){
	freeMessageBuffer();
	char buf[15];
    sprintf(buf,"%d.bin",i);
	char file[200];
	strcpy(file,LearnerPrefix);
	strcat(file,buf);
	ifp.open(file, std::ios::binary);
	// Getting length
	ifp.seekg (0, ifp.end);
    int length = ifp.tellg();
    ifp.seekg (0, ifp.beg);
    //

    while(length==0){
    	ifp.close();
	    ifp.open(file, std::ios::binary);
		// Getting length
		ifp.seekg (0, ifp.end);
	    length = ifp.tellg();
	    ifp.seekg (0, ifp.beg);
    }
    setLength(length);
    char *buffer = new char[length];
    messagebufferptr = buffer;
    ifp.read(buffer,length);
	std::remove(file);
	ifp.close();
	return buffer;
}


void removeReset(int resetCounter){
	char buf[15];
    sprintf(buf,"%d.bin",resetCounter);
	char file[200];
	strcpy(file,RESETPrefix);
	strcat(file,buf);
	std::remove(file);
}


bool reset_exists(int i){
    char buf[15];
    sprintf(buf,"%d.bin",i);
	char file[200];
	strcpy(file,RESETPrefix);
	strcat(file,buf);
	return fileExists(file);
}

void setTransmitFrame(otRadioFrame *frame, int transmitcounter){
    char buf[15];
    sprintf(buf,"%d.bin",transmitcounter);
	char file[200];
	strcpy(file,SUTPrefix);
	strcat(file,buf);
	ofp.open(file, std::ios::out | std::ios::binary);
	ofp.write((char*)frame->mPsdu, frame->mLength);
	ofp.close();
}

