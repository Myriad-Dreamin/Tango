
#include "UserStatus.h"

namespace user_status_util {

    bool has_author_status(UserStatus status)
    {
        return status & 0x1;
    }

    bool has_consumer_status(UserStatus status)
    {
        return status & 0x2;
    }

    void remove_author_status(UserStatus &status)
    {
        status = UserStatus(status & (~0x1));
    }

    void remove_consumer_status(UserStatus &status)
    {
        status = UserStatus(status & (~0x2));
    }


    void add_author_status(UserStatus &status)
    {
        status = UserStatus(status | 0x1);
    }

    void add_consumer_status(UserStatus &status)
    {
        status = UserStatus(status | 0x2);
    }
}
