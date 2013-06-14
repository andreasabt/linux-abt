#include "BotClient.h"

// Konstruktor
BotClient::BotClient(string botname, string password)
{
    this->botname = botname;
    this->password = password;
}
// Destruktor
BotClient::~BotClient()
{
}

// Hauptschleife des Bots
int BotClient::start(string host, int port, string channel)
{
    connectIrc(host, port, channel);
    login();
    while(true)
    {
        char buffer[BUF_SIZE+1] = {0};
        if (recv(sock, buffer, BUF_SIZE*sizeof(char), 0) < 0)
        {
            perror("empfangen fehlgeschlagen!");
            disconnect();
            exit(1);
        }
        cout << buffer;
        if (parse(string(buffer)) == -1)
            break;
    }
    disconnect();
    return 0;
}

// Verbindung zum IRC-Server
void BotClient::connectIrc(string host, int port, string channel)
{
    this->host = host;
    this->port = port;
    this->channel = channel;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socketerzeugung fehlgeschlagen");
        disconnect();
        exit(1);
    }
    hostent *hp = gethostbyname(host.c_str());
    if(!hp)
    {
        perror("gethostbyname() fehlgeschlagen");
        disconnect();
        exit(1);
    }
    sockaddr_in sin;
    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_port = htons(port);
    memset(&(sin.sin_zero), 0, 8*sizeof(char));

    if (connect(sock, (sockaddr*) &sin, sizeof(sin)) == -1)
    {
        perror("connect fehlgeschlagen");
        disconnect();
        exit(1);
    }
}
// Verbindung trennen
void BotClient::disconnect()
{
    close(sock);
}
// Zum Senden von Nachrichten an den Server
void BotClient::sendmsg(string msg)
{
    send(sock, msg.c_str(), msg.length(), 0);
}
// Ping-Pong-Behandlung
void BotClient::ping(string buf)
{
    size_t pp = buf.find("PING");
    if (pp != string::npos)
    {
        string pong("PONG"+buf.substr(pp+4)+"\r\n");
        cout << pong;
        sendmsg(pong);
    }
}
// Parsing einer ankommenden Nachricht
int BotClient::parse(string buf)
{
    if (buf.find("\r\n") == buf.length()-2)
        buf.erase(buf.length()-2);
    ping(buf);
    return botaction(buf);
}

// Nachrichten, auf die der Bot reagieren soll
int BotClient::botaction(string buf)
{
    size_t pos = 0;
    transform(buf.begin(), buf.end(), buf.begin(), ::tolower);
    if ((pos = buf.find(":say ")) != string::npos)
    {
        sendmsg("PRIVMSG #" + channel + " :" + buf.substr(pos+5)+"\r\n");
    }
    if (buf.find(":User!User@User.user.freenode.net") == 0 && buf.find("exit")!=string::npos)
    {
        sendmsg("PRIVMSG #" + channel + " :Tschüss!\r\n");
        disconnect();
        return -1;
    } return 0;
}
void BotClient::login()
{
    sendmsg("NICK " + botname + "\r\n");
    sendmsg("USER " + botname + " 0 0  :" + botname + "\r\n");
    sendmsg("PRIVMSG NickServ IDENTIFY " + password + "\r\n");
    sendmsg("JOIN #" + channel + "\r\n");
    sendmsg("PRIVMSG #" + channel + ":Hallo!!\r\n");
}
