#include <string>
#include <vector>

class Options {

  public:
    void add(::std::string name, char type, bool *var);  // Flag or boolean
    void process(int offset, int argc, char *argv[]);  // Parse the options in the arguments

  private:
    ::std::vector< ::std::string> names;
    ::std::vector<char> types;
    ::std::vector<void*> vars;

};


