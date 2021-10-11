#ifndef VLCWRAPPER_H
#define VLCWRAPPER_H

#include "subsonic-client.h"
#include "mediaObjects.h"

int vlc_open_callback(void* opaque, void** datap, uint64_t* sizep);
void vlc_close_callback(void* opaque);
ssize_t vlc_read_callback(void *opaque, unsigned char* buffer, size_t length);
int vlc_seek_callback(void *opaque, size_t offset);

class vlcwrapper{
  public:
    vlcwrapper();
    ~vlcwrapper();
    void setMedia(memoryMediaObject *mediaObject);
    void play();
    void stop();
    void pause();
    void parseAsync();
    bool isPlaying();
    int64_t getTime();

    libvlc_instance_t * vlcinstance;
    libvlc_media_player_t *mediaplayer;
    libvlc_media_t *media;

    bool playing = false;
};

#endif
