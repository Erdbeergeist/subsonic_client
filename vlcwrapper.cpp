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
  //else return 0;

  char * data = buff->buffer;
  std::memcpy(buffer, &data[buff->last_read_byte_index], bytes_to_copy);
  buff->last_read_byte_index = buff->last_read_byte_index + bytes_to_copy;
  std::cout<<"VLC READ_CALLBACK SIZE||LAST_BYTE: "<<buff->size<<"\t"
           <<buff->last_read_byte_index<<"\t"
          <<std::endl;
  return bytes_to_copy;
}

void vlc_close_callback(void* opaque){
}
/*##############################################################################*/


/*################################VLC WRAPPER###################################*/
vlcwrapper::vlcwrapper(){
  vlcwrapper::vlcinstance = libvlc_new (0, NULL);
  vlcwrapper::mediaplayer = libvlc_media_player_new(vlcwrapper::vlcinstance);
}

vlcwrapper::~vlcwrapper(){
  libvlc_media_release(vlcwrapper::media);
  libvlc_media_player_release(vlcwrapper::mediaplayer);
  libvlc_release(vlcwrapper::vlcinstance);
}

void vlcwrapper::setMedia(memoryMediaObject *mediaObject){
  vlcwrapper::media = libvlc_media_new_callbacks(vlcwrapper::vlcinstance,
                                       vlc_open_callback,
                                       vlc_read_callback,
                                       vlc_seek_callback,
                                       NULL,
                                       &mediaObject->buffer);
  libvlc_media_player_set_media(vlcwrapper::mediaplayer, vlcwrapper::media);
}

void vlcwrapper::play(){
  libvlc_media_player_play(vlcwrapper::mediaplayer);
  vlcwrapper::playing = true;
}

void vlcwrapper::stop(){
  libvlc_media_player_stop_async(vlcwrapper::mediaplayer);
  vlcwrapper::playing = false;
}

void vlcwrapper::parseAsync(){
  libvlc_media_parse_async(vlcwrapper::media);
}

void vlcwrapper::pause(){
  libvlc_media_player_pause(vlcwrapper::mediaplayer);
}

bool vlcwrapper::isPlaying(){
  return libvlc_media_player_is_playing(vlcwrapper::mediaplayer);
}

int64_t vlcwrapper::getTime(){
  return (int64_t) libvlc_media_player_get_time(vlcwrapper::mediaplayer)/1000;
}

/*##############################################################################*/
