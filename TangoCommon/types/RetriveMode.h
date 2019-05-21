#ifndef RETRIVEMODE_H
#define RETRIVEMODE_H

#include <cstdint>

enum RetriveMode: std::uint16_t
{
    // 30%
    Easy = 0,
    // 60%
    Normal = 15,
    // 100%
    Hard = 30
};

#endif // RETRIVEMODE_H
