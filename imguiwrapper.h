#ifndef IMGUIWRAPPER_h
#define IMGUIWRAPPER_h

#include "subsonic-client.h"
#include "mediaObjects.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

template<class T> static bool vector_getter(void* vec, int idx, const char** out_text){
    auto& vector = *static_cast<std::vector<T>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).metadata["name"].c_str();
    return true;
};

template<class T> static bool vector_getter_p(void* vec, int idx, const char** out_text){
    auto& vector = *static_cast<std::vector<T*>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx)->metadata["name"].c_str();
    return true;
};

template<class T> bool ListBoxWrapper(const char* label, int* currIndex, std::vector<T> &values){
    if (values.empty()) { return false; }
    return ImGui::ListBox(label, currIndex, vector_getter<T>,
        static_cast<void*>(&values), values.size());
}

template<class T> bool ListBoxWrapper(const char* label, int* currIndex, std::vector<T*> &values){
    if (values.empty()) { return false; }
    return ImGui::ListBox(label, currIndex, vector_getter_p<T>,
        static_cast<void *>(&values), values.size());
}

static void glfw_error_callback(int error, const char* description);

class imguiwrapper{
  public:
    imguiwrapper();
    ~imguiwrapper();

    void display_window();

  private:
    GLFWwindow* window;
    const char* glsl_version = "#version 130";

};

#endif
