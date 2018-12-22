#include "chatCommands.h"

using namespace std;

cs457::chatCommands::chatCommands(cs457::tcpServerSocket s) : serverSock(s)
{
    init();
    getAllChannels();
    getAllUsers();
};

void cs457::chatCommands::init()
{
    commands.insert({{"/AWAY", AWAY},
                     {"/NICK", NICK},
                     {"/TIME", TIME},
                     {"/JOIN", JOIN},
                     {"/LIST", LIST},
                     {"/RULES", RULES},
                     {"/INFO", INFO},
                     {"/HELP", HELP},
                     {"/PART", PART},
                     {"/TOPIC", TOPIC},
                     {"/QUIT", QUIT},
                     {"/PASS", PASS},
                     {"/WHOIS", WHOIS},
                     {"/DIE", DIE},
                     {"/SETNAME", SETNAME},
                     {"/USER", USER},
                     {"/USERHOST", USERHOST},
                     {"/USERIP", USERIP},
                     {"/USERS", USERS},
                     {"/VERSION", VERSION},
                     {"/PRIVMSG", PRIVMSG},
                     {"/INVITE", INVITE},
                     {"/ISON", ISON},
                     {"/KILL", KILL},
                     {"/KICK", KICK},
                     {"/NOTICE", NOTICE},
                     {"/WHO", WHO}});
}

//add a new ueser to the user vector.
void cs457::chatCommands::addUsertoVec(shared_ptr<cs457::user> u) {
    users.push_back(u);
}

//http://www.cplusplus.com/forum/beginner/87238/
//takes in the channel text file to retrieve all already existing channels.
void cs457::chatCommands::getAllChannels()
{
    std::ifstream file("channels.txt");
    string line;
    while (getline(file, line))
    {
        istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        vector<string> tok(beg, end);
        string n = tok.at(0);
        string p = tok.at(tok.size() - 1);
        string d = concatDescription(tok);
        channels.push_back(cs457::channel(n, d, p));
    }
}

//retrieves all existing users from the user text file.
void cs457::chatCommands::getAllUsers()
{
    std::ifstream file("users.txt");
    string line;
    while(getline(file, line))
    {
        istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        vector<string> tok(beg, end);

        userFile.push_back(tok);
        
    }
}

//if the description of a channel is changed, this will update the channel text file to reflect this. 
void cs457::chatCommands::replaceDescription(string name, string d, string p)
{
    ifstream file;
    file.open("channels.txt");
    ofstream temp("temp.txt");
    string line;
    while (getline(file, line))
    {
        int fword = line.find(" ");
        if (line.substr(0, fword).compare(name) == 0)
        {
            temp << name + " " + d + " " + p << endl;
        }
        else
        {
            temp << line << endl;
        }
    }
    file.close();
    temp.close();
    rename("temp.txt", "channels.txt");
}

//takes a vector of strings and puts together all of the elements that consist of the description.
string cs457::chatCommands::concatDescription(vector<string> s)
{
    s.erase(s.begin());
    s.erase(s.end() - 1);
    string description;
    if (s.size() == 1)
    {
        return s.at(0);
    }
    vector<string>::iterator it = s.begin();
    while (it != s.end())
    {
        description += (*it) + " ";
        ++it;
    }
    return description;
}

//does the same as the previous method, but for another purpose. 
string cs457::chatCommands::concatDescription2(vector<string> s)
{
    s.erase(s.begin());
    s.erase(s.begin());
    string description;
    vector<string>::iterator it = s.begin();
    while (it != s.end())
    {
        description += (*it) + " ";
        ++it;
    }
    return description;
}

string cs457::chatCommands::getChannelsForList() {
    string all = "getChannels ";
    if(channels.size() == 0) {
        return " getChannels None";
    }
    else {
        vector<channel>::iterator iter = channels.begin();
        while(iter != channels.end()) {
            all += iter->getName() + " ";
            ++iter;
        }
        return all;
    }
}

//checks the command from the client. If the user is new, there is limited command options, otherwise they may use any command. 
string cs457::chatCommands::isValidCommand(string com, shared_ptr<cs457::user> u)
{  
    vector<string> part;
    std::stringstream p1(com);
    string inter;
    while (getline(p1, inter, ' '))
    {
        part.push_back(inter);
    }

    if(part[0].compare("getChannels") == 0) {
        return getChannelsForList();
    }

    if(commands.find(part[0]) == commands.end()) {
        char f = com[0];
        if(f != '#') {
            return "Not a valid command";
        }
    }

    if (u->getnewCon() == true) {
        switch (commands.find(part[0])->second)
        {
            case NICK: 
                return nick(part, u);
            case PASS:
                return pass(part, u);
            case QUIT:
                return to_string(u->getSocket()->closeSocket());
            case DIE:
                return "shutdown";
            case TIME: 
                return serverTime();
            case RULES:
                return rules();
            case INFO:
                return info();
            case HELP:
                return help();
            case VERSION:
                return version();
            default:
                return "You have to provide a username and password to do this! \nPlease provide one of each with /NICK nad /PASS.";

        }
    }

    if (commands.find(part[0]) == commands.end())
    {
        char c = com[0];
        if (c == '#' && part.size() != 1)
        {
            return sendMessage(part, u);
        }
        else
        {
            return "This is not a valid command.";
        }
    }
    else
    {
        switch (commands.find(part[0])->second)
        {
        case NICK:
            return nick(part, u);
        case TIME:
            return serverTime();
        case JOIN:
            return join(part, u);
        case LIST:
            return list(part);
        case RULES:
            return rules();
        case INFO:
            return info();
        case HELP:
            return help();
        case PART:
            return partChannel(part, u);
        case TOPIC:
            return topic(part);
        case QUIT:
            return quit(u);
        case PASS:
            return pass(part, u);
        case AWAY:
            return away(part, u);
        case WHOIS:
            return whois();
        case DIE:
            return "shutdown";
        case SETNAME:
            return setName(part, u);
        case USER:
            return userCommand(part, u);
        case USERHOST:
            return userHost(part, u);
        case USERIP:
            return userIP(part, u);
        case USERS:
            return listUsers();
        case VERSION:
            return version();
        case PRIVMSG:
            return privateMessage(part, u, "pm");
        case INVITE:
            return invite(part, u);
        case ISON:
            return ison(part, u);
        case KILL:
            return kill(part, u); 
        case KICK:
            return kick(part, u);
        case NOTICE:
            return privateMessage(part, u, "notice");
        case WHO:
            return who(part, u);
        default:
            return "Invalid command";
        }
    }
}

//client quits the server. 
string cs457::chatCommands::quit(shared_ptr<cs457::user> u) {
    u->getSocket()->closeSocket();
    return "quit";
}

//will set a new nickname if it is not already existing in the users.txt
string cs457::chatCommands::nick(vector<string> s, shared_ptr<cs457::user> u)
{
    if (s.size() == 1)
    {
        return "You must also provide a nickname.";
    }
    else if(u->getUserName() != "anon" && !u->getaddedPass()){
        return "You must enter your password to change your nickname.";
    }
    else if(u->getUserName() != "anon") {
        ifstream file;
        file.open("users.txt");
        ofstream temp("temp.txt");
        string line;
        while(getline(file, line))
        {
            int fword = line.find(" ");
            if(line.substr(0, fword) == u->getUserName())
            {
                temp << s[1] + " " + u->getPassword() + " " + u->getLevel() + " " + u->getBanned() << endl;;
            }
            else
            {
                temp << line << endl;
            }
        }
        file.close();
        temp.close();
        rename("temp.txt", "users.txt");
        u->setUserName(s[1]);
        return "Your name was successfully changed to " + s[1] + ", please add a password to your account using /PASS <password>.";
    }
else{
    for (vector<vector<string>>::iterator it = userFile.begin(); it != userFile.end(); ++it)
    {
        if ((*it).at(0).compare(s[1]) == 0)
        {
            u->setUserName(s[1]);
            u->setPassword((*it).at(1));
            u->setLevel((*it).at(2));
            u->setBanned((*it).at(3));
            return "Enter your password with /PASS <password>";
        }
    }   
    u->setUserName(s[1]);
    u->setaddedName(true);
    string t = "Nickname was successfully changed to ";
    return t + u->getUserName();
}
}

//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
//gets the time and date of the server
string cs457::chatCommands::serverTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X on %d-%m-%Y", &tstruct);
    string t = "This server's time is ";
    return t + buf;
}

//adds a user to a channel. If the channel doesn't already exist, it creates the channel and then adds them to it. 
string cs457::chatCommands::join(vector<string> s, shared_ptr<cs457::user> u)
{
    if (s.size() == 1)
    {
        return "Please prove the name(s) of the channels you would like to join.";
    }

    std::ofstream outfile;
    outfile.open("channels.txt", std::ios_base::app);
    string result;
    bool added = false;

    for (vector<string>::iterator iter = s.begin() + 1; iter != s.end(); ++iter)
    {
        bool added = false;
        if (channels.size() != 0)
        {
            vector<channel>::iterator it = channels.begin();
            while (it != channels.end() && !added)
            {
                if (it->getName().compare(*iter) == 0)
                {
                    it->addUser(u);
                    result += "Added to #" + *iter + "\n";
                    added = true;
                }
                ++it;
            }
        }
        //if channel doesn't exist already, create and add
        if (!added)
        {
            outfile << *iter << " null @\n";
            channels.push_back(channel(*iter, "null", "@"));
            getChannel(*iter)->addUser(u);
            result += "Created and joined " + *iter + "\n";
        }
    }

    return result;
}

//returns the requested channel
vector<cs457::channel>::iterator cs457::chatCommands::getChannel(string ch)
{
    vector<channel>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (it->getName().compare(ch) == 0)
        {
            return it;
        }
        ++it;
    }
    return it;
}

//lists all of the channels. If the user lists a channel with the commands, it will retrieve the description of those channels. 
string cs457::chatCommands::list(vector<string> s)
{
    string result1 = "Current channels are: \n";
    string result2 = "Topics of requested channels: \n";

    if (s.size() == 1)
    {
        vector<channel>::iterator it = channels.begin();
        while (it != channels.end())
        {
            result1 += "#" + it->getName() + "\n";
            ++it;
        }
        return result1;
    }
    else
    {
        vector<string>::iterator iter = s.begin() + 1;
        for (iter; iter != s.end(); ++iter)
        {
            for (vector<channel>::iterator it = channels.begin(); it != channels.end(); ++it)
            {
                if (it->getName().compare(*iter) == 0)
                {
                    string temp = it->getName();
                    string temp2 = it->getDescription();
                    result2 += "#" + temp + ": " + temp2 + "\n";
                }
                ++it;
            }
        }
        return result2;
    }
}

//Returns the rules of the server from a text file. 
string cs457::chatCommands::rules()
{
    std::ifstream file("rules.txt");
    string line;
    string result = "Welcome! Here are your server rules:\n";
    while (getline(file, line))
    {
        result += line + "\n";
    }
    return result;
}

//Obtains the port number and address of the server. 
string cs457::chatCommands::info()
{
    uint p = serverSock.getport();
    string a = serverSock.getaddress();
    if (a.length() == 0)
    {
        a = "127.0.0.1";
    }
    return "Server is running with address " + a + " on port " + to_string(p);
}


//gets the read me file which explains what each command is and how it works.
string cs457::chatCommands::help()
{
    std::ifstream file("README.txt");
    string line;
    string result = "Welcome! Here are your server rules:\n";
    while (getline(file, line))
    {
        result += line + "\n";
    }
    return result;
}

//removes the user from the requested channels. 
string cs457::chatCommands::partChannel(vector<string> s, shared_ptr<cs457::user> u)
{
    string result;
    if (s.size() == 1)
    {
        return "Please add what channel(s) would you like to part with.";
    }

    for (vector<string>::iterator iter = s.begin() + 1; iter != s.end(); ++iter)
    {
        vector<channel>::iterator it = channels.begin();
        while (it != channels.end())
        {
            if (it->getName().compare(*iter) == 0)
            {
                if (it->getUsers().size() != 0)
                    it->removeUser(u);
                result += "you have left #" + *iter + "\n";
            }
            ++it;
        }
    }
    return result;
}

//changes the topic of a channel, or gives the topic. 
string cs457::chatCommands::topic(vector<string> s)
{
    if (s.size() == 1) {
        return "you must provide a channel and/or description.";
    }
    else if (s.size() == 2)
    {
        return getChannel(s.at(1))->getDescription();
    }
    else
    {
        getChannel(s.at(1))->setDescription(concatDescription2(s));
        replaceDescription(getChannel(s.at(1))->getName(), getChannel(s.at(1))->getDescription(), getChannel(s.at(1))->getPassword());
        return "You have changed the topic of #" + s.at(1) + " to " + getChannel(s.at(1))->getDescription();
    }
}

//sends a message to the channel requested. 
string cs457::chatCommands::sendMessage(vector<string> s, shared_ptr<cs457::user> u)
{
    string c = s.at(0).substr(1);
    vector<channel>::iterator amIReal = getChannel(c);
    if (amIReal == channels.end())
    {
        return "This channel does not exist.";
    }
    else
    {
        vector<shared_ptr<cs457::user>> userList = amIReal->getUsers();
        string m = s.at(0) + ": " + u->getUserName() + ": ";

        s.erase(s.begin());
        vector<string>::iterator it = s.begin();
        while (it != s.end())
        {
            m += (*it) + " ";
            ++it;
        }

        m+= "\n";

        for (vector<shared_ptr<cs457::user>>::iterator it = userList.begin(); it != userList.end(); ++it)
        {
            thread childT1(&cs457::tcpUserSocket::sendString, (*it)->getSocket(), m, true);
            childT1.join();
        }
        return "Your message was successfully sent";
    }
}

//sets the pasword of a user. 
string cs457::chatCommands::pass(vector<string> s, shared_ptr<cs457::user> u)
{
    if(s.size() != 2) {
        return "You must provide a password to set.";
    }
    else if (s.at(1).find_first_of('@') != string::npos) {
        return "Your password may not contain the @ symbol.";
    }
    else if ((u->getPassword().compare(s.at(1)) == 0) && (u->getUserName() != "anon")) {
        u->setaddedName(true);
        u->setaddedPass(true);
        u->setnewCon(false);
        return "You have successfully logged in as " + u->getUserName();
    }
    else if ((u->getPassword() != s.at(1)) && (u->getUserName() != "anon") && u->getaddedPass()) {
        return "Incorrect password, try again.";
    }
    else{
        u->setPassword(s.at(1));
        u->setaddedPass(true);

        if(u->getaddedName()==true && u->getaddedPass()==true && u->getnewCon()==true) {
        u->setnewCon(false);

        std::ofstream out;
        out.open("users.txt", std::ios::app);
        out << u->getUserName() << " " << u->getPassword() << " " << u->getLevel() << " " << u->getBanned() << "\n";
        out.close();
        }

        return "Your password was successfully changed.";
    }
}

//sets the away message of a user if they were to get a private message from someone. 
string cs457::chatCommands::away(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() == 1) {
        u->setAwayMessage("");
        return "Away message was removed." + u->getAwayMessage();
    }
    else
    {
        s.erase(s.begin());
        string m;
        vector<string>::iterator it = s.begin();
        while (it != s.end())
    {
        m += (*it) + " ";
        ++it;
    }
        u->setAwayMessage(m);
        return "Your away message was set to: " + u->getAwayMessage();
    }
}

// Lists all of the users. 
string cs457::chatCommands::whois() {
    string result;
    if(users.size() == 1) {
        result += users.at(0)->getUserName();
        return result;
    }
    vector<shared_ptr<cs457::user>>::iterator it = users.begin();
    while(it != users.end()) {
        result += it->get()->getUserName() + ", ";
        ++it;
    }
    return result;
}

string cs457::chatCommands::setName(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() == 1) {
        return "you must provide a name.";
    }
    else if(s.size() > 2) {
        return "your name can only be one word.";
    }
    else {
        u->setName(s[1]);
        return "your name has been changed to " + s[1];
    }
}

string cs457::chatCommands::userCommand(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 4) {
        return "you must provide a hostname, server name, and real name.";
    }
    else {
        u->setHostName(s[1]);
        u->setServerName(s[2]);
        u->setName(s[3]);
        return "Host name set to " + u->getHostName() + ", server name set to " + u->getServerName() + " and real name set to " + u->getName();
    }
}

string cs457::chatCommands::userHost(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 2) {
        return "Your hostname is " + u->getHostName();
    }
    else if(s.size() == 2) {
        vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
        while(iter != users.end()) {
            if(iter->get()->getUserName().compare(s[1]) == 0) {
                return s[1] + " is using hostname " + iter->get()->getHostName();
            }
            ++iter;
        }
        return s[1] + " does not exist.";
    }
    return "Hostname lookup failed.";
}

string cs457::chatCommands::userIP(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 2) {
        return "Your IP is " + u->getSocket()->getAddress();
    }
    else if(s.size() == 2) {
        vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
        while(iter != users.end()) {
            if(iter->get()->getUserName().compare(s[1]) == 0) {
                return s[1] + "'s IP is " + iter->get()->getSocket()->getAddress();
            }
            ++iter;
        }
        return s[1] + " does not exist.";
    }
    return "IP lookup failed.";
}

string cs457::chatCommands::listUsers() {
    string u = "";
    vector<vector<string>>::iterator iter = userFile.begin();
    while(iter != userFile.end()) {
        u += (*iter).at(0);
        ++iter;
        if(iter != userFile.end()) {
            u+= ", ";
        }
    }
    return u;
}

string cs457::chatCommands::version() {
    return "Server is running on version 2.0";
}

string cs457::chatCommands::privateMessage(vector<string> s, shared_ptr<cs457::user> u, string type) {
    if(s.size() < 3) {
        return "You must provide both a user and your message to send a private message.";
    }
    else {
        string m = u->getUserName() + " to you: ";
        string msg = "";

        s.erase(s.begin());
        vector<string>::iterator it = s.begin();
        ++it;
        while (it != s.end())
        {
            msg += (*it) + " ";
            ++it;
        }
        vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
        while(iter != users.end()) {
            if(iter->get()->getUserName().compare(s[0]) == 0) {
                m += msg;
                thread childT1(&cs457::tcpUserSocket::sendString, iter->get()->getSocket(), m, true);
                childT1.join();
                if(iter->get()->getAwayMessage() == "" || type == "notice") {
                    return "'" +  msg + "' was sent to " + s[0];
                }
                else {
                    return s[0] + " " + iter->get()->getAwayMessage();
                }
            }
            ++iter;
        }
        return s[0] + " does not exist.";
    }
}

//invite must include an existing user as well as one existing channel
string cs457::chatCommands::invite(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 3) {
        return "You must provide both a username and a channel for an invite.";
    }
    else {
        string m = u->getUserName() + " has invited you to join " + s[2] + "!";
        vector<shared_ptr<cs457::user>>::iterator uiter = users.begin();
        vector<channel>::iterator citer = channels.begin();
        while(uiter != users.end()) {
            if(uiter->get()->getUserName().compare(s[1]) == 0) {
                while(citer != channels.end()){
                    if(citer->getName().compare(s[2]) == 0) {
                        thread childT1(&cs457::tcpUserSocket::sendString, uiter->get()->getSocket(), m, true);
                        childT1.join();
                        return "You invited " + s[1] + " to join " + s[2];
                    }
                    ++citer;
                }
                return "You can't invite " + s[1] + " to a channel that doesn't exist!";
            }
            ++uiter;
        }
        return s[1] + " is not a valid username.";
    }
}

string cs457::chatCommands::ison(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 2) {
        return "Please provide the user you are looking for.";
    }
    vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
    while(iter != users.end()) {
        if(iter->get()->getUserName().compare(s[1]) == 0) {
            if(iter->get()->getAwayMessage() == "") {
                return s[1] + " is online.";
            }
            else {
                return s[1] + " is currently away.";
            }
        }
        ++iter;
    }
    return s[1] + " does not exist.";
}

string cs457::chatCommands::kill(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 2) {
        return "Please provide who you are trying to boot from the server.";
    }
    string m = "You have been booted off of the server. Restart your app if you would like to try and log back in.";
    vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
    while(iter != users.end()) {
        if(iter->get()->getUserName().compare(s[1]) == 0) {
            thread childT1(&cs457::tcpUserSocket::sendString, iter->get()->getSocket(), m, true);
            childT1.join();
            iter->get()->getSocket()->closeSocket();
            return s[1] + " was booted from the server";
        }
        ++iter;
    }
    return s[1] + " does not exist or is not online.";
}

string cs457::chatCommands::kick(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 3) {
        return "Provide a username and a channel.";
    }
    else {
        string m = "You were kicked from #" + s[2];
        vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
        while(iter != users.end()) {
            if(iter->get()->getUserName().compare(s[1]) == 0) {
                vector<channel>::iterator it = channels.begin();
                while (it != channels.end()) {
                    if (it->getName().compare(s[2]) == 0) {
                        string before = to_string(it->getUsers().size());
                        vector<shared_ptr<cs457::user>>::iterator chaniter = it->getUsers().begin();
                        while(chaniter != it->getUsers().end()) {
                            if(chaniter->get()->getUserName().compare(s[1]) == 0) {
                                it->removeUser(*iter);
                                thread childT1(&cs457::tcpUserSocket::sendString, iter->get()->getSocket(), m, true);
                                childT1.join();
                                return s[1] + " has been kicked from " + s[2];
                            }
                            ++chaniter;
                        }
                        return s[1] + " is not a member of " + s[2];
                    }
                    ++it;
                }
                return s[2] + " is not a channel.";
            }
            ++iter;
        }
        return s[1] + " does not exist.";
    }
    return "Kick was unsuccessful.";
}


string cs457::chatCommands::who(vector<string> s, shared_ptr<cs457::user> u) {
    if(s.size() != 2) {
        return "Provide a username to lookup.";
    }
    else {
        vector<shared_ptr<cs457::user>>::iterator iter = users.begin();
        while(iter != users.end()) {
            if(iter->get()->getUserName().compare(s[1]) == 0) {
                return s[1] + "'s real name is " + iter->get()->getName() + " and they are using ip: " + iter->get()->getSocket()->getAddress();
            }
            ++iter;
        }
        return s[1] + " does not exist.";
    }
}