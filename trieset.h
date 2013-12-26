#ifndef TRIESET_H
#define TRIESET_H

static unsigned char triemap_lut[128] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,
	// Upper alpha
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
	0x00,0x00,0x00,0x00,0x00,0x00,
	// Lower alpha
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
	0x00,0x00,0x00,0x00,0x00
};

// Fast, memory inefficient trie for lowercase alpha strings.
class TrieSet {
	struct TrieSetItem {
		bool word = false;
		TrieSetItem** children;
	};
	TrieSetItem* root;
	int nodes = 1;
	int endpoints = 1;
	int items = 0;
public:
	TrieSet() {
		root = new TrieSetItem();
	}
	~TrieSet() {
		delete_recursive(root);
	}
	void add(const char* string) {
		auto node = root;
		for(; *string; string++) {
			if(!node->children) {
				node->children = new TrieSetItem*[26]{};
				endpoints--;
			}
			if(!node->children[triemap_lut[(unsigned char)*string]]){
				node->children[triemap_lut[(unsigned char)*string]] = new TrieSetItem();
				nodes++;
				endpoints++;
			}
			node = node->children[triemap_lut[(unsigned char)*string]];
		}
		if(!node->word) {
			node->word = true;
			items++;
		}
	}
	bool contains(const char* string) {
		auto node = root;
		for(; *string; string++) {
			if(node->children && node->children[triemap_lut[(unsigned char)*string]])
				node = node->children[triemap_lut[(unsigned char)*string]];
			else
				return false;
		}
		return node->word;
	}
	bool contains(const char* frame, const char* tiles) {
		auto node = root;
		unsigned char c;
		for(; *frame; frame++) {
			c = (*frame == ' ') ? *(tiles++) : *frame;
			if(node->children && node->children[triemap_lut[c]])
				node = node->children[triemap_lut[c]];
			else
				return false;
		}
		return node->word;
	}
	void clear() {
		delete_recursive(root);
		root = new TrieSetItem();
		nodes = 1;
		endpoints = 1;
		items = 0;
	}
	int size() {
		return items;
	}
	int node_count() {
		return nodes;
	}
	int endpoint_count() {
		return endpoints;
	}
private:
	void delete_recursive(TrieSetItem* node) {
		if(node->children) {
			for(int i = 0; i < 26; i++) {
				if(node->children[i])
					delete_recursive(node->children[i]);
			}
			delete[] node->children;
		}
		delete node;
	}
};
#endif
