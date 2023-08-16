/**
 * The Response class is used to act as a middle man between CommandToHTTP and Request;
 * It grabs the URL from CommandToHTTP to create a Request class and storing the response as a string.
 * @author Victor Li (251124330)
 * @author Matthew Tennant (2511)
 * @author Shahzore Khan (250957012)
 * @since 2023-03-14
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "response.h"
#include "request.h"
#include "json.hpp"
#include "Reminder.h"
#include "LocalFiles.h"
#include "commandCenter.h"
// Namespaces for convenience
using json = nlohmann::json;
using namespace std;

/**
 * The constructor for the Response class, used to get the response of an action
 * @param sentence The series of words used to be transcribed into an API response
 * @param num Specifies whether this is the first time Response is
 * @param comm A reference to the single CommandToHTTP object
 */
Response::Response(string sentence, int num, CommandToHTTP& comm) : command(comm){
    if ((sentence.find("change voice") != string::npos) || num == 0){
        string voice_id;
        char audioResponse[999] = "";
        // The user wishes for the voice to sound like Former President Barack Obama
        if ((sentence.find("change voice to Obama and say") != string::npos)){
            // Stem out the un-needed information
            string commandWord = "change voice to Obama and say ";
            voice_id = "RSjKECyneCaZo0BtzEIY";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
        }
        // The user wants the voice to sound like Former President Donald Trump.
        else if ((sentence.find("change voice to Trump and say") != string::npos)){
            // Stem out the un-needed information
            string commandWord = "change voice to Trump and say ";
            voice_id = "7RyPMK0cnru9KGF5dQbY";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
        }
        // The user wants the voice to sound like my friend Drew who helped me test out the voice function.
        else if ((sentence.find("change voice to Drew and say") != string::npos)){
            // Stem out the un-needed information
            string commandWord = "change voice to Drew and say ";
            voice_id = "mDCFRJ7EecKPHmLRRpnd";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
        }
        // The user either asked for a voice that doesn't exist or didn't ask for a specific voice.
        else {
            voice_id = "TxGEqnHWrfWFTfGW9XjX";
        }

        // Creating a new response file to get the text information that will be used to create the audio file
        Response response(sentence, 1, comm);
        string postField = R"({"text": "{}", "voice_settings": {"stability": 0, "similarity_boost":0}})";
        int index = postField.find("{}");
        postField.replace(index, 2, response.getResponse());
        strcat(audioResponse, postField.c_str());
        
        // Setting the link to receive the .mp3 file.
        string link = "https://api.elevenlabs.io/v1/text-to-speech/" + voice_id;
        string fileName = "aiVoice.mp3";
        
        // Get API key from JSON
        // Parses the specified JSON file
        ifstream f("Requests.json");
        json apiJSON = json::parse(f);

        // Goes through each element in the JSON
        string apiKey = apiJSON.at("elevenLabs").at("apikey");
        Request elaGPT(link, apiKey, audioResponse, fileName); 
    }
    else if ((sentence.find("weather") != string::npos) || (sentence.find("Weather") != string::npos)){
        // Parse the JSON result
        Request req(command.getRequest("Weather"));
        req.execute();
        json weatherJSON = json::parse(req.result());

        // Asked for 7 day forecast
        if ((sentence.find("weekly") != string::npos)){
            weatherJSON = weatherJSON.at("daily");
            string fullString = "";

            // Loop through each day to grab needed information
            for (int i = 1; i < weatherJSON.size(); i++){
                fullString += to_string(i) + " day after today's current weather is " + to_string(weatherJSON[i].at("weather")[0].at("description"))
                        + " with the temperature being " + to_string(weatherJSON[i].at("temp").at("day")) + " degrees.";
            }
            fullString.erase(std::remove(fullString.begin() + 1, fullString.end() - 1, '\"'), fullString.end());
            setResponse(fullString);
            cout << "Chungus Response: " << fullString << "\n";
        }

        // Asked for specific day
        else if ((sentence.find("on") != string::npos)){

            // Stem out the un-needed information
            string commandWord = "weather on ";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());

            // Parse the JSON result
            string sentenceArray[1] = {sentence};
            Request newReq(command.getRequestWithParams("Timemachine Weather", sentenceArray, 1));
            newReq.execute();
            json weatherJSON = json::parse(newReq.result());

            // Grab needed information from JSON
            weatherJSON = weatherJSON.at("data")[0];
            string temperature = to_string(weatherJSON.at("temp"));
            cout << temperature << "\n";
            string weather = weatherJSON.at("weather")[0].at("description");
            setResponse("The current weather is " + weather + " and it is currently " + temperature + " degrees.");
            cout << "Chungus Response: " << response << "\n";
        }

        // Asked for today's weather
        else{
            // Grab needed information from JSON
            weatherJSON = weatherJSON.at("current");
            string temperature = to_string(weatherJSON.at("temp"));
            string weather = weatherJSON.at("weather")[0].at("description");
            setResponse("The current weather is " + weather + " and it is currently " + temperature + " degrees.");
            cout << "Chungus Response: " << response << "\n";
        }
    }
    else if ((sentence.find("news") != string::npos) || (sentence.find("News") != string::npos)) {
        // Asked for a specific topic
        if ((sentence.find("topic") != string::npos)) {

            // Stem out the un-needed information
            string commandWord = "news topic ";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());

            // Parse the JSON result
            string sentenceArray[1] = {sentence};
            Request req(command.getRequestWithParams("News Topic", sentenceArray, 1));
            req.execute();
            json newsJSON = json::parse(req.result());

            // Grab needed information from JSON
            newsJSON = newsJSON.at("data");
            string fullString = "Today's " + sentence +  " stories include ";
            // Loop through each day to grab needed information
            int num;
            for (int i = 0; i < newsJSON.size() - 1; i++) {
                fullString += to_string(newsJSON[i].at("title")) + ",";
                num = i;
            }
            fullString += to_string(newsJSON[num + 1].at("title"));
            fullString.erase(std::remove(fullString.begin() + 1, fullString.end() - 1, '\"'), fullString.end());

            setResponse(fullString);
            cout << "Chungus Response: " << response << "\n";
        }

        // Asked ro a specific day
        else if ((sentence.find("day") != string::npos)) {
            // Stem out the un-needed information
            string commandWord = "news day ";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());

            // Parse the JSON result
            string sentenceArray[1] = {sentence};
            Request req(command.getRequestWithParams("News Day", sentenceArray, 1));
            req.execute();
            json newsJSON = json::parse(req.result());

            // Grab needed information from JSON
            newsJSON = newsJSON.at("data");
            string fullString = sentence +  "'s top stories include ";
            // Loop through each day to grab needed information
            int num;
            for (int i = 0; i < newsJSON.size() - 1; i++) {
                fullString += to_string(newsJSON[i].at("title")) + ",";
                num = i;
            }
            fullString += to_string(newsJSON[num + 1].at("title"));
            fullString.erase(std::remove(fullString.begin() + 1, fullString.end() - 1, '\"'), fullString.end());

            setResponse(fullString);
            cout << "Chungus Response: " << response << "\n";
        }

        // Asked for today's top news
        else {
            // Stem out the un-needed information
            Request req(command.getRequest("News"));
            req.execute();
            json newsJSON = json::parse(req.result());

            // Grab needed information from JSON
            newsJSON = newsJSON.at("data");
            string fullString = "Today's top stories include: ";
            // Loop through each day to grab needed information
            int num;
            for (int i = 0; i < newsJSON.size() - 1; i++) {
                fullString += to_string(newsJSON[i].at("title")) + ",";
                num = i;
            }
            fullString += to_string(newsJSON[num + 1].at("title"));
            fullString.erase(std::remove(fullString.begin() + 1, fullString.end() - 1, '\"'), fullString.end());
            setResponse(fullString);
            cout << "Chungus Response: " << response << "\n";
        }
    }
    else if ((sentence.find("map") != string::npos) || (sentence.find("Map") != string::npos)){
        // Stem out un-needed information
        if ((sentence.find("destination") != string::npos)){
            string commandWord = "map destination ";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
        }

        // Split parameters into separate strings
        string start, dest;
        stringstream s(sentence);
        s>>start>>dest;
        string parameters[2] = {start, dest};

        // Parse the JSON result
        Request req(command.getRequestWithParams("Map Distance", parameters, 2));
        req.execute();
        json mapJSON = json::parse(req.result());

        // Grab needed information from the JSON
        mapJSON = mapJSON.at("routes")[0].at("legs")[0];
        start = mapJSON.at("start_address");
        dest = mapJSON.at("end_address");
        string distance = mapJSON.at("distance").at("text");
        string duration = mapJSON.at("duration").at("text");

        string fullString = start + " to " + dest + " is " + distance + " away and takes " + duration + " to get there.";
        setResponse(fullString);
        cout << "Chungus Response: " << response << "\n";
    }
    else if ((sentence.find("reminder") != string::npos) || (sentence.find("Reminder") != string::npos)){
        if ((sentence.find("reminder") != string::npos) && 
        (sentence.find("add") == string::npos) &&
        (sentence.find("view") == string::npos) &&
        (sentence.find("remove") == string::npos)){
        //Asked for reminder
        setResponse("Tell me if you want to add, view or delete a reminder");
        cout << "Chungus Response: " << response << "\n";
        }
        Reminder RL;
        //if user wants to add a reminder
        if ((sentence.find("add") != string::npos)){
            string commandWord = "add";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            RL.addReminder();
            setResponse("reminder added!");
            cout << "Chungus Response: " << response << "\n";
        }
        //if user wants to view reminders
        else if ((sentence.find("view") != string::npos)){
            string commandWord = "view";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            
            RL.viewReminder();
            setResponse("these are all your reminders!");
            cout << "Chungus Response: " << response << "\n";
        }
        //if user wants to remove a reminder
        else if ((sentence.find("remove") != string::npos)){
            string commandWord = "remove";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            
            RL.removeReminder();
            setResponse("exiting reminder deletion!");
            cout << "Chungus Response: " << response << "\n";

        }
    }
    //asked for file manipulation
    else if ((sentence.find("file") != string::npos) || (sentence.find("File") != string::npos)){
        if ((sentence.find("file") != string::npos) && 
        (sentence.find("update") == string::npos) &&
        (sentence.find("delete") == string::npos) &&
        (sentence.find("view") == string::npos)){
        
        setResponse("Tell me if you want to create, update, delete or view local files");
        cout << "Chungus Response: " << response << "\n";
        }
        LocalFiles localFileCommand;
        // if user wants to create a file
        if ((sentence.find("create") != string::npos)){
            string commandWord = "create";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            localFileCommand.createFile();
            setResponse("File created!");
            cout << "Chungus Response: " << response << "\n";
        }
        //if user wants to delete a file
        else if ((sentence.find("delete") != string::npos)){
            string commandWord = "delete";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            
            localFileCommand.deleteFile();
            setResponse("file deleted");
            cout << "Chungus Response: " << response << "\n";
        }
        //if user wants to update a file
        else if ((sentence.find("update") != string::npos)){
            string commandWord = "update";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            localFileCommand.updateFile();
            setResponse("file updated!");
            cout << "Chungus Response: " << response << "\n";
        }
        //if user wants to view all files
        else if ((sentence.find("view") != string::npos)){
            string commandWord = "view";
            int index = sentence.find(commandWord);
            sentence.erase(index, commandWord.length());
            localFileCommand.viewFiles();
            setResponse("showing all files!");
            cout << "Chungus Response: " << response << "\n";
        }
    }
    else{
        // Check if it's a custom command, unfortunately custom formatting isn't supported
        fstream apiNames("Requests.json");
        json api = json::parse(apiNames);
        api = api.at("function");
        for (int i = 0; i < api.size(); i++){
            if ((sentence.rfind(api[i]["name"],0) != string::npos)){
                Request req(command.getRequest(api[i]["name"]));
                req.execute();
                json customJSON = json::parse(req.result());
                string fullResponse = to_string(customJSON);
                fullResponse.erase(std::remove(fullResponse.begin() + 1, fullResponse.end() - 1, '\"'), fullResponse.end());
                setResponse(fullResponse);
                cout << "Chungus Response: " << response << "\n";
                return;
            }
        }

        // Format user input for ChatGPT Post Field
        string postField = R"({"model": "gpt-3.5-turbo","messages": [{"role": "user", "content": "{}"}]})";
        int index = postField.find("{}");
        postField.replace(index, 2, sentence);
        char chatResponse[999] = "";
        strcat(chatResponse, postField.c_str());

        // Get API key from JSON
        // Parses the specified JSON file
        ifstream f("Requests.json");
        json apiJSON = json::parse(f);

        // Goes through each element in the JSON
        string apiKey = apiJSON.at("chatGPT").at("apikey");

        // Create GET request using API key and user input
        Request chatGPT("https://api.openai.com/v1/chat/completions", apiKey, chatResponse);
        chatGPT.execute();

        // Parse response from Request
        json chatGPTJson = json::parse(chatGPT.result());
        setResponse(chatGPTJson.at("choices")[0].at("message").at("content"));
        cout << "Chungus Response: " << response << "\n";
    }
}

/**
 * The deconstructor for the Response class
 */
Response::~Response(){

}

/**
 * Setter method for response
 * @param res Response information
 */
void Response::setResponse(string res){
    response = res;
}


/**
 * Getter method response
 * @return Response information
 */
string Response::getResponse(){
    return response;
}
