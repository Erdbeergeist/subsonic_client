#include "imguiwrapper.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



//FOR NOW ABANDONED

imguiwrapper::imguiwrapper(){

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
      //Need to set an error flag or smth #FIXME

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  imguiwrapper::window = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
  if (imguiwrapper::window == NULL)
      //Need to set an error flag or smth #FIXME
  glfwMakeContextCurrent(imguiwrapper::window);
  glfwSwapInterval(1); // Enable vsync

}

imguiwrapper::~imguiwrapper(){};

void imguiwrapper::display_window(){

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(imguiwrapper::window, true);
  std::cout<<"test"<<std::endl;
  ImGui_ImplOpenGL3_Init(imguiwrapper::glsl_version);
  while (!glfwWindowShouldClose(imguiwrapper::window)){
    glfwPollEvents();


    bool show_demo_window = true;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(&show_demo_window);


    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(imguiwrapper::window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(imguiwrapper::window);
    }

}
