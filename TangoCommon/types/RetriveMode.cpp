#include "RetriveMode.h"

RetriveMode RetriveLevel::get_mode(RetriveMode mode)
{
    return RetriveMode(mode & Bit::Filter);
}

bool RetriveLevel::is_default(RetriveMode mode)
{
    return (mode & Bit::DefaultMode) != 0;
}

RetriveMode RetriveRange::get_mode(RetriveMode mode)
{
    return RetriveMode(mode & Bit::Filter);
}
