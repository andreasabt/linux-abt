#ifndef BOTCLIENT_H
#define BOTCLIENT_H

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

#include <sqlite3.h>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class BotClient {
public:
    BotClient(string botname, string password);
    ~BotClient();

    int sock;

    string host;
    int port;
    string channel;

    string botname;
    string password;

    void connect(string host, int port, string channel);
    void disconnect();

    void sendmsg(string msg);

    void ping(string buf);
    void parse(string buf);
    void botaction(string buf);

    void login();

};

#endif
