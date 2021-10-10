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
          mediaLibrary::artists.back().albums.emplace_back(album_name, album_id, album_artist);
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
          mediaLibrary::artists.back().albums.emplace_back(album_name, album_id, album_artist);
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

            mediaLibrary::artists.back().albums.back().songs.emplace_back(song_title, song_id, song_size, song_duration);
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
}

void mediaPlayer::pause(){
  vlc->pause();
  mediaPlayer::isPlaying = false;
}

void mediaPlayer::addToPlaybackQueue(int artist_idx, int album_idx, int song_idx){
    mediaPlayer::playbackQueue.push_back(mediaPlayer::getSongFromIndices(artist_idx, album_idx, song_idx));
}

void mediaPlayer::addToPlaybackQueue(song *songToAdd){
  mediaPlayer::playbackQueue.push_back(songToAdd);
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
    mediaPlayer::download_index = index;
  }
}

void mediaPlayer::requestPlayback(int artist_idx, int album_idx, int song_idx){

  mediaPlayer::addToPlaybackQueue(artist_idx, album_idx, song_idx);

  /*if (mediaPlayer::playbackQueue.size() < 1) {
    mediaPlayer::currentSong = mediaPlayer::getSongFromIndices(artist_idx, album_idx, song_idx);

    /*Check if the Song is already is downloaded if yes we playback by
    setting it as the current media in the vlc instance.
    If not we begin the download*/
  /*  if(mediaPlayer::currentSong->isDownloaded){
      mediaPlayer::vlc->setMedia(&mediaPlayer::currentSong->data);
      mediaPlayer::play();
    }

    else{
      mediaPlayer::beginDownload(mediaPlayer::currentSong);
    }
  }*/
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

void mediaPlayer::ping(){
//  mediaPlayer::isPlaying = vlc->isPlaying(); //Maybe this is bad #FIXME
  if (mediaPlayer::playbackQueue.size()<1) return;
  std::cout<<mediaPlayer::playbackQueue[download_index]->metadata["name"]<<"\t"<<
             mediaPlayer::playbackQueue[download_index]->data.buffer.isComplete<<"\t"<<
             mediaPlayer::playbackQueue[download_index]->data.buffer.size<<std::endl;
  //Check if current download is finished
  if (mediaPlayer::playbackQueue[mediaPlayer::download_index]->data.buffer.isComplete){
    mediaPlayer::isDownloading = false;
    if (mediaPlayer::backgroundWorker.joinable()) mediaPlayer::backgroundWorker.join();
    mediaPlayer::downloadNextSongInQueue();
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
    mediaPlayer::vlc->setMedia(&mediaPlayer::playbackQueue[mediaPlayer::currentSongPlabackQueueIdx]->data);
    mediaPlayer::playing_idx = mediaPlayer::currentSongPlabackQueueIdx;
    mediaPlayer::play();
  }


  //Are we neither playing nor downloading?
  if (!mediaPlayer::isPlaying){
    if (!mediaPlayer::isDownloading){
      downloadNextSongInQueue();
    }
  }



  //Check if all songs in Queue have been Downloaded

  /*if(mediaPlayer::isDownloading || mediaPlayer::isPlaying)
    fprintf(stdout, "Playing: %u Downloading: %u Downloaded: %i Size: %i\n",
          mediaPlayer::isDownloading, mediaPlayer::isPlaying,
          (unsigned long int)mediaPlayer::currentSong->data.buffer.size,  std::stoi(currentSong->metadata["size"]));*/
  /*if (mediaPlayer::isPlaying && !mediaPlayer::isDownloading)
    return;
  else if(!mediaPlayer::isPlaying && mediaPlayer::isDownloading &&
         currentSong->data.buffer.size > mediaPlayer::bufferAdvanceSize){
    vlc->setMedia(&mediaPlayer::currentSong->data);
    mediaPlayer::play();
  }*/
}
/*##############################################################################*/
