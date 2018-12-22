#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <map>
#include <fstream>
#include <iterator>

#include "channel.h"
#include "user.h"
#include "tcpServerSocket.h"

namespace cs457
{
    using namespace std;
    using std::string;
    using std::vector;
    using std::unordered_map;

    class chatCommands
    {
        public: 
            chatCommands(cs457::tcpServerSocket s);

            //setters
            inline void addChannel(channel c){channels.push_back(c);};
            void init();

            //getters
            vector<channel>::iterator getChannel(string ch);
            void getAllChannels();
            void getAllUsers();
            vector<shared_ptr<user>> getUserVector() {return users;};

            string concatDescription(vector<string> s);
            string concatDescription2(vector<string> s);
            void replaceDescription(string name, string d, string p);
            void fillUserVec();
            void addUsertoVec(shared_ptr<cs457::user> u);

            string isValidCommand(string s, shared_ptr<cs457::user> sock);

            //chat commands 
            string nick(vector<string> s, shared_ptr<cs457::user> u);
            string serverTime();
            string join(vector<string> s, shared_ptr<cs457::user> u);
            string list(vector<string> s);
            string rules();
            string info();
            string help();
            string partChannel(vector<string> s, shared_ptr<cs457::user> u);
            string topic(vector<string> s);
            string sendMessage(vector<string> s, shared_ptr<cs457::user> u);
            string pass(vector<string> s, shared_ptr<cs457::user> u);
            string away(vector<string> s, shared_ptr<cs457::user> u);
            string whois();
            string quit(shared_ptr<cs457::user> u);
            string setName(vector<string> s, shared_ptr<cs457::user> u);
            string userCommand(vector<string> s, shared_ptr<cs457::user> u);
            string userHost(vector<string> s, shared_ptr<cs457::user> u);
            string userIP(vector<string> s, shared_ptr<cs457::user> u);
            string listUsers();
            string version();
            string convertToUpper(string s);
            string privateMessage(vector<string> s, shared_ptr<cs457::user> u, string type);
            string invite(vector<string> s, shared_ptr<cs457::user> u);
            string ison(vector<string> s, shared_ptr<cs457::user> u);
            string kill(vector<string> s, shared_ptr<cs457::user> u);
            string kick(vector<string> s, shared_ptr<cs457::user> u);
            string who(vector<string> s, shared_ptr<cs457::user> u);
            string ping(vector<string> s);

            string getChannelsForList();

        private:
            vector<channel> channels;
            vector<shared_ptr<cs457::user>> users;
            vector<vector<string>> userFile;
            shared_ptr<cs457::tcpUserSocket> sock;
            cs457::tcpServerSocket serverSock;
            enum myEnum {AWAY, CONNECT, DIE, HELP, INFO, INVITE, ISON, JOIN, KICK, KILL, KNOCK, LIST,
                                MODE, NICK, NOTICE, PART, OPER, PASS, PING, PONG, PRIVMSG, QUIT, RESTART,
                                RULES, SETNAME, SILENCE, TIME, TOPIC, USER, USERHOST, USERIP, USERS, VERSION,
                                WALLOPS, WHO, WHOIS }; 
            unordered_map<string, myEnum> commands;


    };
}

#endif