#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <cstring>
#include <vector>
#include "hashset.h"
#include "scrabblerules.h"

#define HORIZONTAL 0
#define VERTICAL 1

#define STRSIZE 16

struct result {
	char word[STRSIZE];
	int x;
	int y;
	int direction;
	int score;
	int weight;
	// Sort by weight, score, then word
	bool operator < (const result& r) const {
		if(weight == r.weight) {
			if(score == r.score)
				return (strcmp(word, r.word) < 0);
			return (score < r.score);
		}
		return (weight < r.weight);
	}
};

struct score_t {
	int score;
	int weight;
};

struct perm {
	const char* tiles;
	bool operator ==(const char* s) const {
		return (strcmp(s, tiles) == 0);
	}
};

class Scrabble {
	int cache[2][15][15];
	HashSet wordlist;
	HashSet perms;
public:
	int gametype;
	char board[15][15];
	std::vector<result> results;
	Scrabble			(const char* wordlist, int gametype);
	~Scrabble			();
	void solve			(const char* tiles);
private:
	void update_cache	();
	bool check_word		(int x, int y, const char* word, int direction);
	bool check_spot		(int x, int y, int length, int direction);
	void get_frame		(char* frame, int x, int y, int length, int direction);
	void fill_frame		(char* word, const char* frame, const char* tiles);
	void full_word		(char* word, int x, int y, char c, int direction);
	int letter_score	(char c);
	score_t score		(int x, int y, int direction, const char* word);
	void get_perms		(const char* tiles, unsigned int length, const char *base);
};

#endif // SCRABBLE_H
