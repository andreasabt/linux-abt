#include "BotClient.h"

BotClient::BotClient(string botname, string password)
{
    this->botname = botname;
    this->password = password;
}
BotClient::~BotClient()
{
}

// Verbindung zum IRC-Server
void BotClient::connect(string host, int port, string channel)
{
    this->host = host;
    this->port = port;
    this->channel = channel;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket");
        disconnect();
        exit(1);
    }

    hostent *hp = gethostbyname(host.c_str());
    if(!hp) {
        cerr << "gethostbyname()" << endl;
        irc_disconnect();
        exit(1);
    }

    sockaddr_in sin;
    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_port = htons(port);
    memset(&(sin.sin_zero), 0, 8*sizeof(char));

    if (connect(sock, (sockaddr*) &sin, sizeof(sin)) == -1) {
        perror("connect");
        disconnect();
        exit(1);
    }
}
// Verbindung trennen
void BotClient::disconnect()
{
    close(sock);
}
void BotClient::sendmsg(string msg)
{

}
void BotClient::ping(string buf)
{

}
void BotClient::parse(string buf)
{

}
void BotClient::botaction(string buf)
{

}
void BotClient::login()
{

}
