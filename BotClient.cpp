#include "BotClient.h"

BotClient::BotClient(string nname, string pw)
{
    this->nname = nname;
    this->pw = pw;
    this->logger = false;
    conn = NULL;
    SQLChecker::init("sqlchecker.sqlite");
}

BotClient::~BotClient()
{
    delete conn;
}

void BotClient::BotClientLoop()
{
    string message;
    for (;;) {
        if (!conn->Receive(message))
            break;
        cout << message << endl;
        conn->PingPong(message);
        if (ChatBotFunctions(message) == -1)
            break;
    }
}

void BotClient::ConnectBotClient(string host, int port, string channel)
{
    conn = new ConnextionIRC(host, port, channel);
    conn->Connect();
    conn->Identify(nname, pw, channel);
    BotClientLoop();
    conn->Disconnect();
}


void BotClient::ChangeTopic(string topic)
{
    conn->Send("TOPIC #" + conn->GetChannel() + " " + topic + "\r\n");
}
void BotClient::Nick(string nname)
{
    conn->Send("NICK " + nname + "\r\n");
}
void BotClient::User(string username)
{
    conn->Send("USER " + username + " 0 0  :" + username + "\r\n");
}
void BotClient::Join(string channel)
{
    conn->SetChannel(channel);
}
void BotClient::Leave(string channel)
{
    conn->Send("PART #" + channel + "\r\n");
}

void BotClient::Logging(bool logger)
{
    this->logger = logger;
}
void BotClient::ClearLog()
{
    SQLChecker::clearTable();
}
void BotClient::LogChat(string name, string msg) {
    int t = time(0);
    stringstream date; date << t;
    SQLChecker::insertLog(date.str().c_str(), name.c_str(), msg.c_str());
}
void BotClient::ShowLog()
{
    string chat_log = SQLChecker::getChatLog();
    if (!chat_log.empty())
        conn->Send("PRIVMSG #" + conn->GetChannel() + "  :" + chat_log + "\r\n");
    else
        conn->Send("PRIVMSG #" + conn->GetChannel() + "  :Kein Log vorhanden...\r\n");
}
void BotClient::ShowLastSeen(string nname)
{
    string date = SQLChecker::getLastSeen(nname.c_str());
    if (!date.empty())
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :" + date);
    else
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :" + nname + " wurde noch nicht geloggt...\r\n");
}


int BotClient::ChatBotFunctions(string buffer)
{
    MessageHandler data(buffer);
    int pos=0;

    if (logger)
    {
        stringstream buf;
        for(int i=0;i<(int)data.message.size();i++)
            buf << data.message[i] << " ";
        if (data.message.size() > 0)
            LogChat(data.sender, buf.str());
    }

    if( (pos=data.atPosition("-change_nick")) != -1 && (int)data.message.size() > pos+1 )
    {
        Nick(data.message[pos+1]);
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :name changed\r\n");
    }
    if( (pos=data.atPosition("-join")) != -1 && (int)data.message.size() > pos+1 )
    {
        Join( data.message[pos+1] );
    }
    if( (pos=data.atPosition("-change_topic")) != -1 && (int)data.message.size() > pos+1)
    {
        ChangeTopic(data.message[pos+1]);
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :topic changed\r\n");
    }
    if( (pos=data.atPosition("-leave")) != -1 && (int)data.message.size() > pos+1 )
    {
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :ciao!\r\n");
        Leave( data.message[pos+1] );
    }
    if((pos=data.atPosition("-show_chatlog")) != -1 )
    {
        ShowLog();
    }
    if ((pos=data.atPosition("-last_seen_user")) != -1 && data.message.size() > pos+1)
    {
        ShowLastSeen(data.message[pos+1]);
    }
    if ((pos=data.atPosition("-quit")) != -1 )
    {
        conn->Send("PRIVMSG #" + conn->GetChannel() + " :cya!\r\n");
        conn->Send("QUIT cya\r\n");
        return -1;
    }
    if ((pos=data.atPosition("-log")) != -1 && (int)data.message.size() > pos+1 ) {
        if (data.message[pos+1] == "on" )
        {
            logger = true;
            conn->Send("PRIVMSG #" + conn->GetChannel() + " :logging on\r\n");
        } else if( data.message[pos+1] == "off" )
        {
            logger = false;
            conn->Send("PRIVMSG #" + conn->GetChannel() + " :logging off\r\n");
        }
    }

    return 0;
}
