#ifndef SUBSONICCLIENT_H
#define SUBSONICCLIENT_H

#include <stdlib.h>
#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <cmath>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <argp.h>
#include <tuple>
#include <typeinfo>
#include <unistd.h>
#include <map>
#include <sys/types.h>
#include <curl/curl.h>
#include "tinyxml2.h"
#include "vlc/vlc.h"



//Argument Parsing
error_t parse_opt(int key, char *arg, struct argp_state *state);

// The actual list of command line options we accept
static struct argp_option cmdline_options[] = {
  {"Test", 'x', "string", 0,"test"},
  { 0 }
};

#endif
