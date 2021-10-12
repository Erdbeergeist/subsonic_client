#ifndef VLCWRAPPER_H
#define VLCWRAPPER_H

#include "subsonic-client.h"
#include "mediaObjects.h"

int vlc_open_callback(void* opaque, void** datap, uint64_t* sizep);
void vlc_close_callback(void* opaque);
ssize_t vlc_read_callback(void *opaque, unsigned char* buffer, size_t length);
int vlc_seek_callback(void *opaque, size_t offset);
void eventCallback(const libvlc_event_t *p_event, void *p_data);

class vlcwrapper{
  public:
    vlcwrapper();
    ~vlcwrapper();
    void setMedia(memoryMediaObject *mediaObject);
    int play();
    int stop();
    int checkStop();
    void pause();
    void parseAsync();
    bool isPlaying();
    int getTime();
    void setTime(int time);

    libvlc_instance_t * vlcInstance;
    libvlc_media_player_t *mediaPlayer;
    libvlc_media_t *media;
    libvlc_event_manager_t *playerEvents;

    bool playing = false;
    bool waitingOnEvent = false;

  private:
    void attachEvent(libvlc_event_type_t event);

};

#endif
