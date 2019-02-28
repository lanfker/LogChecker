#include <dirent.h> 
#include <string> 
#include <vector> 

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H
using namespace std;


namespace DmsLog{
    class DirectoryReader{
        private:
            const string namePattern;

        public: 
            DirectoryReader(const string& pattern);
            vector<string> collectFileNames () const;

    };


}
#endif