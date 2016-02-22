#ifndef HASHSET_H
#define HASHSET_H

#include <cstdint>

// Limited hash set for storing char* strings
// No way to remove keys once added
// probably not memory efficient
// basic iterator support

class HashSet {
	const char** set;
	uint32_t buckets;
	uint32_t mask;
	uint32_t collisions;
	uint32_t items;
public:
	// Create set with 256 buckets
	HashSet() {
		buckets = 256;
		mask = buckets-1;
		set =  new const char*[buckets]();
		items = 0;
		collisions = 0;
	}
	// Free keys before delete
	~HashSet() {
		for(uint32_t i = 0; i < buckets; i++) {
			if(set[i] != 0)
				delete[] set[i];
		}
		delete[] set;
	}
	// Add to hash set using open addressing and quadratic probing
	bool add(const char* str) {
		// Keep load below 50%
		if((float)items / (float)buckets > 0.50)
			rehash(buckets);

		uint32_t hash = strhash(str) & mask;
		uint32_t i = 1;
		// Bucket empty
		if(!set[hash]) {
			set[hash] = str;
			items++;
			return true;
		}
		// Bucket full, not same
		else if(stricmp(str, set[hash])) {
			while(set[hash]) {
				hash = (hash + (i*i)) & mask;
				// String found, don't insert
				if(set[hash] && stricmp(str, set[hash]) == 0)
					return false;
				i++;
			}
			set[hash] = str;
			items++;
			collisions++;
			return true;
		}
		return false;
	}
	// Check hash set using open addressing and quadratic probing
	// Check set for lower case conversion of str
	// There's a small preformance hit with this
	bool contains(const char* str) {
		uint32_t hash = strhash(str) & mask;
		uint32_t i = 1;
		while(set[hash]) {
			if(stricmp(str, set[hash]) == 0)
				return true;
			hash = (hash + (i*i)) & mask;
			i++;
		}
		return false;
	}
	// Rehash to accommodate x number of elements
	void rehash(uint32_t new_size) {
		uint32_t old_buckets = buckets;
		auto old_set = set;

		buckets = new_size * 2;
		mask = buckets - 1;
		set =  new const char*[buckets]();

		items = 0;
		collisions = 0;

		for(uint32_t i = 0; i < old_buckets; i++) {
			if(old_set[i] != 0)
				add(old_set[i]);
		}
		delete[] old_set;
	}
	// Empty set
	void clear() {
		for(uint32_t i = 0; i < buckets; i++) {
			if(set[i]) {
				delete[] set[i];
				set[i] = 0;
			}
		}
		buckets = 256;
		mask = buckets - 1;
		delete[] set;
		set =  new const char*[buckets]();
		items = 0;
		collisions = 0;
	}
	// Stats
	uint32_t size() {
		return items;
	}
	uint32_t bucket_count() {
		return buckets;
	}
	uint32_t collisions_count() {
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
	inline uint32_t strhash(const char* str) {
		uint32_t hash = 5381;
		while(*str) {
			hash = (hash * 33) + (*(str++) & 0x1F);
			// hash = (*(str++) & 0x1F) + (hash * 65599);
		}
		return hash;
	}
	// 
	inline uint32_t stricmp(const char* s1, const char* s2) {
		uint32_t c1, c2;
		do {
			c1 = (*(s1++) & 0x1F);
			c2 = (*(s2++) & 0x1F);
		} while(c1 && c2 && c1 == c2);
		return c1 - c2;
	}
};
#endif
