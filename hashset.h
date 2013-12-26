#ifndef HASHSET_H
#define HASHSET_H

#include <cstring>

static unsigned char tolower_lut[] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

// Limited hash set for storing char* strings
// No way to remove keys once added
// probably not memory efficient
// basic iterator support

class HashSet {
	const char** set;
	int buckets;
	int collisions;
	int items;
public:
	// Create set with 256 buckets
	HashSet() {
		buckets = 256;
		set =  new const char*[buckets]();
		items = 0;
		collisions = 0;
	}
	// Free keys before delete
	~HashSet() {
		for(int i = 0; i < buckets; i++) {
			if(set[i] != 0)
				delete[] set[i];
		}
		delete[] set;
	}
	// Add to hash set using open addressing and quadratic probing
	bool add(const char* str) {
		// Keep load below 50%
		if((float)items / (float)buckets > 0.5)
			rehash(buckets);

		int hash = strhash(str) % buckets;
		int i = 1;
		// Bucket empty
		if(!set[hash]) {
			set[hash] = str;
			items++;
			return true;
		}
		// Bucket full, not same
		else if(strcmp(str, set[hash])) {
			while(set[hash]) {
				hash = (hash + (i*i)) % buckets;
				// String found, don't insert
				if(set[hash] && strcmp(str, set[hash]) == 0)
					return false;
			}
			set[hash] = str;
			items++;
			collisions++;
			return true;
		}
		return false;
	}
	// Check hash set using open addressing and quadratic probing
	bool contains(const char* str) {
		int hash = strhash(str) % buckets;
		int i = 1;
		while(set[hash]) {
			if(strcmp(str, set[hash]) == 0)
				return true;
			hash = (hash + (i*i)) % buckets;
			i++;
		}
		return false;
	}
	// Check set for lower case conversion of str
	// There's a small preformance hit with this
	bool contains_tolower(const char* str) {
		int hash = strhash_tolower(str) % buckets;
		int i = 1;
		while(set[hash]) {
			if(stricmp(str, set[hash]) == 0)
				return true;
			hash = (hash + (i*i)) % buckets;
			i++;
		}
		return false;
	}
	// Rehash to accommodate x number of elements
	void rehash(int new_size) {
		int old_buckets = buckets;
		auto old_set = set;

		buckets = new_size * 2;
		set =  new const char*[buckets]();

		items = 0;
		collisions = 0;

		for(int i = 0; i < old_buckets; i++) {
			if(old_set[i] != 0)
				add(old_set[i]);
		}
		delete[] old_set;
	}
	// Empty set
	void clear() {
		for(int i = 0; i < buckets; i++) {
			if(set[i]) {
				delete[] set[i];
				set[i] = 0;
			}
		}
		buckets = 256;
		delete[] set;
		set =  new const char*[buckets]();
		items = 0;
		collisions = 0;
	}
	// Stats
	int size() {
		return items;
	}
	int bucket_count() {
		return buckets;
	}
	int collisions_count() {
		return collisions;
	}
	// Iterator stuff
	const char** begin() {
		if(items == 0)
			return 0;
		const char** it = set;
		while(!*it)
			it++;
		return it;
	}
	void next(const char*** it) {
		do {
			(*it)++;
			if((*it) == &(set[buckets]))
				(*it) = 0;
		} while((*it) != 0 && !**it);
	}
private:
	// djb2 hash
	// Not the best, but fast
	size_t strhash(const char* str) {
		size_t hash = 5381;
		while(*str)
			hash = (hash * 33) + *(str++);
		return hash;
	}
	size_t strhash_tolower(const char* str) {
		size_t hash = 5381;
		while(*str)
			hash = (hash * 33) + tolower_lut[(unsigned char)*(str++)];
		return hash;
	}
	// 
	int stricmp(const char* s1, const char* s2) {
		int c1, c2;
		do {
			c1 = tolower_lut[(unsigned char)*(s1++)];
			c2 = tolower_lut[(unsigned char)*(s2++)];
		} while(c1 && c2 && c1 == c2);
		return c1 - c2;
	}
};
#endif
