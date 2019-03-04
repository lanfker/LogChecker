#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "MsgStats.h"

#ifndef LOG_VALIDATOR
#define LOG_VALIDATOR

using namespace std;
namespace DmsLog {

    class LogValidator {
        private:
            std::ifstream infile;
            std::string fname;
            int nSources{ 4 };

            std::vector<int> idList{};
            //std::vector<int> emptyList{};
            //std::vector<int> varyingPayloadList{};
            void ignoreHeader();
            bool resCheck(const unordered_map<int, MsgStats> &statsMap, const unordered_set<int>& sources, const long long ts) const;

        public:
            LogValidator(const std::string& fname);
            LogValidator(const std::string& fname, const int nSources);
            ~LogValidator();
            void setIdList(std::vector<int> list);
            //void setVaryingPayloadList(std::vector<int> list);
            bool processLogs();
            const string& getFileName();

    };

    void printMsg(const string& prefix, int id, const vector<int>& vec);
}
#endif
