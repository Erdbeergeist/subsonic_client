#ifndef MEDIAOBJECTS_H
#define MEDIAOBJECTS_H

#include "subsonic-client.h"

class subsonicAPI;

//Buffer Struct to hold the Result
struct BufferStruct{
    char * buffer = NULL;
    std::size_t size = 0;
    std::size_t last_read_byte_index = 0;
};


class memoryMediaObject{
public:

  memoryMediaObject();
  ~memoryMediaObject();
  BufferStruct data;
};

class song{
public:
  song();
  song(std::string song_title, std::string song_id);
  ~song();
  void fillmetadata(std::string song_title, std::string song_id);

  memoryMediaObject data;
  bool isInitialized = false;
  std::map<std::string, std::string> metadata;
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
    void scanFullLibrary(subsonicAPI *sAPI);
    void createLibraryXML();

    std::vector<artist> artists;

  private:
    std::string defaultLibXMLFileName = "Library.xml";

};
#endif
