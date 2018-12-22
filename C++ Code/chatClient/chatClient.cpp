#include <netinet/in.h> 
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "chatClient.h"

using namespace std;

cs457::chatClient::chatClient(uint portNumber): port(portNumber), address("127.0.0.1"){
};

cs457::chatClient::chatClient(string networkAddress, uint portNumber): port(portNumber), address(networkAddress){
};

void cs457::chatClient::init()
{
    //here I may have differences with server 
    //check after testing
    clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    const char * cstr = address.c_str();
    bzero(&serverAddress,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr = addr;
    //string addresscpy(inet_ntoa(addr));
    //address = addresscpy;  
    serverAddress.sin_port = htons(port);
}

void cs457::chatClient::closeSocket()
{
    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
    serverSocket = 0;
}

//https://www.geeksforgeeks.org/socket-programming-cc/
int cs457::chatClient::connectToServer()
{
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    serverSocket = sock;

    //if connection is successful, return 0.
    return 0; 
};

ssize_t cs457::chatClient::sendMessage(const string &data, bool useMutex)
{
    if (data.size() == 0) return 0;                 
    int bufferSize = data.size()+1; 
    vector<char> stringBuffer(data.c_str(), data.c_str() + data.size() + 1u);

    ssize_t rval; 
    rval = send(serverSocket, &stringBuffer[0], bufferSize, 0);

    return rval;
};

tuple<string,ssize_t> cs457::chatClient::recvString(int bufferSize, bool useMutex)
{
    char stringBuffer[bufferSize]; 
    memset(stringBuffer, 0, sizeof(stringBuffer));    //change made here. Zeros out buffer.

    ssize_t recvMsgSize;

   if (useMutex)
    {
        lock_guard<mutex> lock(recvMutex);
        recvMsgSize = recv(serverSocket, stringBuffer, bufferSize, 0); 
    }    
    else
    {
        recvMsgSize = recv(serverSocket, stringBuffer, bufferSize, 0); 
    }
    
    
   
    return make_tuple(string(stringBuffer),recvMsgSize);     
};
