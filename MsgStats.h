#pragma once

#include <vector>
namespace DmsLog {
    struct MsgStats {
        int messageId;
        bool nonEmpty;
        bool varying; 
        long long firstOccur;
        long long lastOccur;
        long long cnt;
        std::vector<std::vector<int> > samples;
        MsgStats(): nonEmpty{false}, varying{false}, firstOccur{-1}, lastOccur{-1}, cnt{0} {}
    };
}
