/**
 * This file represents the header for the CommandToHTTP class
 * @author Victor Li (251124330)
 * @since 2023-03-14
 */


#ifndef COMMANDTOHTTP_H
#define COMMANDTOHTTP_H
#include <iostream>
#include <map>
using namespace std;

// Interface for CommandToHTTP class
class CommandToHTTP{
    private:
        map<string, string> requests;
    public:
        CommandToHTTP(string filePath);
        ~CommandToHTTP();
        void addRequestFromFile(string fileName);
        void addRequest(string name, string url);
        void deleteRequest(string name);
        string getRequest(string name);
        string getRequestWithParams(string name, string parameters[], int size);

};

#endif
