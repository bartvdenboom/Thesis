LINK_REQ LINK_ACC LINK_ACR LINK_REJ LINK_ADV DATA_REQ DATA_RES PARE_REQ PARE_RES CHIL_REQ CHIL_RES CHIL_URQ CHIL_URS ANNO DISC_REQ DISC_RES DEST_UNR0 DEST_UNR1 DEST_UNR2 DEST_UNR3 DEST_UNR4 DEST_UNR5 DEST_UNR6 PACK_TOO[0] TIME_EXCE0 TIME_EXCE1 PARA_PROB0 PARA_PROB1 PARA_PROB2 ECHO_REQ0 ECHO_REP0


void learn2()
{
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
        std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
    }

    listen(client, 1);
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    
    std::string data;
    std::string response;
    int state = 0;
    char recvData[bufsize+1]={0};

    while (server > 0) 
    {       //char *recvData = new char[bufsize+1];
            
            int lenRead = recv(server, recvData, bufsize, 0);
            //printf("%i",lenRead);
            //for( int i = 0 ; i < (bufsize) ; i ++ ){
            //        std::cout << recvData[i]; 
            //        }
            if (lenRead == -1)
                recvData[lenRead] = '\n';
            else if (lenRead == 0){
                //strncpy(data, "", sizeof(data));
                data = "";
            }
            else
                recvData[bufsize] = '\n'; 
                data="";
                data.assign(strtok(recvData,"\n"),0,strlen(recvData+1));
                if(data == "RESET")
                {
                    state = 0;
                    response = "DONE\n";
                }
                else if (data == "A")
                {
                    if(state==0){
                        response = "C\n";
                    }
                    else{
                        response = "D\n";
                    }
                    
                    
                }
                else if (data == "B")
                {   
                    state = (1-state);
                    response = "E\n";
                }
                else
                {
                    response = "\n";
                }
                send(server,response.data(), response.size(),0);
    }

    close(client);
    
}