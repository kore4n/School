/**
 * This file represents the header for the Request class
 * @author Victor Li (251124330)
 * @author Matthew Tennant (251164712)
 * @since 2023-03-14
 */


#ifndef REQUEST_H
#define REQUEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <curl/curl.h>

// Structure used to represent the JSON stored in memory for the Request class
struct MemoryStruct {
  char *memory; // Contents of JSON
  size_t size; // Length of JSON
};

// Interface for Request Class
class Request{
    private:
        CURL* curlHandle; // Used for cURL configures
        CURLcode res; // Response from GET request
        MemoryStruct chunk; // JSON memory chunk
    public:
        Request(std::string requestURL);
        Request(std::string requestURL, std::string token, char* info);
        Request(std::string requestURL, std::string token, char* info, std::string fileName);

        Request(const Request &oldRequest);
        ~Request();
        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
        static size_t write_to_file(void* contents, size_t size, size_t nmemb, FILE* userp);
        bool execute();
        std::string result();
};

#endif