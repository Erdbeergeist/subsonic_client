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
    subsonicAPI sAPI();

    mediaLibrary mediaLib;
    mediaLib.scanLocalLibrary();
    //mediaLib.scanRemoteLibrary(&sAPI);

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
    static int item_current_idx = 0;

    //bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Library");
    ListBoxWrapper<artist>("Artists", &item_current_idx, mediaLib.artists);
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

    std::string track_id;
    std::cout<<"Choose Track:\n";
    std::cin>>track_id;
  //  sAPI.getMusicDirectory("artist-95533");
   //sAPI.getMusicDirectory("album-47767");//track-38004
    //sAPI.Print();

    //sAPI.getSong(track_id);//track-38004
    //sAPI.Print();
    //sAPI.getMusicDirectory("artist-95533");
  //  sAPI.Print();


    memoryMediaObject* song = new memoryMediaObject();
    std::cout<<"Downloading..."<<std::endl;
    sAPI.download(track_id, song);
  // std::ofstream download;
//    download.open("RiverFlowsInYou.flac", std::ios::out | std::ios::binary);
  //  download.write(&song->data.buffer, song->data.size);
    //char* buffer;
    //char *data = song.buffer;
    //memcpy(buffer, data, 1);

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    inst = libvlc_new (0, NULL);
    m = libvlc_media_new_callbacks(inst,
                                   vlc_open_callback,
                                   vlc_read_callback,
                                   vlc_seek_callback,
                                   NULL,
                                   &song->data);
    mp = libvlc_media_player_new_from_media (m);

    std::cout<<"Playing..."<<std::endl;
    libvlc_media_player_play(mp);
    sleep(400);

    libvlc_media_release(m);
    libvlc_media_player_release(mp);
    libvlc_release(inst);
}
