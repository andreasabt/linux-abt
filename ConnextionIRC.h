#ifndef CONNEXTIONIRC_H
#define CONNEXTIONIRC_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BUFSIZE 1024

using namespace std;

class ConnextionIRC {
public:
    ConnextionIRC(string host, int port, string channel);
    ~ConnextionIRC();

    int sock;

    void    Connect();
    void    Disconnect();

    void    Send(string msg);
    bool    Receive(string &buf);
    void    PingPong(string buffer);
    string  ParseIrc(string buffer);
    void    Identify(string name, string pw, string channel);
    void    SetChannel(string channel);
    string  GetChannel();

private:
    string  host;
    int     port;
    string  channel;
};

#endif
