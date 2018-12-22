#include <iostream>
#include <string> 
#include <tuple> 
#include <thread> 
#include <vector> 
#include <memory> 
#include <fstream>
#include <sstream>
 
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include "chatCommands.h"
#include "user.h"
#include "channel.h"

using namespace std;

bool ready = true; 
string dbPath;
int port;
string banner;

void init() 
{
    vector<string> temp;
    ifstream conf("chatserver.conf");
    string line;
    while(getline(conf,line)){
        istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        vector<string> tok(beg,end);
        temp.push_back(tok.at(1)); 
        
    }
    port = stoi(temp.at(0));
    dbPath = temp.at(1);

    ifstream ban(dbPath + "/banner.txt");
    string b;
    while(getline(ban,b)){
        banner += b + "\n";
    }

}

int cclient(shared_ptr<cs457::user> user,int id, shared_ptr<cs457::chatCommands> com)
{

    thread childT1(&cs457::tcpUserSocket::sendString,user->getSocket().get(),banner,true);
    childT1.join();
    com->addUsertoVec(user);
    string msg;
    ssize_t val = 0;
    while (val != -1) 
    {

        tie(msg,val) = user->getSocket().get()->recvString();
        if(val != -1) {
            cout << "[SERVER] The client is sending message " << msg << endl;
            string send = com->isValidCommand(msg, user);

            if(send == "shutdown") {
                thread childTExit(&cs457::tcpUserSocket::sendString,user->getSocket().get(),"GOODBYE EVERYONE",false);
                childTExit.join(); 
                val = -1;
            }

            if(send.compare("Your message was successfully sent") != 0){
                string s =  "[SERVER REPLY] " + send + "\n"; 
                thread childT2(&cs457::tcpUserSocket::sendString,user->getSocket().get(),s,true);
                childT2.join();
            }
        }
    }
    cout << "Client with id of " << id << " has quit the server." << endl;
    return 1; 
}

int main(int argc, char * argv[])
{
    init();
    cout << "Initializing Socket" << std::endl; 
    cs457::tcpServerSocket mysocket(port);
    cout << "Binding Socket" << std::endl; 
    mysocket.bindSocket(); 
    cout << "Listening Socket" << std::endl; 
    mysocket.listenSocket(); 
    cout << "Waiting to Accept Socket" << std::endl;
    int id = 0; 
    vector<unique_ptr<thread>> threadList; 

    shared_ptr<cs457::chatCommands> command = make_shared<cs457::chatCommands>(mysocket);
  
    while (ready)
    { 
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val; 
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl; 
        cout << "Socket Accepted" << std::endl; 
        shared_ptr<cs457::user> u = make_shared<cs457::user>(clientSocket);
        unique_ptr<thread> t = make_unique<thread>(cclient, u, id, command); 
        threadList.push_back(std::move(t)); 
        
        id++; //not the best way to go about it. 
       // threadList.push_back(t); 
       
    
        
    }

    for (auto& t: threadList)
    {
        t.get()->join();
    }
    
        
    cout << "Server is shutting down after one client" << endl; 
    return 0; 
}