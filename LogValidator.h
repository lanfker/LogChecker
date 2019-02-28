#include <fstream>
#include <string>
#include <vector>

#ifndef LOG_VALIDATOR
#define LOG_VALIDATOR

using namespace std;
namespace DmsLog {

    class LogValidator {
        private: 
            std::ifstream infile;
            std::string fname;
            int nSources{4};
            std::vector<int> emptyList{};
            std::vector<int> varyingPayloadList{};
            void ignoreHeader();

        public: 
            LogValidator (const std::string& fname);
            LogValidator (const std::string& fname, const int nSources);
            ~LogValidator ();
            void setEmptyList(std::vector<int> list);
            void setVaryingPayloadList(std::vector<int> list);
            bool processLogs();
            const string& getFileName ();

    };

    void printMsg (const string& prefix, int id,  const vector<int>& vec);
}
#endif
