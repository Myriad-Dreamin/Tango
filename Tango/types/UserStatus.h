#ifndef USERSTATUSTYPE_H
#define USERSTATUSTYPE_H


enum UserStatus: unsigned char
{
    None      = 0x0 | 0x0,
    Author    = 0x1 | 0x0,
    Consumer  = 0x0 | 0x2,
    Both      = 0x1 | 0x2
};

namespace user_status {
    bool has_author_status(UserStatus status);
    bool has_consumer_status(UserStatus status);
}

#endif // USERSTATUSTYPE_H
