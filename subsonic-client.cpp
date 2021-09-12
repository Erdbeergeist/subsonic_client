#include "core.h"
#include "tinyxml2.h"


int main(int argc, char *argv[]) {

    tinyxml2::XMLDocument xml;

    subsonicAPI sAPI();

    sAPI.getArtists();
    sAPI.Print();
    sAPI.getMusicDirectory("artist-76005");//track-38004
    sAPI.Print();
    sAPI.getSong("track-38004");//track-38004
    sAPI.Print();
    sAPI.download("track-38004");
    /*for(tinyxml2::XMLElement* node=xml.FirstChildElement()->FirstChildElement()->FirstChildElement();
    node;
    node=node->NextSiblingElement()){
        std::cout<<node->GetText()<<std::endl;
            //do something
    }*/
}
