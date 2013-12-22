#ifndef HASHSET_H
#define HASHSET_H

#include <cstring>
#include <cctype>

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
		for(; *str; ++str)
			hash = (hash * 33) + *str;
		return hash;
	}
	size_t strhash_tolower(const char* str) {
		size_t hash = 5381;
		for(; *str; ++str)
			hash = (hash * 33) + tolower(*str);
		return hash;
	}
	int stricmp(const char* s1, const char* s2) {
		int c1, c2;
		do {
			c1 = tolower(*s1++);
			c2 = tolower(*s2++);
		} while(c1 && c2 && c1 == c2);
		return c1 - c2;
	}
};
#endif
