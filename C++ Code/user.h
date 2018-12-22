#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <string>
#include "tcpUserSocket.h"

namespace cs457
{
    using namespace std;
    using std::string;
    
    class user
    {
        public:
            user(shared_ptr <tcpUserSocket> sock);
            
            //getters
            inline string getUserName(){return username;};
            inline string getPassword(){return password;};
            inline shared_ptr <tcpUserSocket> getSocket(){return sock;};
            inline string getBanned() {return banned;};
            inline string getLevel() {return level;};
            inline bool getnewCon() {return newCon;};
            inline bool getaddedName() {return addedName;};
            inline bool getaddedPass() {return addedPass;};
            inline string getAwayMessage() {return awayMessage;};
            inline string getHostName() {return hostname;};
            inline string getServerName() {return servername;};
            inline string getName() {return name;};


            //setters
            inline void setUserName(string n){username = n;};
            inline void setPassword(string p){password = p;};
            inline void setBanned(string b) {banned = b;};
            inline void setLevel(string l) {level = l;};
            inline void setnewCon(bool f) {newCon = f;};
            inline void setaddedName(bool f) {addedName = f;};
            inline void setaddedPass(bool f) {addedPass = f;};
            inline void setAwayMessage(string s) {awayMessage = s;};
            inline void setName(string s) {name = s;}; 
            inline void setHostName(string s) {hostname = s;};
            inline void setServerName(string s) {servername = s;};

        private:
            string username = "anon";
            string name = "unknown";
            string password = "@";
            string hostname = "default";
            string servername = "default";
            bool addedName = false;
            bool addedPass = false;
            shared_ptr<tcpUserSocket> sock;
            string banned = "false";
            string level = "user";
            bool newCon = true;
            string awayMessage;

    };
}

#endif