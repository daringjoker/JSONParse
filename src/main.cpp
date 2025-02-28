#include <fstream>
#include <iostream>
#include "JSON/Json.h"
#include <optional>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

  std::ifstream file(argv[1]);
  if(!file.is_open())
  {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
  }

  std::string fullBuffer;
  std::string line;
  while(std::getline(file, line))
  {
    fullBuffer += line;
  }
    std::cout<<argv[1]<<std::endl;
    std::cout<<"+---------------------------------+"<<std::endl;
  std::optional<JSONValue>value = JSON::Parse(fullBuffer.c_str());
  if(value.has_value())
  {
    std::cout << value.value() << std::endl;
    std::cout<<"+---------------------------------+"<<std::endl;
  }
  else
  {
    std::cerr << "Error: Could not parse JSON" << std::endl;
    return 1;
  }
  return 0;
}
