/**
 * The CommandToHTTP class is used to read and write APIs from a JSON file,
 * storing the URL and its label as a Map/Dictionary.
 * @author Victor Li (251124330)
 * @since 2023-03-14
 */
#include <iostream>
#include <fstream>
#include "CommandToHTTP.h"
#include "json.hpp"

// Namespaces for convenience
using json = nlohmann::json;
using namespace std;

/**
 * The constructor for CommandToHTTP
 * @param filePath The path to the JSON file storing all the APIs.
 */
CommandToHTTP::CommandToHTTP(string filePath){
    // On Windows you have to use an absolute path, Linux relative path is fine
    addRequestFromFile(filePath);
}

/**
 * Deconstructor for CommandToHTTP
 */
CommandToHTTP::~CommandToHTTP(){

}

/**
 * Reads a JSON file to concatenate URL and API keys together and
 * then storing the information in a Map.
 * @param filePath The path to the JSON file storing all the APIs.
 */
void CommandToHTTP::addRequestFromFile(string filePath) {
    // Parses the specified JSON file
    ifstream f(filePath);
    json data = json::parse(f);

    // Goes through each element in the JSON
    data = data.at("function");
    for (int i = 0; i < data.size(); i++){
        // Grabs the URL
        string tempString = data[i]["url"];
        json innerJSON = data.at(i).at("fields");

        // Adds each field value into the URL queries
        for (string it : innerJSON){
            int index = tempString.find("{}");
            tempString.replace(index, 2, it);
        }

        // Stores in Map
        requests[data[i]["name"]] = tempString;
    }
}

/**
 * Adds/Changes a key and value in the Map
 * @param name Label for the API
 * @param url  The URL path
 */
void CommandToHTTP::addRequest(string name, string url) {
    requests[name] = url;
}

/**
 * Deletes a key an pair in the map
 * @param name Label for the API
 */
void CommandToHTTP::deleteRequest(string name) {
    requests.erase(name);
}

/**
 * Getter method for API requests that don't need additional parameters
 * @param name Label for the API
 * @return The URL path
 */
string CommandToHTTP::getRequest(string name) {
    return requests[name];
}

/**
 * Getter method for API requests that need additional input
 * @param name Label for the API
 * @param parameters The array of inputs
 * @param size Size of the array
 * @return The URL path
 */
string CommandToHTTP::getRequestWithParams(string name, string parameters[], int size) {
    // Get uncompleted URL
    string fullURL = requests[name];

    // If only 1 additional parameter, just add it to URL
    if (size == 1){
        int index = fullURL.find("{}");
        fullURL.replace(index, 2, parameters[0]);
        return fullURL;
    }

    // If more than 1 parameter, loop through array and add each element to the URL
    else{
        for (int i = 0; i < size; i++){
            int index = fullURL.find("{}");
            fullURL.replace(index, 2, parameters[i]);
        }
        return fullURL;
    }
}
