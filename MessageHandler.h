#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <vector>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <string>

using namespace std;

class MessageHandler {
public:
    MessageHandler(string msg);
    ~MessageHandler() {};

    string          sender;
    vector<string>  message;

    int atPosition(string msgVar);
};

#endif
