BIN    = subsonic-client
OBJ    = subsonic-client.o core.o tinyxml2.o
HEADER = core.h tinyxml2.h

# Compiler options
CXX = g++-7
CXXFLAGS = -std=c++0x -Wall -g -O3 -march=native -fopenmp
LIBFLAGS = -L/usr/local/lib -lrt -lpthread -lcurl
INCFLAGS = -I/home/lukas/anaconda3/include

# Compilation rules
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(OBJ) $(LIBFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -f $(BIN) subsonic-client.o
