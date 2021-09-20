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
  ~song();

  memoryMediaObject data;
  bool isInitialized = false;
  std::map<std::string, std::string> metadata;
};

class album{
  public:
    album();
    ~album();

    std::map<std::string, std::string> metadata;
    std::vector<song> songs;
};

class artist{
  public:
    artist();
    ~artist();


    std::map<std::string, std::string> metadata;
    std::vector<album> albums;
};

class mediaLibrary{
  public:
    mediaLibrary();
    ~mediaLibrary();
    void scanLibrary(subsonicAPI *sAPI);

    artist artists;

};
#endif
