#include <iostream>
#include <string>
#include <vector>

#include "tkargparse.h"

using namespace std;

void ::Options::add(string name, char type, bool *var)
{
  names.push_back("-" + name);
  types.push_back(type);
  vars.push_back(var);
}


void ::Options::process(int offset, int argc, char *argv[])
{
  int i;
  vector< ::std::string> args;

  // Build a vector, because I fid it easier to deal with
  for (i = offset; i < argc; i++)
    args.push_back(string(argv[i]));


  // TODO: Loop using numbers (and vector.size() ) instead, so I know which
  //       element of "names" I'm looking at, so I can grab the corresponding
  //       element of "types" and "vars" to play around with.



  // See if each named flag is in there:
  for (vector<string>::iterator nit = names.begin(); nit != names.end(); ++nit)
  {
    for (vector<string>::iterator it = args.begin(); it != args.end(); ++it)
    {
      if ((*it)[0] != '-')
        continue;  // Skip a line that isn't an argument
      if (*it == *nit)
        cerr << "Found one: " << *it << endl;
    }
  }



  for (vector<string>::iterator it = args.begin(); it != args.end(); ++it)
    cerr << *it << endl;


}
