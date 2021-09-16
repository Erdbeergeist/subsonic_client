#include "curlwrapper.h"

// Function to pass to CURL, fills BufferStruct
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data){

    size_t realsize = size * nmemb;

    struct BufferStruct * mem = (struct BufferStruct *) data;

    //we need to cast this to char* since buffer is of the type char*
    mem->buffer = (char *) realloc(mem->buffer, mem->size + realsize + 1);

    if ( mem->buffer ){
        std::memcpy( &( mem->buffer[ mem->size ] ), ptr, realsize );
        mem->size += realsize;
        mem->buffer[ mem->size ] = 0;
    }
    return realsize;
}


/*#################################curlwrapper##################################*/
curlwrapper::curlwrapper(){
  curlwrapper::curlhandle = curl_easy_init();
  if (curlwrapper::curlhandle){
    curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEDATA, (void *)&(curlwrapper::curloutput));
    curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_ERRORBUFFER, curlwrapper::errbuf);
  }
}

curlwrapper::~curlwrapper(){
  curl_easy_cleanup(curlwrapper::curlhandle);
}

std::string curlwrapper::perform_request(std::string url){
  curlwrapper::curlresult = curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_URL, url.c_str());
  curlwrapper::curlresult = curl_easy_perform(curlwrapper::curlhandle);
  std::string output = (curlwrapper::curloutput).buffer;
  return output;
}

void curlwrapper::perform_download(std::string url, BufferStruct *buffer){
  curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEDATA, (void *)(buffer));
  curlwrapper::curlresult = curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_URL, url.c_str());
  curlwrapper::curlresult = curl_easy_perform(curlwrapper::curlhandle);
  curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEDATA, (void *)&(curlwrapper::curloutput));
}
/*##############################################################################*/
