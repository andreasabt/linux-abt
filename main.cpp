#include "BotClient.h"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        perror("RoboAbt braucht drei Argumente!!!");
        exit(1);
    }
    BotClient *chatBot = new BotClient("RoboAbt", "schinkenbrot");
    chatBot->ConnectBotClient(string(argv[1]), atoi(argv[2]), argv[3]);

    delete chatBot;
    return 0;
}
