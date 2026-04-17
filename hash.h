#ifndef HASH_H
#define HASH_H

#include <stdint.h>

// Returns a hash given key and length of key
uint32_t jenkins_one_at_a_time_hash(const char *key, int length);

#endif