#ifndef CHAT_CLIENT
#define CHAT_CLIENT

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <memory>
#include <cstring>
#include <vector>
#include <tuple>
#include <mutex>


namespace cs457
{
    using namespace std;

    class chatClient
    {
        public:
            chatClient(uint portNumber);
            chatClient(string networkAddress, uint portNumber);
            int connectToServer();
            ssize_t sendMessage(const string &data, bool useMutex);
            tuple<string,ssize_t> recvString(int bufferSize, bool useMutex);
            void init();
            void closeSocket();


        private:
            struct sockaddr_in serverAddress;
            struct sockaddr_in clientAddress;
            uint port;
            string address;
            struct in_addr addr;


            int serverSocket;
            int clientSocket;

            mutex sendMutex;
            mutex recvMutex;
    };
}
#endif
