#include <fstream>
#include <iostream>
#include <vector>
#include "string"

#include "driver.hpp"
#include "lexer.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char* argv[])
{
  try
  {
    if(argc != 2)
    {  
      std::cout << "error: " << std::endl;
      switch(argc < 2)
      {
        case true:  std::cout << "no input files" << std::endl;
                    break;  
        case false: std::cout << "too many arguments: ";
                    for(int n = 1; n < argc; ++n)
                      std::cout << argv[n] << " ";
                    std::cout << std::endl;
                    break; 
      }
      return 1; 
    }

    std::string fileName(argv[1]);
    std::ifstream InputFile(fileName);
    if (!InputFile)
    {
      std::cerr << "error: " << std::endl;
      std::cerr << "cannot open " << fileName << std::endl;
      return 1;
    }

    yy::Driver driver{};
    driver.set_input_stream(InputFile);
    driver.parse();
    if(driver.is_executable())
      driver.execute();
    else
    {
      std::cerr << "syntax analysis completed with errors" << std::endl;
      std::cerr << "program execution terminated" << std::endl;
    }
  }
  catch(std::exception& exptn)
  {
    std::cerr << exptn.what() << std::endl;
    return 1;
  }
  catch(...)
  {
   std::cerr << "undefined error" << std::endl;
   return 1;
  }
}