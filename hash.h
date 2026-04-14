#ifndef HASH_H
#define HASH_H

#include <stdint.h>

/*

*/
uint32_t jenkins_one_at_a_time_hash(const char *key, int length);

#endif