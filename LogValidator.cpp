#ifdef WIN32
#include "pch.h"
#endif
#include "LogValidator.h"
#include "MsgStats.h"
#include <algorithm>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
using namespace std;

namespace DmsLog {

    LogValidator::LogValidator(const std::string& fname) : infile{ fname }, fname{ fname } {};
    LogValidator::LogValidator(const std::string& fname, const int n) : infile{ fname }, fname{ fname }, nSources{ n } {};

    LogValidator::~LogValidator() { if (this->infile.is_open()) this->infile.close(); }

    const string& LogValidator::getFileName() const {
        return fname;
    }

    void LogValidator::setIdList(std::vector<int> list) { swap(this->idList, list); }

    //void LogValidator::setVaryingPayloadList(std::vector<int> list) { swap(this->varyingPayloadList, list); }

    void LogValidator::ignoreHeader() {
        std::string line;
        for (int i = 0; i < 3; ++i) {
            std::getline(infile, line);
        }
    }

    bool LogValidator::processLogs() {
        std::unordered_set<int> sources;
        for (int i = 0; i < nSources; ++i) sources.insert(i);

        unordered_map<int, MsgStats> statsMap; 
        for (const auto& id : idList) {
            MsgStats stats; 
            stats.messageId = id;
            stats.firstOccur = stats.lastOccur = -1;
            statsMap[id] = stats;
        }

        std::string line;
        long long lastTimeStamp = {};
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            int dt{}, src{}, cyc{}, id{};
            string type{};
            iss >> lastTimeStamp >> dt >> type;
            iss >> std::hex >> src >> cyc >> id;
            
            // if a source is found, remove the source. Once the sources set is empty, we confirm all sources exist in the log
            if (sources.size() && sources.count(src))  sources.erase(src);

            if ( !statsMap.count(id)) continue; //If we are not intersted in this ID, we skip checking it.
            if (statsMap[id].firstOccur == -1) statsMap[id].firstOccur = lastTimeStamp;
            statsMap[id].lastOccur = lastTimeStamp;
            statsMap[id].cnt ++;


            vector<int> payloadBytes;
            int _byte{};
            bool non_empty = false;
            while (iss >> _byte) {
                // empty check, if there is at least one byte in the payload that is not zero, this ID passes emtpy check.
                if (_byte) non_empty = true;
                if (non_empty ) {
                    statsMap[id].nonEmpty = true;
                }
                payloadBytes.push_back(_byte);
            }
            if (non_empty) {
                if (statsMap[id].samples.size() == 0) statsMap[id].samples.push_back(payloadBytes);
                else if (statsMap[id].samples.size() == 1){
                    if (payloadBytes.size() != statsMap[id].samples[0].size()) {
                        statsMap[id].varying = true;
                        statsMap[id].samples.push_back (payloadBytes);
                    }
                    else {
                        for (unsigned int i = 0; i < payloadBytes.size(); ++i) {
                            if (payloadBytes[i] != statsMap[id].samples[0][i]) {
                                statsMap[id].varying = true;
                                statsMap[id].samples.push_back (payloadBytes);
                                break;
                            }
                        }
                    }
                }
            }
            //if (sources.size() == 0 && emptyIdSet.size() == 0 && varyingPayloadMap.size() == 0) return true;
        }
        return resCheck (statsMap, sources, lastTimeStamp);
    }

    bool LogValidator::resCheck(const unordered_map<int, MsgStats> &statsMap, const unordered_set<int>& sources, const long long ts) const {
        bool ans = true;
        if (sources.size() != 0) {
            ans = false;
            cout<<" The missing sources are: ";
            for (auto& s : sources) { cout<<s<<" "; }
            cout<<endl;
        }
        for (const auto& rec : statsMap) {

            cout<<"ID 0x"<< std::hex << rec.second.messageId << " is "<< (rec.second.nonEmpty? "non-empty" : "always EMPTY");
            if (rec.second.nonEmpty == false) {
                ans = false;
            }
            cout<<(rec.second.varying? " varying":" always the SAME");
            if (rec.second.varying == false) {
                ans = false;
            }
            if (rec.second.firstOccur == -1 || rec.second.lastOccur == -1 ) {
                ans = false;
                cout<< " UNAVAILABLE";
            }
            else {
                cout<< " cnt: "<<std::dec << rec.second.cnt <<" timespan: " << static_cast<double>(rec.second.lastOccur - rec.second.firstOccur + 1) * 100.0/ts<<"%";
            }
            cout<<endl;
        }
        return ans;
    }
}
