#ifndef RETRIVEMODE_H
#define RETRIVEMODE_H

#include <cstdint>


namespace RetriveRange {
    enum Bit: std::int32_t
    {
        // 100%
        Hard   = 0x0 << 2,
        // 60%
        Normal = 0x1 << 2,
        // 30%
        Easy   = 0x2 << 2,

        Filter = 0x3 << 2,
    };
}

namespace RetriveLevel {
    enum Bit: std::int32_t
    {
        DefaultMode = 0x0,
        EasyMode    = 0x1,
        NormalMode  = 0x2,
        HardMode    = 0x3,

        Filter = 0x3,
    };
}

enum RetriveMode: std::int32_t
{
    Easy_DefaultMode = RetriveRange::Easy | RetriveLevel::DefaultMode,
    Easy_EasyMode    = RetriveRange::Easy | RetriveLevel::EasyMode,
    Easy_NormalMode  = RetriveRange::Easy | RetriveLevel::NormalMode,
    Easy_HardMode    = RetriveRange::Easy | RetriveLevel::HardMode,
    Easy             = Easy_DefaultMode,


    Normal_DefaultMode = RetriveRange::Normal | RetriveLevel::DefaultMode,
    Normal_EasyMode    = RetriveRange::Normal | RetriveLevel::EasyMode,
    Normal_NormalMode  = RetriveRange::Normal | RetriveLevel::NormalMode,
    Normal_HardMode    = RetriveRange::Normal | RetriveLevel::HardMode,
    Normal             = Normal_DefaultMode,


    Hard_DefaultMode = RetriveRange::Hard | RetriveLevel::DefaultMode,
    Hard_EasyMode    = RetriveRange::Hard | RetriveLevel::EasyMode,
    Hard_NormalMode  = RetriveRange::Hard | RetriveLevel::NormalMode,
    Hard_HardMode    = RetriveRange::Hard | RetriveLevel::HardMode,
    Hard             = Hard_DefaultMode,

    DefaultMode = Hard_DefaultMode,
    EasyMode    = Hard_EasyMode,
    NormalMode  = Hard_NormalMode,
    HardMode    = Hard_HardMode,
    Default     = DefaultMode
};


namespace RetriveRange {
    RetriveMode get_mode(RetriveMode mode);
}

namespace RetriveLevel {
    RetriveMode get_mode(RetriveMode mode);
    bool is_default(RetriveMode mode);
}

#endif // RETRIVEMODE_H
