*-----------------------------------------------------------------------*
This project was created by Angela Root [eid: 830291232].

Some resources used were stackoverflow, piazza, geeks for geeks, and Jordan Peterson(from our class). 
Any code that was taken directly from a site has the website commented above it. 
*-----------------------------------------------------------------------*
To run this server-client socket programming assignment:
    1. After unzipping the contents of this project, start VSCode with the root_P1 directory.
    2. In Qt Creator open root_P1/chatClient
    3. Build and run the driver.cpp in VSCode to start up the server.
    4. Once the server is running and is "Waiting to accept socket" go to QT and build and run the clientDriver. 
    5. To have multiple users on the same machine just run the clientDriver as many times as you would like.
    6. Chat with others using all of the implememnted commands listed below.
*-----------------------------------------------------------------------*
Notes about the GUI in Qt:
    When a connection is made with the server, a new window will open.
    Within this window, after you have provided a nickname a list of all of the existing channels will be displayed on the left. 
    If you double click on one of the channels, as long as you are logged in, you will be added to that channel.
    Any channel that you have joined in your session will create a new tab.

    These tabs are not currently functional as far as seeing chats in the corresponding tab.
    However, if you press close on the tab, you will part from that channel. 
*-----------------------------------------------------------------------*
Here are the commands you may use and their descriptions:

/RULES
specifies what must be done after connecting to the server and a generic description of how to navigate.

/NICK
nick only allows a user to change their nickname if the name is not already used. User must have a nickname in order to join other chat rooms. 

/TIME
user can get the date and time of their current server, but not other servers. 

/JOIN
user is added to the requested channel or channels. list of channels should be seperated by whitespace, and if the channel does not already exist then the channel is created and the user is added to it as well.

/LIST
if no parameters are given, the server replies with all of the current channels.
otherwise the user can provide the channels it is looking for and the server will return the descriptions of each of the requested channels.
user is not able to specify server like in the RFC. 

/INFO
will let you know what port and ip address the server is currently runnnig on. 

/HELP
also known as this document. How to use the commands, and what they will return. 

/TOPIC
returns either the description of a specific requested channel, or allows you to change what the topic of a specific room is. 

/PART
allows a user to leave a channel that they were previously involved with. 

#[channel name] message
will send a message to the channel of your choice, for all other people that are currently using the channel to recieve. 

/PASS
allows user to set/change the password that goes with their current user information.

/AWAY
will send an automated message to anyone that asks to /PRIVMSG you while you are out. If you don't provide a message with the command, your previous away message will be erased.

/WHOIS
will give back the nicknames of all of the currently active users. 

/DIE
closes the clients connection with the server.

/SETNAME
once a user is logged in with a nickname and password, they are able to set their realname

/USER
takes in 3 arguments: hostname, servername, and realname. Will set all of these for your user as long as you are logged in.

/USERHOST
if no arguments are given, your hostname is returned. You may also provide one username and recieve back their hostname. 

/USERIP
just like /USERHOST, if no arguments are given, you will get back your IP address, if you provide one username you will recieve their IP address.

/USERS
returns a list of all of the users.

/VERSION
gives back the version of the server that is running.

/INVITE
provide a username and a channel namne and that user will recieve a message that you have invited them to that channel.

/ISON
takes in a username and will check if that user is currently on based on if they have set an away message or not. 

/KILL
allows you to provide a username and that user will be kicked off of the server. The kicked user may still view the last state of the server they had open, but must reconnect if they want to chat or do any other commands. 

/KICK
provide a username and a channel and if that user is a member of that channel they will be removed. 

/WHO
provide a username and you will get back what their realname is.

/QUIT
no arguments, and will close your connection with the server. 
*-----------------------------------------------------------------------*
