#include "mediaObjects.h"
#include "subsonicAPI.h"


/*##############################memoryMediaObject###############################*/
memoryMediaObject::memoryMediaObject(){
}

memoryMediaObject::~memoryMediaObject(){
}
/*##############################################################################*/

/*##################################artist######################################*/
artist::artist(){
}

artist::~artist(){
}
/*##############################################################################*/

/*##################################album#######################################*/
album::album(){
}

album::~album(){
}
/*##############################################################################*/

/*##################################song########################################*/
song::song(){
}

song::~song(){
}
/*##############################################################################*/

/*#################################mediaLibrary#################################*/
mediaLibrary::mediaLibrary(){
}

mediaLibrary::~mediaLibrary(){
}

void mediaLibrary::scanLibrary(subsonicAPI *sAPI){
  sAPI->getArtists();
  sAPI->getXML();
  for(tinyxml2::XMLElement* node=sAPI->xml.FirstChildElement()->FirstChildElement()->FirstChildElement();
  node;
  node=node->NextSiblingElement()){
    for (tinyxml2::XMLElement* indexname = node->FirstChildElement(); indexname; indexname=indexname->NextSiblingElement()){
    std::cout<<"i: "<<indexname->FindAttribute("name")->Value()<<std::endl;}
    //do something
  }
}
/*##############################################################################*/
