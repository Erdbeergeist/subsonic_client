#include "subsonic-client.h"
#include "mediaObjects.h"
#include "curlwrapper.h"
#include "subsonicAPI.h"
#include "vlcwrapper.h"

int main(int argc, char *argv[]) {

    tinyxml2::XMLDocument xml;

    subsonicAPI sAPI();
    mediaLibrary mediaLib;

    mediaLib.scanLibrary(&sAPI);


  //  sAPI.getMusicDirectory("artist-95533");
   //sAPI.getMusicDirectory("album-47767");//track-38004
    //sAPI.Print();
    sAPI.getSong("track-47768");//track-38004
    sAPI.Print();


    memoryMediaObject* song = new memoryMediaObject();
    std::cout<<"Downloading..."<<std::endl;
    sAPI.download("track-47768", song);
  // std::ofstream download;
//    download.open("RiverFlowsInYou.flac", std::ios::out | std::ios::binary);
  //  download.write(&song->data.buffer, song->data.size);
    //char* buffer;
    //char *data = song.buffer;
    //memcpy(buffer, data, 1);

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    inst = libvlc_new (0, NULL);
    m = libvlc_media_new_callbacks(inst,
                                   vlc_open_callback,
                                   vlc_read_callback,
                                   vlc_seek_callback,
                                   NULL,
                                   &song->data);
    mp = libvlc_media_player_new_from_media (m);

    std::cout<<"Playing..."<<std::endl;
    libvlc_media_player_play(mp);
    sleep(400);

    libvlc_media_release(m);
    libvlc_media_player_release(mp);
    libvlc_release(inst);
}
