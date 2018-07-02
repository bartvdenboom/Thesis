#include <test_platform.h>

#include <sys/stat.h>

#include <string>
#include <limits.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*

bool fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}

bool transmit_exists(int i) {
    std::string path;
    std::string prefix = "/SUT/transmit/transmit_frame";
    path = get_current_dir_name()+prefix;
    char buf[100];
    sprintf(buf,"%d.bin",i);
    char file[300];
	strcpy(file, path.c_str());
	strcat(file, buf);
    return fileExists(path.c_str());
}


otRadioFrame getTransmitFrame(int i){
	std::ifstream fp;
	std::string path;
    std::string prefix = "/SUT/transmit/transmit_frame";
    path = get_current_dir_name()+prefix;
    char buf[100];
    sprintf(buf,"%d.bin",i);
    char filename[300];
	strcpy(filename, path.c_str());
	strcat(filename, buf);
	fp.open(filename, std::ios::binary);
	// Getting length
	fp.seekg (0, fp.end);
    int length = fp.tellg();
    fp.seekg (0, fp.beg);
    //
    while(length==0){
    	fp.close();
	    fp.open(filename, std::ios::binary);
		// Getting length
		fp.seekg (0, fp.end);
	    length = fp.tellg();
	    fp.seekg (0, fp.beg);
    }
    char buffer[length];
    fp.read(buffer,length);
    otRadioFrame transmitframe;
    memset(&transmitframe, 0, sizeof(transmitframe));
    transmitframe.mPsdu = (uint8_t*)buffer;
    transmitframe.mLength = static_cast<uint8_t>((size_t)length);
	fp.close();
	std::remove(filename);
	return transmitframe;
}

void setRESETFrame(int ResetCounter){
	std::ofstream fp;
	std::string path;
    std::string prefix = "/Learner/transmit/RESET_FRAME";
    path = get_current_dir_name()+prefix;
    char buf[100];
    sprintf(buf,"%d.bin",ResetCounter);
    char filename[300];
	strcpy(filename, path.c_str());
	strcat(filename, buf);
	fp.open(filename, std::ofstream::out | std::ofstream::app);
	fp << ".." << std::endl;
	fp.close();
}


void setTransmitFrame(otRadioFrame *frame, int transmitcounter){
	std::ofstream fp;
	std::string path;
    std::string prefix = "/Learner/transmit/transmit_frame";
    path = get_current_dir_name()+prefix;
    char buf[100];
    sprintf(buf,"%d.bin",transmitcounter);
    char filename[300];
	strcpy(filename, path.c_str());
	strcat(filename, buf);
	printf("%s\n",filename );
	fp.open(filename, std::ios::out | std::ios::binary);
	fp.write((char*)frame->mPsdu, frame->mLength);
	fp.close();
}
*/

int bufferSize = 0;

char *SUTPrefix = (char*)malloc(sizeof(char)*200);
char *LearnerPrefix = (char*)malloc(sizeof(char)*200);
char *RESETPrefix = (char*)malloc(sizeof(char*)*200);


char Sutprefix[30] = {"/SUT/transmit/transmit_frame\0"};
char Learnerprefix[36] = {"/Learner/transmit/transmit_frame\0"};
char Resetprefix[36] = {"/Learner/transmit/RESET_FRAME\0"};

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


bool fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}


bool transmit_exists(int i) {    
    char buf[15];
    sprintf(buf,"%d.bin",i);
	char file[200];
	strcpy(file,SUTPrefix);
	strcat(file,buf);
   	return fileExists(file);
}

char *messagebufferptr = NULL;
void freeMessageBuffer(){
	if(messagebufferptr!=NULL){
		
		delete [] messagebufferptr;
		messagebufferptr = NULL;
	}

}


char *getTransmitBuffer(int i){
	char buf[15];
    sprintf(buf,"%d.bin",i);
	char file[200];
	strcpy(file,SUTPrefix);
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

void setRESETFrame(int ResetCounter){
	char buf[15];
    sprintf(buf,"%d.bin",ResetCounter);
	char file[200];
	strcpy(file,RESETPrefix);
	strcat(file,buf);
	ofp.open(file, std::ofstream::out | std::ofstream::app);
	ofp << ".." << std::endl;
	ofp.close();
}

void setTransmitFrame(otRadioFrame *frame, int transmitcounter){
    char buf[15];
    sprintf(buf,"%d.bin",transmitcounter);
	char file[200];
	strcpy(file,LearnerPrefix);
	strcat(file,buf);
	ofp.open(file, std::ios::out | std::ios::binary);
	ofp.write((char*)frame->mPsdu, frame->mLength);
	ofp.close();
}





