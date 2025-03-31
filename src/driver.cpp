#include <fstream>
#include <iostream>
#include <vector>
#include "string"

#include "driver.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char* argv[])
{
    if(argc != 2)
    {  
        std::cout << "error: " << std::endl;
        switch(argc < 2)
        {
            case true:   std::cout << "no input files" << std::endl;
                         break;
            
            case false:  std::cout << "too many arguments: ";
                         for(int n = 1; n < argc; ++n)
                            std::cout << argv[n] << " ";
                         std::cout << std::endl;
                         break; 
        }
        return 0; 
    }

    std::string fileName(argv[1]);
    std::ifstream InputFile(fileName);
    if (!InputFile)
    {
        std::cerr << "error: " << std::endl;
        std::cerr << "cannot open " << fileName << std::endl;
        return 0;
    }

    FlexLexer *lexer = new yyFlexLexer;
    lexer->switch_streams(&InputFile, &std::cout);
    yy::Driver driver{lexer};
    
    driver.parse();
    driver.execute();
    delete lexer;
}