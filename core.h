#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <cstdint>
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
#include <unistd.h>
#include <sys/types.h>
#include <curl/curl.h>
#include "tinyxml2.h"
#include "vlc/vlc.h"
//#include "vlc/libvlc_media.h"

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
    size_t last_read_byte_index = 0;
};

//Callback Function for CURL
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
int vlc_open_callback(void* opaque, void** datap, uint64_t* sizep);
void vlc_close_callback(void* opaque);
ssize_t vlc_read_callback(void *opaque, unsigned char* buffer, size_t length);
int vlc_seek_callback(void *opaque, size_t offset);

class curlwrapper{
  public:

    curlwrapper();
    std::string perform_request(std::string url);
    void perform_download(std::string url, BufferStruct *data);

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

    void getXML();
    void getXML(std::string);
    void Print();

    tinyxml2::XMLDocument xml;
    std::string last_output = "";

    // API Functions
    std::string getMusicFolders();
    std::string getArtists();
    std::string getMusicDirectory(std::string id);
    std::string getSong(std::string id);
    BufferStruct download(std::string id);

    BufferStruct last_song;
    //

    ~subsonicAPI();

  private:

    std::string assemble_url(std::string API_signature);
    std::string assemble_url(std::string API_signature, std::vector<std::pair<std::string, std::string>> *parameters);

    std::string baseurl = "";
    std::string username = "";
    std::string password = "";
    std::string application_name = "";
    curlwrapper cr;

};



#endif
