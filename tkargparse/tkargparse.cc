#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "tkargparse.h"

using namespace std;


void ::Options::add(string name, char type, void *var)
{
  OptionEntry o("-" + name, type, var);
  opts.push_back(o);
}

void ::Options::process(int offset, int argc, char *argv[])
{
  unsigned int i;
  int j;
  string arg;

  for (i = 0; i < opts.size(); i++)
  {
    for (j = offset; j < argc; j++)
    {
      arg = string(argv[j]);
      if (arg[0] != '-')
        continue;  // Skip a line that does not contain an argument
      if (arg.compare(opts[i].name) == 0)
      {
        // Process the argument
        switch(opts[i].type) {
          case 'x': // Simple flag/boolean
            *((bool*) opts[i].ptr) = true;
            break;
          case 'd':
            *((double*) opts[i].ptr) = atof(argv[++j]);
            break;
          case 'i':
            *((int*) opts[i].ptr) = atoi(argv[++j]);
            break;
          case 'l':
            *((int*) opts[i].ptr) = atol(argv[++j]);
            break;
          default:
            break;
        }
      }
      arg = "";
    }
  }
}

Options::OptionEntry::OptionEntry(string n, char t, void *v)
{
  name = n;
  type = t;
  ptr = v;
}
