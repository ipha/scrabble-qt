#ifndef HASHQUICK_H
#define HASHQUICK_H

#include <cstdint>
#include <cstdio>

static unsigned int hash_lut[] = {
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

        0, 1<< 1, 1<< 2, 1<< 3, 1<< 4, 1<< 5, 1<< 6, 1<< 7, 1<< 8, 1<< 9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15,
    1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<20, 1<<21, 1<<22, 1<<23, 1<<24, 1<<25,     0,     0,     0,     0,     0,

        0, 1<< 1, 1<< 2, 1<< 3, 1<< 4, 1<< 5, 1<< 6, 1<< 7, 1<< 8, 1<< 9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15,
    1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<20, 1<<21, 1<<22, 1<<23, 1<<24, 1<<25,     0,     0,     0,     0,     0,

        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0
};

class HashQuick {
    bool set[1<<26];
public:
    // Add to hash set
    void add(const char* str) {
        set[strhash(str)] = true;
    }
    // Check hash set
    bool contains(const char* str) {
        return set[strhash(str)];
    }
    bool contains(const char* str1, const char* str2) {
        return set[strhash(str1) | strhash(str2)];
    }
    bool contains(const char* str1, const char* str2, bool has_wild) {
        if(has_wild) {
            for(char c = 'a'; c <= 'z'; c++) {
                if(set[strhash(str1) | strhash(str2) | hash_lut[(int)c]])
                    return true;
            }
            return false;
        } else {
            return set[strhash(str1) | strhash(str2)];
        }
    }

private:
    // Inline maybe?
    inline uint32_t strhash(const char* str) {
        uint32_t hash = 0;
        while(*str)
            hash |= hash_lut[(int)*(str++)];
        return hash;
    }
};

#endif // HASHQUICK_H
