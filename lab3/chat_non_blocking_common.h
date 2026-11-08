#ifndef CHAT_NON_BLOCKING_COMMON_H
#define CHAT_NON_BLOCKING_COMMON_H

#include <sys/types.h>

#define CHAT_KEY 123456
#define CHAT_CAPACITY 100

struct chat_memory {
    char from_user1[CHAT_CAPACITY];
    char from_user2[CHAT_CAPACITY];
    pid_t pid1;
    pid_t pid2;
};

#endif
