#include "core.h"


int main(int argc, char *argv[]) {

    subsonicAPI sAPI("https://192.168.1.155/index.php/apps/music/subsonic/rest/", "luxmit", "wpkuqfc3m17o", "subsonic-dev");

    std::cout<<sAPI.getMusicFolders()<<std::endl;

}
