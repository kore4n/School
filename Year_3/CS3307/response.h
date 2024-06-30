/**
 * This file represents the header for the Response class
 * @author Victor Li (251124330)
 * @author Matthew Tennant (251164712)
 * @since 2023-03-14
 */

#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <map>
#include "CommandToHTTP.h"
using namespace std;

// Interface for Response class
class Response{
    private:
        string response;
        CommandToHTTP command;
    public:
        Response(string sentence,int checkNum, CommandToHTTP& comm);
        ~Response();
        string getResponse();
        void setResponse(string res);
};

#endif
