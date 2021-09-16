#include "vlcwrapper.h"


/*###############################livbVLC Callbacks##############################*/
int vlc_open_callback(void* opaque, void** datap, uint64_t* sizep){
  BufferStruct *buff = static_cast<BufferStruct*> (opaque);
  *sizep = buff->size;
  *datap = buff;
  buff->last_read_byte_index = 0;
  return 0;
}

int vlc_seek_callback(void *opaque, size_t offset){
  BufferStruct *buff = (BufferStruct *) (opaque);
  if (offset<=buff->size){
    buff->last_read_byte_index = offset;
    return 0;
  }
  else return -1;
}


ssize_t vlc_read_callback(void *opaque, unsigned char* buffer, size_t length){
  BufferStruct *buff = (BufferStruct *) (opaque);

  size_t bytes_to_copy = 0;
  size_t bytes_copied = buff->last_read_byte_index;
  size_t bytes_left = buff->size - bytes_copied;

  if (bytes_left >= length) bytes_to_copy = length;
  else if (bytes_left < length) bytes_to_copy = bytes_left;
  else return 0;

  char * data = buff->buffer;
  std::memcpy(buffer, &data[buff->last_read_byte_index], bytes_to_copy);
  buff->last_read_byte_index = buff->last_read_byte_index + bytes_to_copy;

  return bytes_to_copy;
}

void vlc_close_callback(void* opaque){
}
/*##############################################################################*/
