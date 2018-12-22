#ifndef CHANNEL_H
#define CHANNEL_H

#include "tcpUserSocket.h"
#include "user.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <thread>

namespace cs457
{
    using namespace std;
    using std::string;
    using std::vector;    
    class channel
    {
        public:
            channel(string name, string desc, string pass);

            //getters
            inline string getName(){return name;};
            inline string getDescription(){return description;};
            inline string getPassword(){return password;};
            inline vector<shared_ptr<user>> getUsers(){return users;};
            void removeUser(shared_ptr<user> u);

            //setters
            inline void setName(string n){name = n;};
            inline void setDescription(string d){description = d;};
            inline void setPassword(string p){password = p;};
            inline void addUser(shared_ptr<user> u){users.push_back(u);};


            
        private:
            string name;
            string description;
            string password;
            vector<shared_ptr<user>> users;

    };
}

#endif