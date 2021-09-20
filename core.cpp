#include "subsonic-client.h"

// -------------------------------------------------------------------------
// The main command line parser
// -------------------------------------------------------------------------
error_t parse_opt(int key, char *arg, struct argp_state *state){
  switch (key)
  {
    case 'x':
    {
      return strtod(arg,NULL);
      break;
    }
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
