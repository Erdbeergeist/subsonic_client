#ifndef MEDIAOBJECTS_H
#define MEDIAOBJECTS_H

#include "subsonic-client.h"

class subsonicAPI;
class vlcwrapper;


//Buffer Struct to hold the Media data in Memorz
struct BufferStruct{
    char * buffer = NULL;
    std::size_t size = 0;
    std::size_t last_read_byte_index = 0;
    bool isComplete = false;
};


class memoryMediaObject{
public:

  memoryMediaObject();
  ~memoryMediaObject();
  BufferStruct buffer;
};

class song{
public:
  song();
  song(std::string song_title, std::string song_id, std::string song_size, std::string song_duration);
  ~song();
  void fillmetadata(std::string song_name, std::string song_id, std::string song_size, std::string song_duration);

  memoryMediaObject data;
  std::map<std::string, std::string> metadata;
  bool isDownloaded = false;
};

class album{
  public:
    album();
    album(std::string album_name, std::string album_id, std::string album_artist);
    ~album();
    void fillmetadata(std::string name, std::string id, std::string artist_name);

    std::map<std::string, std::string> metadata;
    std::vector<song> songs;
};

class artist{
  public:
    artist();
    artist(std::string name, std::string id, std::string album_count);
    ~artist();
    void fillmetadata(std::string name, std::string id, std::string album_count);

    std::map<std::string, std::string> metadata;
    std::vector<album> albums;
};

class mediaLibrary{
  public:
    mediaLibrary();
    ~mediaLibrary();
    void scanRemoteLibrary(subsonicAPI *sAPI);
    void scanLocalLibrary();
    void scanLocalLibrary(std::string libraryfile);
    void createLibraryXML();

    void getArtistNames(const char *artistnames);

    std::vector<artist> artists;

  private:
    std::string defaultLibXMLFileName = "Library.xml";

};

class mediaPlayer{
  public:
    mediaPlayer(subsonicAPI *sAPI, vlcwrapper *vlc, mediaLibrary *mediaLib);
    ~mediaPlayer();
    void play();
    void stop();
    void pause();
    void requestPlayback(int artist_idx, int album_idx, int song_idx);
    void ping(); //This function will initiate playback once a download has buffered enough or resume playback after a buffer pause

    vlcwrapper *vlc;
    subsonicAPI *sAPI;
    mediaLibrary *mediaLib;
    std::thread backgroundWorker;
    song *currentSong;
    size_t bufferAdvanceSize = 100000000; //How many bytes do we have to have downloaded before we begin playback

  private:
    bool isPlaying = false;
    bool isDownloading = false;
};
#endif
