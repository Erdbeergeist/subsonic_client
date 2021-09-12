#include "core.h"
#include "tinyxml2.h"

// -------------------------------------------------------------------------
// The main command line parser
// -------------------------------------------------------------------------
error_t parse_opt(int key, char *arg, struct argp_state *state){
  switch (key)
  {
    case 'x':
    {
      return strtod(arg,NULL);
      break;
    }
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}


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

/*#################################SUBSONIC_API#################################*/
subsonicAPI::subsonicAPI(){
}

subsonicAPI::subsonicAPI(std::string username, std::string password){
  subsonicAPI::set_auth(username, password);
}

subsonicAPI::subsonicAPI(std::string baseurl, std::string username, std::string password){
  subsonicAPI::set_baseurl(baseurl);
  subsonicAPI::set_auth(username, password);
}

subsonicAPI::subsonicAPI(std::string baseurl, std::string username, std::string password, std::string application_name){
  subsonicAPI::set_baseurl(baseurl);
  subsonicAPI::set_auth(username, password);
  subsonicAPI::set_application_name(application_name);
}

void subsonicAPI::set_baseurl(std::string baseurl){
  subsonicAPI::baseurl = baseurl;
}

void subsonicAPI::set_application_name(std::string application_name){
  subsonicAPI::application_name = application_name;
}

void subsonicAPI::set_auth(std::string username, std::string password){
  subsonicAPI::username = username;
  subsonicAPI::password = password;
}

std::string subsonicAPI::get_baseurl(){
  return subsonicAPI::baseurl;
}
std::string subsonicAPI::get_username(){
  return subsonicAPI::username;
}
std::string subsonicAPI::get_password(){
  return subsonicAPI::password;
}

subsonicAPI::~subsonicAPI(){
}

std::string subsonicAPI::assemble_url(std::string API_signature){
  std::string full_url = subsonicAPI::baseurl + API_signature + "?u=" + subsonicAPI::username +
                         "&p=" + subsonicAPI::password + "&c=" + subsonicAPI::application_name;
  return full_url;
}

std::string subsonicAPI::assemble_url(std::string API_signature, std::vector<std::pair<std::string, std::string>> *parameters){
  std::string full_url = subsonicAPI::baseurl + API_signature + "?u=" + subsonicAPI::username +
                         "&p=" + subsonicAPI::password + "&c=" + subsonicAPI::application_name;

  for (std::pair<std::string, std::string> element : *parameters){full_url = full_url + "&" + element.first + "=" + element.second;}
  return full_url;
}

void subsonicAPI::getXML(){
  subsonicAPI::xml.Parse(subsonicAPI::last_output.c_str());
}

void subsonicAPI::getXML(std::string raw_xml){
  subsonicAPI::xml.Parse(raw_xml.c_str());
}

void subsonicAPI::Print(){
  std::cout<<subsonicAPI::last_output<<std::endl;
}

// API Functions
std::string subsonicAPI::getMusicFolders(){
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getMusicFolders"));
  subsonicAPI::getXML();
  return subsonicAPI::last_output;
}

std::string subsonicAPI::getArtists(){
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getArtists"));
  subsonicAPI::getXML();
  return subsonicAPI::last_output;
}

std::string subsonicAPI::getMusicDirectory(std::string id){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getMusicDirectory", &parameters));
  subsonicAPI::getXML();
  return subsonicAPI::last_output;
}

std::string subsonicAPI::getSong(std::string id){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getSong", &parameters));
  subsonicAPI::getXML();
  return subsonicAPI::last_output;
}

BufferStruct subsonicAPI::download(std::string id){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  BufferStruct data;
  subsonicAPI::cr.perform_download(subsonicAPI::assemble_url("download", &parameters), &data);

  /*std::ofstream download;
  download.open("RiverFlowsInYou.flac", std::ios::out | std::ios::binary);
  download.write(data.buffer, data.size);*/
}
/*##############################################################################*/


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

void curlwrapper::perform_download(std::string url, BufferStruct *data){
  curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEDATA, (void *)(data));
  curlwrapper::curlresult = curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_URL, url.c_str());
  curlwrapper::curlresult = curl_easy_perform(curlwrapper::curlhandle);
  curl_easy_setopt(curlwrapper::curlhandle, CURLOPT_WRITEDATA, (void *)&(curlwrapper::curloutput));
}
/*##############################################################################*/
