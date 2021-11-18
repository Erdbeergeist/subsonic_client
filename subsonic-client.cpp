#include "subsonic-client.h"
#include "mediaObjects.h"
#include "curlwrapper.h"
#include "subsonicAPI.h"
#include "vlcwrapper.h"
#include "imguiwrapper.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char *argv[]) {

  //Start Subsonic API and read local library
  subsonicAPI sAPI("https://192.168.1.155/index.php/apps/music/subsonic/rest/", "luxmit", "3x19lyq7e10i", "subsonic-dev");

  mediaLibrary mediaLib;
  mediaLib.scanLocalLibrary();
  //  mediaLib.scanRemoteLibrary(&sAPI);
  //sAPI.getCoverArt(mediaLib.artists[0].albums[0].metadata["id"],&mediaLib.artists[0].albums[0].coverImage);
  //exit(1);
  //####################################VLC#####################################
  vlcwrapper vlc;

  //############################################################################
  //Register subsonicAPI, mediaLibrary and vlcwrapper with the mediaPlayer class
  mediaPlayer player(&sAPI, &vlc, &mediaLib);

  //#################################Main Window################################
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
      return 1;

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1280, 1600, "Test", NULL, NULL);
  if (window == NULL)
      return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();


  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  bool playing = false;
  static int artists_current_idx = 0;
  static int albums_current_idx = 0;
  static int songs_current_idx = 0;
  bool close_button = true;
  float playing_position = 0;

while (!glfwWindowShouldClose(window)){
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  //Setup Main Window
  static float f = 0.0f;
  static int counter = 0;

  bool show_demo_window = true;
  //ImGui::ShowDemoWindow(&show_demo_window);
  ImGui::Begin("Library", &close_button,
                                        (ImGuiWindowFlags_AlwaysAutoResize));
  ListBoxWrapper<artist>("###Artist", &artists_current_idx, mediaLib.artists);
  ImGui::SameLine(0,25);
  ListBoxWrapper<album>("###Album", &albums_current_idx, mediaLib.artists[artists_current_idx].albums);
  ImGui::SameLine(0,25);
  ListBoxWrapper<song>("###Song", &songs_current_idx, mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs);
  ListBoxWrapper<song>("Playlist", &player.currentSongPlabackQueueIdx, player.playbackQueue);

  ImGui::SliderInt("###PlayingPos", &player.state.songPosition, 0,  player.state.songDuration, "%i", ImGuiSliderFlags_NoInput);

  if (ImGui::Button("Play")){
    player.requestPlayback(artists_current_idx, albums_current_idx, songs_current_idx);
  }
  ImGui::SameLine(0,25);
  if (ImGui::Button("Stop")){
    vlc.stop();
  }
  ImGui::SameLine(0,25);
  if (ImGui::Button("Pause")){
    vlc.pause();
  }

  player.ping();
    //std::cout<<mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data.buffer.size<<"\t"
      //       <<mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data.buffer.last_read_byte_index<<"\t"
        //     <<(float)(mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data.buffer.last_read_byte_index/mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data.buffer.size)<<std::endl;
    //std::cout<<"main"<<std::endl;
  if (!close_button) exit(1); //FIXME ADD CLEANUP

  ImGui::End();

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
  }


//  sAPI.getMusicDirectory("artist-95533");
 //sAPI.getMusicDirectory("album-47767");//track-38004
  //sAPI.Print();
    //sAPI.getSong(track_id);//track-38004
    //sAPI.Print();
    //sAPI.getMusicDirectory("artist-95533");
  //  sAPI.Print();



  // std::ofstream download;
//    download.open("RiverFlowsInYou.flac", std::ios::out | std::ios::binary);
  //  download.write(&song->data.buffer, song->data.size);
    //char* buffer;
    //char *data = song.buffer;
    //memcpy(buffer, data, 1);



}
