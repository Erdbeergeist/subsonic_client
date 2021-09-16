#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H

#include "subsonic-client.h"
#include "mediaObjects.h"

//Callback Function for CURL
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);

class curlwrapper{
  public:

    curlwrapper();
    std::string perform_request(std::string url);
    void perform_download(std::string url, BufferStruct *buffer);

    ~curlwrapper();

  private:

    CURL *curlhandle;
    CURLcode curlresult;
    BufferStruct curloutput;
    char errbuf[CURL_ERROR_SIZE];
};
#endif
