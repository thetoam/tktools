#include <string>
#include <vector>


class Options {
  public:
    void add(::std::string name, char type, void *var);  // Generic variable
    void process(int offset, int argc, char *argv[]);  // Parse the options in the arguments
  private:
    class OptionEntry {
      public:
        ::std::string name;
        char type;
        void *ptr;

        OptionEntry(::std::string n, char t, void *v);
    };
    ::std::vector<OptionEntry> opts;
};
