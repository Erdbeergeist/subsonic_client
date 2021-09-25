#include "subsonicAPI.h"
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

std::string subsonicAPI::getArtist(std::string id){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getArtist", &parameters));
  subsonicAPI::getXML();
  return subsonicAPI::last_output;
}

std::string subsonicAPI::getAlbum(std::string id){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  subsonicAPI::last_output = subsonicAPI::cr.perform_request(subsonicAPI::assemble_url("getAlbum", &parameters));
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

void subsonicAPI::download(std::string id, memoryMediaObject *memObj){
  std::pair<std::string, std::string> p;
  p.first = "id";
  p.second = id;
  std::vector<std::pair<std::string, std::string>> parameters {p};
  subsonicAPI::cr.perform_download(subsonicAPI::assemble_url("download", &parameters), &memObj->data);

  std::ofstream download;
  download.open("RiverFlowsInYou.flac", std::ios::out | std::ios::binary);
  download.write(memObj->data.buffer, memObj->data.size);
}
/*##############################################################################*/
