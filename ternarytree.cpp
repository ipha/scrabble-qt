#include <cstring>
#include "ternarytree.h"

TernaryTree::TernaryTree() {
}

TernaryTree::~TernaryTree() {
	if(root)
		remove_recursive(root);
}

// Public wrapper for recursive add
void TernaryTree::add(const char *string) {
	root = add_recursive(string, root);
	clear_prefix();
}

// Public wrapper for height building
void TernaryTree::build_height() {
	build_height(root);
}

// Checks tree for string
bool TernaryTree::contains(const char *string) {
	Item* node = root;
	unsigned char c = tolower_lut[(unsigned char)*(string++)];

	while(node) {
		if(c == node->splitchar) {
			if(!*string)
				return node->word;
			node = node->equal;
			c = tolower_lut[(unsigned char)*(string++)];
		}
		else
			node = (c < node->splitchar) ? node->low : node->high;
	}
	return false;
}

// Checks tree starting at prefix
// Spaces in frame are filled with a char from tiles
// Number of tiles should be >= the number of spaces in frame
bool TernaryTree::contains_prefix(const char* frame, const char* tiles) {
	Item* node = prefix;
	frame += prefix_count;
	unsigned char c;

	if(*frame == ' ' && *tiles)
		c = tolower_lut[(unsigned char)*(tiles++)];
	else
		c = tolower_lut[(unsigned char)*frame];

	while(node) {
		if(c == node->splitchar) {
			if(!*(++frame))
				return node->word;
			node = node->equal;

			if(*frame == ' ' && *tiles)
				c = tolower_lut[(unsigned char)*(tiles++)];
			else
				c = tolower_lut[(unsigned char)*frame];
		}
		else
			node = (c < node->splitchar) ? node->low : node->high;
	}
	return false;
}

// Sets the starting point for contains_prefix
// Usefull if you're doing multiple searches for similar strings
bool TernaryTree::set_prefix(const char* string) {
	Item* node = root;
	prefix_count = 0;
	unsigned char c = tolower_lut[(unsigned char)*(string++)];

	while(node && c && c != ' ') {
		if(c == node->splitchar) {
			node = node->equal;
			c = tolower_lut[(unsigned char)*(string++)];
			prefix_count++;
		}
		else
			node = (c < node->splitchar) ? node->low : node->high;
	}

	prefix = node;
	if(node && (node->height >= (int)strlen(string)))
		return true;
	else
		return false;
}

// Reset prefix to root
void TernaryTree::clear_prefix() {
	prefix = root;
	prefix_count = 0;
}

// Empty the tree
// No other way to remove elements currently
void TernaryTree::clear() {
	remove_recursive(root);
	root = 0;
	items = 0;
	nodes = 0;
	clear_prefix();
}

int TernaryTree::size() {
	return items;
}

int TernaryTree::node_count() {
	return nodes;
}

// Recursive add for string
// Traverses tree and adds nodes for each char in string
TernaryTree::Item* TernaryTree::add_recursive(const char* string, Item* node) {
	if(!node) {
		nodes++;
		node = new Item(*string);
	}
	if(*string < node->splitchar)
		node->low = add_recursive(string, node->low);
	else if(*string > node->splitchar)
		node->high = add_recursive(string, node->high);
	else {
		if(*(++string) == '\0') {
			if(!node->word)
				items++;
			node->word = true;
		}
		else
			node->equal = add_recursive(string, node->equal);
	}
	return node;
}

// Remove item and all children
void TernaryTree::remove_recursive(Item* node) {
	if(node->low)
		remove_recursive(node->low);
	if(node->equal)
		remove_recursive(node->equal);
	if(node->high)
		remove_recursive(node->high);
	delete node;
}

// Build a height map for use in prefix optimization
// The height field on each node is the hight of longest child starting with splitchar
//
// For a wordlist containing only cat, the height at c is 2
// For a wordlist containing cat and cargo, the height at c is 4
int TernaryTree::build_height(Item* node) {
	if(node) {
		int low = build_height(node->low);
		int equal = build_height(node->equal) + 1;
		int high = build_height(node->high);

		node->height = equal;

		if(low >= equal && low >= high)
			return low;
		else if(equal >= low && equal >= high)
			return equal;
		else
			return high;
	} else {
		return -1;
	}
}
