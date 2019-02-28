#include "LogValidator.h"
#include <algorithm>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
using namespace std;

namespace DmsLog{

    LogValidator::LogValidator (const std::string& fname): infile{fname}, fname{fname} {};
    LogValidator::LogValidator (const std::string& fname, const int n): infile{fname}, fname{fname},nSources{n} {};

    LogValidator::~LogValidator (){ if (this->infile.is_open ()) this->infile.close (); }

    const string& LogValidator::getFileName()  {
        return fname;
    }

    void LogValidator::setEmptyList(std::vector<int> list) { swap (this->emptyList, list); }

    void LogValidator::setVaryingPayloadList(std::vector<int> list) { swap (this->varyingPayloadList, list); }

    void LogValidator::ignoreHeader() {
        std::string line; 
        for (int i = 0; i < 3; ++ i) {
            std::getline (infile, line);
        }
    }

    bool LogValidator::processLogs() {
        std::unordered_set<int> sources;
        for (int i = 0; i < nSources; ++ i) sources.insert (i); 

        std::unordered_set<int> emptyIdSet;
        for (const auto& id : this->emptyList) emptyIdSet.insert (id); 

        std::unordered_map<int, vector<vector<int>> > varyingPayloadMap;
        vector<vector<int>> dummy;
        for (const auto& id : this->varyingPayloadList) varyingPayloadMap[id] = dummy; 

        std::string line; 
        while (std::getline (infile, line)) {
            std::istringstream iss (line);
            int time{}, dt{},  src{}, cyc{}, id{};
            string type{};
            iss>>time>>dt>> type;
            iss>>std::hex>> src>> cyc>> id;
            // if a source is found, remove the source. Once the sources set is empty, we confirm all sources exist in the log
            if (sources.size() && sources.count (src))  sources.erase (src); 

            vector<int> payloadBytes;
            int _byte{};
            bool non_empty = false;
            while (iss>>_byte) {
                // empty check, if there is at least one byte in the payload that is not zero, this ID passes emtpy check.
                if (_byte) non_empty=true;
                if (non_empty && emptyIdSet.count (id)) {
                    emptyIdSet.erase (id); 
                    std::cout<<"ID 0x" <<std::hex<<id<<" Not Empty: " << line <<endl;
                }
                payloadBytes.push_back (_byte);
            }
            if (varyingPayloadMap.count (id) && non_empty) {
                if (varyingPayloadMap[id].size() == 0 ) varyingPayloadMap[id].push_back (payloadBytes);
                else {
                    if (payloadBytes.size() != varyingPayloadMap[id][0].size() ) {
                        printMsg ("Payload changed, sample one: ", id, varyingPayloadMap[id][0]);
                        printMsg ("Payload changed, sample two: ", id, payloadBytes);
                        varyingPayloadMap.erase (id);
                    }
                    else {
                        for (int i = 0; i < payloadBytes.size(); ++ i) {
                            if (payloadBytes[i] != varyingPayloadMap[id][0][i]) {
                                printMsg ("Payload one: ", id, varyingPayloadMap[id][0]);
                                printMsg ("Payload two: ", id, payloadBytes);
                                varyingPayloadMap.erase (id);
                                break;
                            }
                        }
                    }
                }
            }
        }
        return (sources.size () == 0 || emptyIdSet.size() == 0 && varyingPayloadMap.size() == 0);
    }

    void printMsg (const string& prefix, int id,  const vector<int>& vec) {
        std::cout <<"ID " << std::hex <<id<< " " <<prefix  << " Payload: ";
        for (auto& b : vec) {
            std::cout<<" "<<std::hex<<std::setfill('0') << std::setw(2)<<b;
        }
        std::cout<<std::endl;
    }
}
