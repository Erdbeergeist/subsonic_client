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
    //mediaLib.scanRemoteLibrary(&sAPI);

  //####################################VLC#####################################
  memoryMediaObject* song_obj = new memoryMediaObject();
  vlcwrapper vlc;
  //vlc.setmedia(song_obj);
  //############################################################################

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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
  while (!glfwWindowShouldClose(window)){
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Setup Main Window
    static float f = 0.0f;
    static int counter = 0;
    static int artists_current_idx = 0;
    static int albums_current_idx = 0;
    static int songs_current_idx = 0;
    bool playing = false;

    //bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Begin("Library");
    ListBoxWrapper<artist>("Artist", &artists_current_idx, mediaLib.artists);
    ListBoxWrapper<album>("Album", &albums_current_idx, mediaLib.artists[artists_current_idx].albums);
    ListBoxWrapper<song>("Song", &songs_current_idx, mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs);

    if (ImGui::Button("Play")){
      std::string track_id = mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].metadata["id"];

      if (!mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].isInitialized){
        sAPI.download(track_id, &mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data);
        mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].isInitialized = true;
        std::cout<<"Downloaded"<<std::endl;
      }
      vlc.setmedia(&mediaLib.artists[artists_current_idx].albums[albums_current_idx].songs[songs_current_idx].data);
      vlc.play();
      playing = true;
  //    sleep(400);
    }
    if (ImGui::Button("Stop"))
      vlc.stop();

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
