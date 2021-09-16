BIN    = subsonic-client
OBJ    = subsonic-client.o core.o subsonicAPI.o mediaObjects.o curlwrapper.o vlcwrapper.o tinyxml2.o
HEADER = subsonicAPI.h subsonic-client.h mediaObjects.h curlwrapper.h vlcwrapper.h tinyxml2.h

# Compiler options
CXX = g++-7
CXXFLAGS = -std=c++0x -Wall -g -O3 -march=native -fopenmp -I/home/lukas/Programs/vlc/include
LIBFLAGS = -L/usr/local/lib -lrt -lpthread -lcurl -lvlc
INCFLAGS = -I/home/lukas/anaconda3/include -I/home/lukas/Programs/vlc/include

# Compilation rules
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(OBJ) $(LIBFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -f $(BIN) subsonic-client.o
