#ifndef SUBSONICAPI_H
#define SUBSONICAPI_H

#include "subsonic-client.h"
#include "curlwrapper.h"

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
    void download(std::string id, memoryMediaObject *memObj); //Should really return if successful

    BufferStruct last_song;

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
