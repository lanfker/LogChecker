#include <iostream>
#include <sstream>
#include <string>
#include "LogValidator.h"
#ifdef __linux__
#include "DirectoryReader.h"
#endif

using namespace std;

/**
 * The first parameter should be the **prefix** of the file names we plan to match
 * The subsequent parameters are meant to provide simple checks for CAN IDs. There 
 * are two types of checks:
 * * check for non-zero byte payload
 * * check for payload change
 */
int main(int argc, char *argv[])
{
    /*
    if (argc<2) {
        cout<<"At least provide a file name match pattern to start with\n"<<endl;
    }
    */
    string pattern ("L0227_look_");
    DmsLog::DirectoryReader reader(pattern);
    #ifdef __linux__
    vector<string> fnames = reader.collectFileNames();
    #endif
    for (const auto &fname : fnames)
    {

        DmsLog::LogValidator logvalidator(fname);
        logvalidator.setEmptyList({0x451, 0x712});
        logvalidator.setVaryingPayloadList({0x451, 0x712, 0x457});
        if (logvalidator.processLogs()) {
            std::cout << std::endl
                      << "--------------------RESULT-----------------" << std::endl;
            std::cout << logvalidator.getFileName() << " is VALID" << std::endl
                      << std::endl;
        }
        else {
            std::cout << std::endl
                      << "--------------------RESULT-----------------" << std::endl;
            std::cout << logvalidator.getFileName() << " is INVALID" << std::endl
                      << std::endl;
        }
    }

    return 0;
}
