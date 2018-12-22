#include "channel.h"

cs457::channel::channel(string n, string d, string p): name(n), description(d), password(p){};

void cs457::channel::removeUser(shared_ptr<user> u) {
    vector<shared_ptr<user>>::iterator itr = users.begin();
    while(itr != users.end()) {
        if(itr->get()->getUserName().compare(u->getUserName()) == 0){
            users.erase(itr);
            break;
        }
        ++itr;
    }
};



