#ifndef USERSTATUSTYPE_H
#define USERSTATUSTYPE_H

#include <cstdint>

enum UserStatus: std::uint8_t
{
    None      = 0x0 | 0x0,
    Author    = 0x1 | 0x0,
    Consumer  = 0x0 | 0x2,
    Both      = 0x1 | 0x2
};

namespace user_status_util {
    bool has_author_status(UserStatus status);
    bool has_consumer_status(UserStatus status);

    void remove_author_status(UserStatus &status);
    void remove_consumer_status(UserStatus &status);

    void add_author_status(UserStatus &status);
    void add_consumer_status(UserStatus &status);
}

#endif // USERSTATUSTYPE_H
