#include "BotClient.h"


int main() {

    BotClient *bot = new BotClient("RoboAbt", "schinkenbrot");
    bot->start("irc.freenode.net", 6667, "AbtChannel");

    return 0;
}
