#ifndef BOTCLIENT_H
#define BOTCLIENT_H

#include <vector>
#include <algorithm>
#include <sstream>

#include <sqlite3.h>

#include "ConnextionIRC.h"
#include "SQLChecker.h"
#include "MessageHandler.h"

class BotClient
{
public:
    BotClient(string nname, string pw);
    ~BotClient();

    ConnextionIRC      *conn;

    string          nname;
    string          pw;
    bool            logger;

    void            BotClientLoop();
    void    ConnectBotClient(string host, int port, string channel);
    int     ChatBotFunctions(string buffer);
    void    Nick(string nname);
    void    User(string username);
    void    Join(string channel);
    void    Leave(string channel);
    void    ChangeTopic(string topic);
    void    Logging(bool logger);
    void    ClearLog();
    void    LogChat(string name, string msg);
    void    ShowLog();
    void    ShowLastSeen(string nname);

};

#endif
