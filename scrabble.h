#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <cstring>
#include <utility> // std::pair
#include <set>
#include "ternarytree.h"
#include "scrabblerules.h"

#define HORIZONTAL 0
#define VERTICAL 1

#define STRSIZE 16

typedef std::pair<int, int> score_t;

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

struct set_strcomp {
	bool operator()(const char *s1, const char *s2) const {
		return strcmp(s1, s2) < 0;
	}
};

class Scrabble {
	int cache[2][15][15];
	TernaryTree wordlist;
	std::set<const char*, set_strcomp> perms;
public:
	int gametype;
	char board[15][15];
	std::set<result> results;
	Scrabble			(const char* wordlist, int gametype);
	~Scrabble			();
	void solve			(const char* tiles);
private:
	void update_cache	();
	bool check_word		(int x, int y, const char* frame, const char* tiles, int direction);
	bool check_spot		(int x, int y, int length, int direction);
	void get_frame		(char* frame, int x, int y, int length, int direction);
	void fill_frame		(char* word, const char* frame, const char* tiles);
	void full_word		(char* word, int x, int y, char c, int direction);
	int letter_score	(char c);
	score_t score		(int x, int y, int direction, const char* word);
	void get_perms		(const char* tiles, unsigned int length, const char *base);
};

#endif // SCRABBLE_H
