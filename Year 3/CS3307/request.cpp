/**
 * The Request class is used to send a HTTP request, and saves the response in the computer's memory
 * @author Victor Li (251124330)
 * @author Matthew Tennant (251164712)
 * @since 2023-03-14
 */

#include "request.h"
#include "json.hpp"

// Namespaces for convenience
using json = nlohmann::json;
/**
 * Request 
 * Default constructor for Request class
 * @param requestURL The URL where the JSON is stored
 * @return null
 */ 
Request::Request(std::string requestURL){
    // Allocate minimum memory to be expanded later
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_ALL);
    curlHandle = curl_easy_init();

    // Convert C++ string to C string for cURL method
    char* cstr = new char [requestURL.length() + 1];
    std::strcpy(cstr, requestURL.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_URL, cstr);

    // Send data and chunk into functions
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);

    // Add user-agent field
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

/**
 * Request
 * Constructor for Request class, with additional bearer token for authorization.
 * Mainly used for ChatGPT.
 * @param requestURL The URL where the JSON is stored
 * @param token The token used for the authorization header
 * @param info The information the user wants to ask ChatGPT.
 * @return null
 */
Request::Request(std::string requestURL, std::string token, char* info){
    // Allocate minimum memory to be expanded later
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_ALL);
    curlHandle = curl_easy_init();

    // Convert C++ string to C string for Header
    char* headerStr = new char [token.length() + 1];
    std::strcpy(headerStr, token.c_str());
    char headerString[99] = "Authorization: Bearer ";
    strcat(headerString, headerStr);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, headerString);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);

    // Insert message into data parameters
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, info);

    // Convert C++ string to C string for cURL method
    char* cstr = new char [requestURL.length() + 1];
    std::strcpy(cstr, requestURL.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_URL, cstr);

    // Send data and chunk into functions
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);

    // Add user-agent field
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}
/**
 * Request
 * Constructor for Request class, with additional bearer token for authorization.
 * This method is used for elvenLabs which is the API used to generate custom voices
 * and .mp3 file
 * @param requestURL The URL where the JSON is stored
 * @param token The token used for the authorization header
 * @param info The information the user wants to ask ChatGPT.
 * @param fileName the name of the .mp3 file that will be created.
 * @return null
 */
Request::Request(std::string requestURL, std::string token, char* info, std::string fileName){
    // Allocate minimum memory to be expanded later
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_ALL);
    curlHandle = curl_easy_init();

    // Convert C++ string to C string for Header
    char* headerStr = new char [token.length() + 1];
    std::strcpy(headerStr, token.c_str());
    char headerString[99] = "xi-api-key: ";
    strcat(headerString, headerStr);
    struct curl_slist* headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, headerString);
    headers = curl_slist_append(headers, "accept: audio/mpeg");
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);

    // Insert message into data parameters
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, info);

    // Convert C++ string to C string for cURL method
    char* cstr = new char [requestURL.length() + 1];
    std::strcpy(cstr, requestURL.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_URL, cstr);

    // Creating the file
    FILE* filePointer = fopen(fileName.c_str(), "wb");
    
    // Writing the recieved data into filePointer
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_to_file);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, filePointer);
    
    //save_to_file(curlHandle);
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK){
        std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << std::endl;
    }
    fclose(filePointer);
    system("cvlc --play-and-exit -q aiVoice.mp3 2> /dev/null &");
}
/**
 * Request
 * Copy constructor for Request class
 * @param Request The Request class to be copied onto the new one
 * @return null
 */ 

Request::Request(const Request &oldRequest){
    // Set new data members with old classes value
    chunk = oldRequest.chunk;
    res = oldRequest.res;
    curlHandle = oldRequest.curlHandle;
}

/**
 * ~Request
 * Destructor for Request class
 * @param null
 * @return null
 */ 
Request::~Request(){
    // Cleanup cURL and libcURL
    curl_easy_cleanup(curlHandle);
    free(chunk.memory);
    curl_global_cleanup();
}


/**
 * WriteMemoryCallback
 * Helper method used to write JSON into memory
 * @param contents The JSON content
 * @param size The size of the buffer
 * @param nmemb The size of each eleemnt per buffer
 * @param userp Pointer to the data
 * @return Size of the the data 
 */ 
size_t Request::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    // Get the size of the data    
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    // Resize buffer to hold both old and new data
    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);

    // Check if enough memory to store data
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    // Copy new data into the buffer
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);

    // Update size of buffer
    mem->size += realsize;

    // Terminate and return the size of the data
    mem->memory[mem->size] = 0;
    return realsize;
}
/**
 * When called writes the content
 * @param contents The JSON content
 * @param size The size of the buffer
 * @param nmemb The size of each element per buffer
 * @param stream a pointer to the file that it should be written to
 * @return Size of the data
*/
size_t Request::write_to_file(void* contents, size_t size, size_t nmemb, FILE* stream){
    size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

/**
 * execute
 * Method used to retrieve the document from the URL
 * @param null
 * @return A boolean value that represents if the retrieval was successful or not
 */ 
bool Request::execute(){
    res = curl_easy_perform(curlHandle);
    if (res == CURLE_OK) return true;
    else return false;
}

/**
 * result
 * Method used to return the document
 * @param null
 * @return A string representation of the document
 */ 
std::string Request::result(){
    return chunk.memory;
}