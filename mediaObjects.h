#ifndef MEDIAOBJECTS_H
#define MEDIAOBJECTS_H

#include "subsonic-client.h"

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

#endif
