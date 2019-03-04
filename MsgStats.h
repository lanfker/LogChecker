#pragma once

#include <vector>
namespace DmsLog {
    struct MsgStats {
        int messageId; //CAN ID
        bool nonEmpty; // If payload contains non zero byte, true
        bool varying;  // if two message have one byte different or they have different lengths, true
        long long firstOccur; //the timestamp of the first message of this ID
        long long lastOccur; //the timestamp of the last message of this ID
        long long cnt; // the number of messages of this ID
        std::vector<std::vector<int> > samples; // Samples of payload that are changing and non-empty
        MsgStats(): nonEmpty{false}, varying{false}, firstOccur{-1}, lastOccur{-1}, cnt{0} {}
    };
}
