#include <iostream>
#include <sstream>

class StdIOStreamRedirect
{
  std::streambuf *cinbuf_;
  std::streambuf *coutbuf_;
  std::istringstream in_;
  std::ostringstream out_;
public:
  StdIOStreamRedirect()
    : cinbuf_(std::cin.rdbuf())
    , coutbuf_(std::cout.rdbuf())
  {
    std::cin.rdbuf(in_.rdbuf());
    std::cout.rdbuf(out_.rdbuf());
  }

  ~StdIOStreamRedirect()
  {
    std::cin.rdbuf(cinbuf_);   //reset to standard input again
    std::cout.rdbuf(coutbuf_); //reset to standard output again
  }

  std::string output() { return out_.str(); }

  //template<class T>
  //std::istream & operator >> (const T & value) { return in_ >> value; }
};