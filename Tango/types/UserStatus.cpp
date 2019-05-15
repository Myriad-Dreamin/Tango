#include "UserStatus.h"

namespace user_status {
    bool has_author_status(UserStatus status)
    {
        return status & 0x1;
    }

    bool has_consumer_status(UserStatus status)
    {
        return status & 0x2;
    }
}
