/**
 * The commandCenter class is used to interpret a string into a series of actions.
 * @author Victor Li (251124330)
 * @since 2023-03-26
 */
#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include "commandCenter.h"
#include "response.h"
#include "json.hpp"
#include "Reminder.h"

// Namespaces for convenience
using json = nlohmann::json;
using namespace std;

/**
 * The constructor for the commandCenter class, used to get the string return of a command
 */
commandCenter::commandCenter() {
}

/**
 * The deconstructor for the commandCenter class
 */
commandCenter::~commandCenter() {
}
/**
 * The method that runs a function based on the input
 * @param res The command that the user specified
 */
void commandCenter::getResponse(string res, CommandToHTTP& comm) {

    // Deals with everything revolving the history command
    if (res.rfind("!history", 0) != string::npos){

        string commandWord = "!history";
        int index = res.find(commandWord);
        res.erase(index, commandWord.length());

        // Print out the history file
        if (res.length() == 0){
            fstream file("history.txt");
            string fileLine;
            int counter = 1;
            cout << "History of commands" << endl;
            while (getline(file, fileLine)) cout << to_string(counter++) + ". " + fileLine << endl;
            file.close();
        }

        else{
            // If parameter isn't a number, it's an unsupported command
            for (int x = 1; x < strlen(res.c_str()); x++){
                if(!isdigit(res.c_str()[x])){
                    cout << "Unknown command" << endl;
                    return;
                }
            }

            // Number has to be 1-10, otherwise it is out of range
            res.erase(0, 1);
            int selector = stoi(res);

            if (selector > 0 && selector <= 10){
                fstream file("history.txt");
                string fileLine;
                int counter = 1;
                // Keep iterating through the file until reached the desired line
                while (getline(file, fileLine)){
                    // Use line to create a response class and print the value
                    if (counter == selector){
                        cout << fileLine << endl;
                        Response userRes(fileLine, 0, comm);
                        cout << userRes.getResponse() << endl;
                    }
                    counter++;
                }
            }
            else{
                cout << "Out of range, please choose a number from 1-10" << endl;
            }
        }
    }
    else if (res.rfind("!favourite", 0) != string::npos){
        string commandWord = "!favourite";
        int index = res.find(commandWord);
        res.erase(index, commandWord.length());

        // Print out the history file
        if (res.length() == 0){
            fstream file("favourite.txt");
            string fileLine;
            int counter = 1;
            cout << "Favourite commands" << endl;
            while (getline(file, fileLine)) cout << to_string(counter++) + ". " + fileLine << endl;
            file.close();
        }
        else{
            // Add a command
            if (res.rfind("add", 1) != string::npos){
                string commandName;
                cout << "Enter command name: ";
                getline(cin, commandName);
                if (res.rfind("!", 0) != string::npos){
                    cout << "Cannot favourite command starting with !";
                    return;
                }
                else{
                    fstream file;
                    file.open("favourite.txt", fstream::app);
                    file << "\n" + commandName;
                    file.close();
                }
            }
            else{

                // If parameter isn't a number, it's an unsupported command
                for (int x = 1; x < strlen(res.c_str()); x++){
                    if(!isdigit(res.c_str()[x])){
                        cout << "Not a number." << endl;
                        return;
                    }
                }

                // Delete from file
                res.erase(0, 1);
                int selectedLine = stoi(res);
                fstream file("favourite.txt");
                string fileLine;
                int counter = 1;

                // Keep iterating through the file until reached the desired line
                while (getline(file, fileLine)){
                    // Use line to create a response class and print the value
                    if (counter == selectedLine){
                        cout << fileLine << endl;
                        Response userRes(fileLine, 0, comm);
                        cout << userRes.getResponse() << endl;
                        return;
                    }
                    counter++;
                }
                cout << "Unknown command" << endl;

            }
        }
    }
    else if (res.rfind("!command", 0) != string::npos){
        string commandWord = "!command ";
        int index = res.find(commandWord);
        res.erase(index, commandWord.length());

        // Add a custom command
        if (res.rfind("add", 0) != string::npos){
            string commandName;
            cout << "Enter command name: ";
            getline(cin, commandName);

            map<string, string> fields;
            string name[3];
            string value[3];
            int numFields = 0;
            for (int x = 0; x < 3; x++){
                cout << "Enter field name (max 3), type exit to continue to next step: ";
                getline(cin, name[x]);
                if (name[x].find("exit") != string::npos){
                    break;
                }
                cout << "Enter field value: ";
                getline(cin, value[x]);
                numFields++;
            }

            string url;
            cout << "Enter url (fields have to be included in url to work): ";
            getline(cin, url);

            // Update JSON file
            fstream file("Requests.json");
            json data = json::parse(file);
            file.close();
            if (numFields == 0){
                data.at("function").push_back({{"name", commandName}, {"url", url}});
            }
            else if (numFields == 1){
                data.at("function").push_back({{"name", commandName}, {"fields", {{name[0], value[0]}}}, {"url", url}});
            }
            else if (numFields == 2){
                data.at("function").push_back({{"name", commandName}, {"fields", {{name[0], value[0]}, {name[1], value[1]}}}, {"url", url}});
            }
            else if (numFields == 3){
                data.at("function").push_back({{"name", commandName}, {"fields", {{name[0], value[0]}, {name[1], value[1]}, {name[2], value[2]}}}, {"url", url}});
            }
            file.open("Requests.json", fstream::in | fstream::out | fstream::trunc);
            file << data;
            file.close();

        }

        // Remove a custom command
        if (res.rfind("delete", 0) != string::npos){
            fstream file("Requests.json");
            json data = json::parse(file);
            file.close();

            // Get name of request to delete from json
            string commandName;
            cout << "Enter command name: ";
            getline(cin, commandName);

            // Iterate through json to find correct element and thne update json file
            for (int i = 0; i < data.at("function").size(); i++){
                if (commandName.find(data.at("function")[i]["name"]) != string::npos){
                    data.at("function").erase(i);
                    cout << data.at("function") << endl;

                    file.open("Requests.json", fstream::in | fstream::out | fstream::trunc);
                    file << data;
                    return;
                }
            }
            cout << "Command does not exist" << endl;
        }
    }
    else if (res.rfind("!localFile", 0) != string::npos){
        int choice;
        cout << "Please choose an option: " << endl;
        cout << "1. Create a file" << endl;
        cout << "2. Update a file" << endl;
        cout << "3. Delete a file" << endl;
        cout << "4. View all files in the working directory" << endl;
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                localFileCommand.createFile();
                return;

            case 2:
                localFileCommand.updateFile();
                return;

            case 3:
                localFileCommand.deleteFile();
                return;

            case 4:
                localFileCommand.viewFiles();
                return;

            default:
                cout << "Invalid choice. Please try again." << endl;
                return;
        }
    }
    else if (res.rfind("!reminders", 0) != string::npos){
        Reminder RL;
        while (true) {
            // Prompt the user for an action
            cout << "Choose an action:" << endl;
            cout << "1. Add a reminder" << endl;
            cout << "2. View all reminders" << endl;
            cout << "3. Remove a reminder" << endl;
            cout << "4. Quit" << endl;
            string action;
            getline(cin, action);

            // Perform the chosen action
            if (action == "1") {
                RL.addReminder();
                
            } 
            
            else if (action == "2") {
                RL.viewReminder();
            }
            else if (action == "3") {
                RL.removeReminder();
            }

            else if (action == "4") {
                break;
            }
            else {
                cout << "Invalid action." << endl;
                continue;
            }
        }

    }
    else if (res.rfind("!help", 0) != string::npos){
        cout << "!history - Displays the last 10 requests \n"
                "!history # - Runs the #'d line in the history \n"
                "!favourite - Displays the requests the user favourited \n"
                "!favourite # - Runs the #'d line in the favourite list \n"
                "!command add - Adds a key-word that can be associated with an api call \n"
                "!command delete - Deletes a key-word that can be associated with an api call \n"
                "!localFile - Launches the LocalFile menu \n"
                "!reminders - Launches the Reminders menu \n"
                << endl;
    }
    else{
        cout << "Unknown command" << endl;
    }
}
