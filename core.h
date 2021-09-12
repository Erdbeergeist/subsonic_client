#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <cmath>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <argp.h>
#include <tuple>
#include <typeinfo>
#include <curl/curl.h>


//Argument Parsing
error_t parse_opt(int key, char *arg, struct argp_state *state);

// The actual list of command line options we accept
static struct argp_option cmdline_options[] = {
  {"Test", 'x', "string", 0,"test"},
  { 0 }
};

//Buffer Struct to hold the Result
struct BufferStruct{
    char * buffer = NULL;
    size_t size = 0;
};

//Callback Function for CURL
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);


class curlwrapper{
  public:

    curlwrapper();
    std::string perform_request(std::string url);

    ~curlwrapper();

  private:

    CURL *curlhandle;
    CURLcode curlresult;
    BufferStruct curloutput;
    char errbuf[CURL_ERROR_SIZE];
};

class subsonicAPI{
  public:

    subsonicAPI();
    subsonicAPI(std::string username, std::string password);
    subsonicAPI(std::string baseurl, std::string username, std::string password);
    subsonicAPI(std::string baseurl, std::string username, std::string password, std::string application_name);

    void set_auth(std::string username, std::string password);
    void set_baseurl(std::string baseurl);
    void set_application_name(std::string application_name);
    std::string get_baseurl();
    std::string get_username();
    std::string get_password();

    // API Functions
    std::string getMusicFolders();
    //

    ~subsonicAPI();

  private:

    std::string assemble_url(std::string API_signature);
    std::string assemble_url(std::string API_signature, std::vector<std::string> parameters);

    std::string baseurl = "";
    std::string username = "";
    std::string password = "";
    std::string application_name = "";
    curlwrapper cr;

};



#endif
