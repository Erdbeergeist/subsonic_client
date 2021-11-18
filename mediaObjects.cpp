#include "mediaObjects.h"
#include "subsonicAPI.h"
#include "vlcwrapper.h"

/*##############################memoryMediaObject###############################*/
memoryMediaObject::memoryMediaObject(){
}

memoryMediaObject::~memoryMediaObject(){
}
/*##############################################################################*/

/*##################################artist######################################*/
artist::artist(){
}

artist::artist(std::string name, std::string id, std::string album_count){
  artist::fillmetadata(name, id, album_count);
}

void artist::fillmetadata(std::string name, std::string id, std::string album_count){
  artist::metadata["name"] = name;
  artist::metadata["id"] = id;
  artist::metadata["album_count"] = album_count;
}

artist::~artist(){
}
/*##############################################################################*/

/*##################################album#######################################*/
album::album(){
}

album::album(std::string album_name, std::string album_id, std::string album_artist){
  album::fillmetadata(album_name, album_id, album_artist);
}

album::album(std::string album_name, std::string album_id, std::string album_artist, artist *pArtist){
  album::fillmetadata(album_name, album_id, album_artist);
  album::pArtist = pArtist;
}

void album::fillmetadata(std::string album_name, std::string album_id, std::string album_artist){
  album::metadata["name"] = album_name;
  album::metadata["id"] = album_id;
  album::metadata["artist"] = album_artist;
}

album::~album(){
}
/*##############################################################################*/

/*##################################song########################################*/
song::song(){
}

song::song(std::string song_title, std::string song_id, std::string song_size, std::string song_duration){
  song::fillmetadata(song_title, song_id, song_size, song_duration);
  song::byteSize = &data.buffer.size;
}

song::song(std::string song_title, std::string song_id, std::string song_size, std::string song_duration, artist *pArtist, album *pAlbum){
  song::fillmetadata(song_title, song_id, song_size, song_duration);
  song::byteSize = &data.buffer.size;
  song::pArtist = pArtist;
  song::pAlbum = pAlbum;
}

void song::fillmetadata(std::string song_name, std::string song_id, std::string song_size, std::string song_duration){
  song::metadata["name"] = song_name;
  song::metadata["id"] = song_id;
  song::metadata["size"] = song_size;
  song::metadata["duration"] = song_duration;
}

song::~song(){
}
/*##############################################################################*/

/*#################################mediaLibrary#################################*/
mediaLibrary::mediaLibrary(){
}

mediaLibrary::~mediaLibrary(){
}

void mediaLibrary::createLibraryXML(){

  tinyxml2::XMLDocument libraryXML;
  tinyxml2::XMLNode * pRoot = libraryXML.NewElement("Library");
  libraryXML.InsertFirstChild(pRoot);

  //Loop over all artists
  for (auto artist_e : mediaLibrary::artists){
    tinyxml2::XMLElement * artist = libraryXML.NewElement("Artist");

    //Set Artist Metadata
    for (auto const& it: artist_e.metadata){
      artist->SetAttribute(it.first.c_str(), it.second.c_str());
    }

    //Set Albums
    for (auto album_e: artist_e.albums){
      tinyxml2::XMLElement * album = libraryXML.NewElement("Album");
      //Set Album Metadata
      for (auto const& it_alb: album_e.metadata){
        album->SetAttribute(it_alb.first.c_str(), it_alb.second.c_str());
      }

      //Set Songs
      for (auto song_e: album_e.songs){
          tinyxml2::XMLElement * song = libraryXML.NewElement("Song");
          //Set Song Metadata
          for(auto const& it_song: song_e.metadata){
            song->SetAttribute(it_song.first.c_str(), it_song.second.c_str());
          }
          album->InsertEndChild(song);

      }
      artist->InsertEndChild(album);
    }
    pRoot->InsertEndChild(artist);

  }
  tinyxml2::XMLError eResult = libraryXML.SaveFile(mediaLibrary::defaultLibXMLFileName.c_str());

}

void mediaLibrary::scanRemoteLibrary(subsonicAPI *sAPI){
  std::cout<<"Full Remote Library Scan ... this might take a while..."<<std::endl;
  std::string name, id, album_count;

  tinyxml2::XMLDocument artists_xml, albums_xml, songs_xml;
  artists_xml.Parse(sAPI->getArtists().c_str());

  for(tinyxml2::XMLElement* node=artists_xml.FirstChildElement()->FirstChildElement()->FirstChildElement();
      node;
      node=node->NextSiblingElement()){
    for (tinyxml2::XMLElement* indexname = node->FirstChildElement(); indexname; indexname=indexname->NextSiblingElement())
      {
      //Fill Artist Info
      name = indexname->FindAttribute("name")->Value();
      id = indexname->FindAttribute("id")->Value();
      album_count = indexname->FindAttribute("albumCount")->Value();
      mediaLibrary::artists.emplace_back(name, id, album_count);

    //  std::cout<<std::setfill(' ')<<std::setw(40)<<"\r"<<name<<"                                         "<<std::flush;

      //Fill Album Info into Artist
      albums_xml.Parse(sAPI->getArtist(id).c_str());
      std::string album_name, album_id, album_artist, song_count, album_duration, album_year, album_created, album_genre, album_coverart;
      for(tinyxml2::XMLElement* node_album=albums_xml.FirstChildElement()->FirstChildElement()->FirstChildElement();
          node_album;
          node_album=node_album->NextSiblingElement())
     {
          album_name = node_album->FindAttribute("name")->Value();
          album_id = node_album->FindAttribute("id")->Value();
          album_artist = name;

          //std::cout<<"Album : "<<mediaLibrary::artists.back().albums.back().metadata["name"]<<std::endl;
          artist *pArtist = &(mediaLibrary::artists.back());
          mediaLibrary::artists.back().albums.emplace_back(album_name, album_id, album_artist, pArtist);
          std::cout<<std::setfill(' ')<<std::setw(40)<<"\r"<<name<<"                                         "
                                                           <<album_name<<"                                         "<<std::flush;

          //song_count = node_album->FindAttribute("songCount")->Value();
          //album_year = node_album->FindAttribute("year")->Value();
          //album_created = node_album->FindAttribute("created")->Value();
          //album_genre = node_album->FindAttribute("genre")->Value();
          //album_coverart = node_album->FindAttribute("coverArt")->Value();
          //album_duration = node_album->FindAttribute("duration")->Value();

          //Fill Song Info into Album
          songs_xml.Parse(sAPI->getAlbum(album_id).c_str());
          std::string song_id, song_artist,
                      song_name, song_album,
                      song_duration, song_album_id,
                      song_size;
          for(tinyxml2::XMLElement* node_song=songs_xml.FirstChildElement()->FirstChildElement()->FirstChildElement();
                          node_song;
                          node_song=node_song->NextSiblingElement()){

            song_id = node_song->FindAttribute("id")->Value();
            song_name = node_song->FindAttribute("title")->Value();
            song_size = node_song->FindAttribute("size")->Value();
            song_duration = node_song->FindAttribute("duration")->Value();
          //  sAPI->Print();
            mediaLibrary::artists.back().albums.back().songs.emplace_back(song_name, song_id, song_size, song_duration);
          }
      }
    }
  }
  std::cout<<"\n Now Saving XML"<<std::endl;
  mediaLibrary::createLibraryXML();
}

void mediaLibrary::scanLocalLibrary(){

  std::string name, id, album_count;

  tinyxml2::XMLDocument library_xml, songs_xml, albums_xml;

  library_xml.LoadFile(mediaLibrary::defaultLibXMLFileName.c_str());

  for(tinyxml2::XMLElement* node=library_xml.FirstChildElement();
      node;
      node=node->NextSiblingElement()){
    for (tinyxml2::XMLElement* indexname = node->FirstChildElement(); indexname; indexname=indexname->NextSiblingElement())
      {
      //Fill Artist Info
      name = indexname->FindAttribute("name")->Value();
      id = indexname->FindAttribute("id")->Value();
      album_count = indexname->FindAttribute("album_count")->Value();
      mediaLibrary::artists.emplace_back(name, id, album_count);

  //  std::cout<<std::setfill(' ')<<std::setw(40)<<"\r"<<name<<"                                         "<<std::flush;
      //Fill Album Info into Artist
      std::string album_name, album_id, album_artist, song_count, album_duration, album_year, album_created, album_genre, album_coverart;
      for(tinyxml2::XMLElement* node_album=indexname->FirstChildElement();
          node_album;
          node_album=node_album->NextSiblingElement())
     {
          album_name = node_album->FindAttribute("name")->Value();
          album_id = node_album->FindAttribute("id")->Value();
          album_artist = name;

          //std::cout<<"Album : "<<mediaLibrary::artists.back().albums.back().metadata["name"]<<std::endl;
          artist *pArtist = &(mediaLibrary::artists.back());
          mediaLibrary::artists.back().albums.emplace_back(album_name, album_id, album_artist, pArtist);
          //std::cout<<std::setfill(' ')<<std::setw(40)<<"\r"<<name<<"                                         "
            //                                               <<album_name<<"                                         "<<std::flush;

          //song_count = node_album->FindAttribute("songCount")->Value();
          //album_year = node_album->FindAttribute("year")->Value();
          //album_created = node_album->FindAttribute("created")->Value();
          //album_genre = node_album->FindAttribute("genre")->Value();
          //album_coverart = node_album->FindAttribute("coverArt")->Value();
          //album_duration = node_album->FindAttribute("duration")->Value();
          //Fill Song Info into Album
          std::string song_id, song_artist,
                      song_title, song_album,
                      song_duration, song_album_id,
                      song_size;
          for(tinyxml2::XMLElement* node_song=node_album->FirstChildElement();
                          node_song;
                          node_song=node_song->NextSiblingElement()){

            song_id = node_song->FindAttribute("id")->Value();
            song_title = node_song->FindAttribute("name")->Value();
            song_size = node_song->FindAttribute("size")->Value();
            song_duration = node_song->FindAttribute("duration")->Value();
            album *pAlbum = &(mediaLibrary::artists.back().albums.back());
            mediaLibrary::artists.back().albums.back().songs.emplace_back(song_title, song_id, song_size, song_duration, pArtist, pAlbum);
          }
      }

    }
  }
}

void mediaLibrary::getArtistNames(const char *artistnames){
}
/*##############################################################################*/

/*#################################mediaPlayer##################################*/
mediaPlayer::mediaPlayer(subsonicAPI *sAPI, vlcwrapper *vlc, mediaLibrary *mediaLib){
  mediaPlayer::sAPI = sAPI;
  mediaPlayer::vlc = vlc;
  mediaPlayer::mediaLib = mediaLib;
}

mediaPlayer::~mediaPlayer(){
//Call Clean Up #FIXME
}

void mediaPlayer::play(){
  if (!mediaPlayer::isPlaying){
    vlc->play();
    mediaPlayer::isPlaying = true;
  }
}

void mediaPlayer::stop(){
  vlc->stop();
  mediaPlayer::isPlaying = false;
  mediaPlayer::isWaitingOnVLC = true;
}

void mediaPlayer::pause(){
  vlc->pause();
  mediaPlayer::isPlaying = false;
}

bool loadTextureFromImage(GLuint* out_texture, int* out_width, int* out_height){
  //use stb_image to load image from memory, then opengl to load decompressed RGBA from RAM to GPU and set the imageTextureOnGPU Flag to indicate that we can display it via dearImgui
  return true;
}

int mediaPlayer::addToPlaybackQueue(int artist_idx, int album_idx, int song_idx){
  song *songToAdd = mediaPlayer::getSongFromIndices(artist_idx, album_idx, song_idx);
  for (auto element : mediaPlayer::playbackQueue){
    if (songToAdd->metadata["id"] == element->metadata["id"]) return -1;
  }
  mediaPlayer::playbackQueue.push_back(songToAdd);
  return 0;
}

int mediaPlayer::addToPlaybackQueue(song *songToAdd){
  for (auto element : mediaPlayer::playbackQueue){
    if (songToAdd->metadata["id"] == element->metadata["id"]) return -1;
  }
  mediaPlayer::playbackQueue.push_back(songToAdd);
  return 0;
}

song* mediaPlayer::getSongFromIndices(int artist_idx, int album_idx, int song_idx){
  //std::cout<<"&mediaLib/Song/: "<<&mediaPlayer::mediaLib->artists[artist_idx].albums[album_idx].songs[song_idx]<<std::endl;
  //std::cout<<"&mediaLib/Song/data/buffer/iscomplete: "<<&mediaPlayer::mediaLib->artists[artist_idx].albums[album_idx].songs[song_idx].data.buffer.isComplete<<std::endl;
  return &mediaPlayer::mediaLib->artists[artist_idx].albums[album_idx].songs[song_idx];
}

void mediaPlayer::beginDownload(song *songToDownload, int index = 0){
  if (!mediaPlayer::isDownloading){
    mediaPlayer::backgroundWorker = std::thread(&subsonicAPI::download, mediaPlayer::sAPI, songToDownload->metadata["id"], &songToDownload->data);
    mediaPlayer::isDownloading = true;
    mediaPlayer::download_idx = index;
  }
}

void mediaPlayer::requestPlayback(int artist_idx, int album_idx, int song_idx){
  int ret = mediaPlayer::addToPlaybackQueue(artist_idx, album_idx, song_idx);
}

int mediaPlayer::downloadNextSongInQueue(){

  //Check for undownloaded songs in queue
  for (int i = 0; i < mediaPlayer::playbackQueue.size(); i++){
    if (!mediaPlayer::playbackQueue[i]->data.buffer.isComplete){
      mediaPlayer::beginDownload(mediaPlayer::playbackQueue[i], i);
      return i;
    }
  }
  return -1;
}

int mediaPlayer::downloadNextCoverInQueue(){
  //Check for undownloaded songs in queue
  for (int i = 0; i < mediaPlayer::playbackQueue.size(); i++){
    if (!mediaPlayer::playbackQueue[i]->pAlbum->coverImage.buffer.isComplete){
      mediaPlayer::downloadCoverArt(mediaPlayer::playbackQueue[i]->pAlbum);
      return i;
    }
  }
  return -1;
}

int mediaPlayer::downloadCoverArt(album *albumToGetCover){
  if (!mediaPlayer::isDownloadingCover){
    mediaPlayer::backgroundWorkerCovers = std::thread(&subsonicAPI::getCoverArt, mediaPlayer::sAPI, albumToGetCover->metadata["id"], &albumToGetCover->coverImage);
    mediaPlayer::isDownloadingCover = true;
    //mediaPlayer::download_idx = -2;
  }
}

void mediaPlayer::ping(){

  if (mediaPlayer::playbackQueue.size()<1) return;

  if (mediaPlayer::isWaitingOnVLC){
    std::cout<<"MEDIA PLAYER PING WAITINGONVLC: "<<vlc->waitingOnEvent<<std::endl;
    mediaPlayer::isWaitingOnVLC = vlc->waitingOnEvent;
    return;
  }

  if (mediaPlayer::isDownloading){
  //  std::cout<<"MEDIA PLAYER PING: DW_NAME||DW_COMPLETE||DW_SIZE: "<<mediaPlayer::playbackQueue[download_idx]->metadata["name"]<<"\t"<<
  //             mediaPlayer::playbackQueue[download_idx]->data.buffer.isComplete<<"\t"<<
  //             (int64_t) (mediaPlayer::playbackQueue[download_idx]->data.buffer.size-mediaPlayer::bufferAdvanceSize)<<"\t"<<
  //             mediaPlayer::playbackQueue[download_idx]->data.buffer.size<<std::endl;
  }
  int downloadNext = 0;

  //Check if current download is finished
  if (mediaPlayer::playbackQueue[mediaPlayer::download_idx]->data.buffer.isComplete){
    mediaPlayer::isDownloading = false;
    if (mediaPlayer::backgroundWorker.joinable()) mediaPlayer::backgroundWorker.join();
    downloadNext = mediaPlayer::downloadNextSongInQueue();
  }

  //Are we currently playing?
  if (!mediaPlayer::isPlaying && (mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.isComplete
                              || mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.size > mediaPlayer::bufferAdvanceSize)){
    mediaPlayer::vlc->setMedia(&mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data);
    mediaPlayer::playing_idx = mediaPlayer::currentSongPlabackQueueIdx;
    mediaPlayer::play();
  }

  //Are we playing, but want another song in the queue
  else if ((mediaPlayer::isPlaying && mediaPlayer::playing_idx != mediaPlayer::currentSongPlabackQueueIdx) && (mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.isComplete
                                   || mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.size > mediaPlayer::bufferAdvanceSize)){
    mediaPlayer::stop();
  }

  //Are we neither playing nor downloading?
  if (!mediaPlayer::isPlaying){
    if (!mediaPlayer::isDownloading){
      downloadNext = mediaPlayer::downloadNextSongInQueue();
    }
  }

  //Has the Time Slider been moved ?
  if (mediaPlayer::isPlaying && (mediaPlayer::state.songPosition != mediaPlayer::songPosition)) {
    mediaPlayer::setTime(mediaPlayer::state.songPosition);
  }

  //Song should have a pointer to album and album a pointer to artist #FIXME
  if (mediaPlayer::enableCoverDownload){
    if (mediaPlayer::backgroundWorkerCovers.joinable()) {
      mediaPlayer::backgroundWorkerCovers.join();
      mediaPlayer::isDownloadingCover = false;
    }
    mediaPlayer::currentDownloadingCover_idx = mediaPlayer::downloadNextCoverInQueue();
  }

  //Update State
  mediaPlayer::state.playing_idx = mediaPlayer::playing_idx;
  mediaPlayer::state.download_idx = mediaPlayer::download_idx;
  mediaPlayer::state.isPlaying = mediaPlayer::isPlaying;
  mediaPlayer::state.isDownloading = mediaPlayer::isDownloading;
  mediaPlayer::state.songPosition = mediaPlayer::vlc->getTime();
  mediaPlayer::songPosition = mediaPlayer::state.songPosition;
  mediaPlayer::state.songDuration = std::atoi(mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->metadata["duration"].c_str());


  //if (mediaPlayer::isPlaying) std::cout<<"MEDIA PLAYER PING: SONGPOSITION||VLC PLAYING: "<<mediaPlayer::state.songPosition<<"\t"<<mediaPlayer::vlc->isPlaying()<<std::endl;
}

int mediaPlayer::setTime(int newTime){
  if (mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.isComplete) mediaPlayer::vlc->setTime(newTime);
  else if (((float)newTime/(float)mediaPlayer::songPosition)*((float)mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.last_read_byte_index) <
            mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.size - 2*mediaPlayer::bufferAdvanceSize){
    mediaPlayer::vlc->setTime(newTime);
    std::cout<<"MEDIA PLAYER SETTIME: CHANGESONGPOSITION||VLC PLAYING: "<<newTime<<"\t"<<mediaPlayer::songPosition<<"\t"<<
              mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.size<<"\t"<<mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.last_read_byte_index
              <<"\t"<<(std::size_t)((float)newTime/(float)mediaPlayer::songPosition)<<"\t"<<((float)newTime/(float)mediaPlayer::songPosition)*((float)mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.last_read_byte_index)<<
              "\t"<<mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data.buffer.size - 2*mediaPlayer::bufferAdvanceSize<<std::endl;

  }
  if (!mediaPlayer::vlc->isPlaying()){
    mediaPlayer::stop();
    mediaPlayer::vlc->setMedia(&mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data);
    mediaPlayer::vlc->setTime(mediaPlayer::songPosition);
    mediaPlayer::play();
  }
  if (mediaPlayer::isPlaying) std::cout<<"MEDIA PLAYER PING: SONGPOSITION||VLC PLAYING: "<<mediaPlayer::vlc->getTime()<<"\t"<<mediaPlayer::vlc->isPlaying()<<std::endl;
  return mediaPlayer::vlc->getTime();
}
/*##############################################################################*/
