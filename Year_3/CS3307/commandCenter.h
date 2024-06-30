//
// Created by victor on 3/25/23.
//

#ifndef UNTITLED_COMMANDCENTER_H
#define UNTITLED_COMMANDCENTER_H

#include <string>
#include "CommandToHTTP.h"
#include "LocalFiles.h"

using namespace std;
class commandCenter {
    private:
        LocalFiles localFileCommand;
    public:
        commandCenter();
        ~commandCenter();
        void getResponse(string res, CommandToHTTP& comm);
};


#endif //UNTITLED_COMMANDCENTER_H
