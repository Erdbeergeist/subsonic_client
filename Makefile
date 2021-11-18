IMGUI_DIR = /home/lukas/Programs/imgui/

BIN    = subsonic-client
OBJ    = subsonic-client.o subsonicAPI.o mediaObjects.o curlwrapper.o vlcwrapper.o tinyxml2.o imguiwrapper.o $(IMGUI_DIR)/imgui.o $(IMGUI_DIR)/imgui_demo.o $(IMGUI_DIR)/imgui_draw.o $(IMGUI_DIR)/imgui_tables.o $(IMGUI_DIR)/imgui_widgets.o $(IMGUI_DIR)/backends/imgui_impl_glfw.o $(IMGUI_DIR)/backends/imgui_impl_opengl3.o
#HEADER = subsonicAPI.h subsonic-client.h mediaObjects.h curlwrapper.h vlcwrapper.h tinyxml2.h imguiwrapper.h $(IMGUI_DIR)/imgui_impl_glfw.h $(IMGUI_DIR)/imgui_impl_opengl3.h $(IMGUI_DIR)/imgui.h stb_image.h

# Compiler options
CXX = g++-7

GLFWLIBS = `pkg-config --static --libs glfw3`
CXXFLAGS = -std=c++0x -Wall -g -O3 -march=native -fopenmp -I/home/lukas/Programs/vlc/include -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
LIBFLAGS = -L/usr/local/lib -lrt -lpthread -lcurl -lvlc -lglfw -lGL $(GLFWLIBS)
INCFLAGS = -I/home/lukas/anaconda3/include -I/home/lukas/Programs/vlc/include -I/home/lukas/subsonic-client/imgui

# Compilation rules

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(OBJ) $(LIBFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -f $(BIN) subsonic-client.o subsonic-client.o core.o subsonicAPI.o mediaObjects.o curlwrapper.o vlcwrapper.o tinyxml2.o stb_image.o
