#include <cstdio>
#include "scrabble.h"

Scrabble::Scrabble (const char* filename, int game) {
	gametype = game;

	// Open wordfile and add to list
	FILE* wordfile = fopen(filename, "r");
	if(wordfile) {
		char line[STRSIZE];
		while(fscanf(wordfile, "%15s", line) != EOF) {
			wordlist.add(line);
		}
		fclose(wordfile);
	} else {
		fprintf(stderr, "ERROR: can't open file: %s", filename);
	}
	wordlist.build_height();

	printf("There are %i words in the list\n", wordlist.size());
	printf("with %i nodes\n", wordlist.node_count());
}

Scrabble::~Scrabble () {
	// delete perms
	for(auto perm : perms)
		delete[] perm;
}

void Scrabble::solve (const char* tiles) {
	update_cache();

	char frame[STRSIZE];
	char word[STRSIZE];

	// Clear previous results
	results.clear();

	for(int length = 1; length <= 7; length++) {
		// Empty perms set
		for(auto perm : perms)
			delete[] perm;
		perms.clear();

		// Fill perms set
		// With blanks
		if(strpbrk (tiles, "? *")) {
			char tiles_temp[8];
			strcpy(tiles_temp, tiles);
			char* blank = strpbrk(tiles_temp, "? *");
			for(char c = 'A'; c <= 'Z'; c++) {
				blank[0] = c;
				get_perms(tiles_temp, length, NULL);
			}
		}
		// Without blanks
		else {
			get_perms(tiles, length, NULL);
		}
		// Horizontal pass
		printf("Checking length %i horizontal\n", length);
		for(int y = 0; y < 15; y++) {
		for(int x = 0; x < (15-length); x++) {
			if(!check_spot(x, y, length, HORIZONTAL))
				continue;
			get_frame(frame, x, y, length, HORIZONTAL);
			if(!wordlist.set_prefix(frame))
				continue;
			for(auto perm : perms) {
				if(check_word(x, y, frame, perm, HORIZONTAL)) {
					fill_frame(word, frame, perm);
					results.insert(result(word, cache[HORIZONTAL][x][y], y, HORIZONTAL, score(x, y, HORIZONTAL, word)));
				}
			}
		}
		}
		// Vertical pass
		printf("Checking length %i vertical\n", length);
		for(int y = 0; y < (15-length); y++) {
		for(int x = 0; x < 15; x++) {
			if(!check_spot(x, y, length, VERTICAL))
				continue;
			get_frame(frame, x, y, length, VERTICAL);
			if(!wordlist.set_prefix(frame))
				continue;
			for(auto perm : perms) {
				if(check_word(x, y, frame, perm, VERTICAL)) {
					fill_frame(word, frame, perm);
					results.insert(result(word, x, cache[VERTICAL][x][y], VERTICAL, score(x, y, VERTICAL, word)));
				}
			}
		}
		}
	}
}

bool Scrabble::check_word (int x, int y, const char* frame, const char* tiles, int direction) {
	if(wordlist.contains_prefix(frame, tiles)) {
		// Check each new word
		char new_word[STRSIZE];
		int start = cache[direction][x][y];
		if(direction == HORIZONTAL) {
			for(; *frame; frame++) {
				if(*frame == ' ') {
					full_word(new_word, start, y, *(tiles++), VERTICAL);
					if(new_word[1] && !wordlist.contains(new_word))
						return false;
				}
				start++;
			}
		} else if(direction == VERTICAL) {
			for(; *frame; frame++) {
				if(*frame == ' ') {
					full_word(new_word, x, start, *(tiles++), HORIZONTAL);
					if(new_word[1] && !wordlist.contains(new_word))
						return false;
				}
				start++;
			}
		}
		return true;
	}
	return false;
}

void Scrabble::update_cache () {
	for(int y = 0; y < 15; y++) {
		for(int x = 0; x < 15; x++) {
			int x_start = x;
			int y_start = y;

			while(x_start > 0 && board[x_start-1][y] != ' ')
				x_start--;
			while(y_start > 0 && board[x][y_start-1] != ' ')
				y_start--;
			cache[HORIZONTAL][x][y] = x_start;
			cache[VERTICAL][x][y] = y_start;
		}
	}
}

bool Scrabble::check_spot (int x, int y, int length, int direction) {
	int al = length;
	int ax = x;
	int ay = y;

	// Special case for first word
	if(x == 7 && y == 7 && board[7][7] == ' ')
		return true;
	if(board[x][y] != ' ')
		return false;
	if(direction == HORIZONTAL) {
		while(al > 0 && ax < 15){
			if(board[ax][ay] == ' ')
				al--;
			ax++;
		}
		// Too long
		if(al != 0)
			return false;
		// Make sure word touches other letters
		// 
		else if(ax > x + length) // Intersects other letter(s)
			return true;
		else if(x > 0 && board[x-1][y] != ' ') // Touches on left
			return true;
		else if(x + length < 15 && board[x+length][y] != ' ') // Touches on right
			return true;
		// Touches on top or bottom
		for(int i = x; i < x + length; i++) {
			if(y > 0 && board[i][y-1] != ' ')
				return true;
			else if(y < 14 && board[i][y+1] != ' ')
				return true;
		}
	} else if(direction == VERTICAL) {
		while(al > 0 && ay < 15){
			if(board[ax][ay] == ' ')
				al--;
			ay++;
		}
		// Too long
		if(al != 0)
			return false;
		// Make sure word touches other letters
		// 
		else if(ay > y + length) // Intersects other letter(s)
			return true;
		else if(y > 0 && board[x][y-1] != ' ') // Touches on top
			return true;
		else if(y + length < 15 && board[x][y+length] != ' ') // Touches on bottom
			return true;
		// Touches on left or right
		for(int i = y; i < y + length; i++) {
			if(x > 0 && board[x-1][i] != ' ')
				return true;
			else if(x < 14 && board[x+1][i] != ' ')
				return true;
		}
	}
	// Bad spot
	return false;
}

void Scrabble::get_frame (char* frame, int x, int y, int length, int direction) {
	int frame_index = 0;
	int start = cache[direction][x][y];

	if(direction == HORIZONTAL) {
		while((start+frame_index) < 15 && board[start+frame_index][y] != ' ') {
			frame[frame_index] = board[start+frame_index][y];
			frame_index++;
		}
		for(int i = 0; i < length; i++) {
			frame[frame_index] = board[start+frame_index][y];
			frame_index++;
			while((start+frame_index) < 15 && board[start+frame_index][y] != ' ') {
				frame[frame_index] = board[start+frame_index][y];
				frame_index++;
			}
		}
	}
	if(direction == VERTICAL) {
		while((start+frame_index) < 15 && board[x][start+frame_index] != ' ') {
			frame[frame_index] = board[x][start+frame_index];
			frame_index++;
		}
		for(int i = 0; i < length; i++) {
			frame[frame_index] = board[x][start+frame_index];
			frame_index++;
			while((start+frame_index) < 15 && board[x][start+frame_index] != ' ') {
				frame[frame_index] = board[x][start+frame_index];
				frame_index++;
			}
		}
	}
	// Null terminate new string
	frame[frame_index] = '\0';
}

// Magic
void Scrabble::fill_frame (char* word, const char* frame, const char* tiles) {
	for(; *frame; frame++) {
		if(*frame == ' ')
			*(word++) = *(tiles++);
		else
			*(word++) = *frame;
	}
	// Null terminate new string
	*word = '\0';
}

void Scrabble::full_word (char* word, int x, int y, char c, int direction) {
	get_frame(word, x, y, 1, direction);
	*strchr(word, ' ') = c;
}

int Scrabble::letter_score(char c) {
	if(c >= 'a' && c <= 'z')
		return letter_value[gametype][c-'a'];
	else
		return 0;
}

score_t Scrabble::score (int x, int y, int direction, const char* word) {
	int score = 0;
	int score_mult = 1;
	int tile_count = 0;
	int second_score = 0;
	int words_created = 1;
	bool triple_used = false;
	char new_word[STRSIZE];

	int start = cache[direction][x][y];

	if(direction == HORIZONTAL) {
		for(unsigned int i = 0; i < strlen(word); i++) {
			// Tile we placed
			if(board[start+i][y] == ' ') {
				tile_count++;
				score += letter_score(word[i]) * letter_mult[gametype][start+i][y];
				score_mult *= word_mult[gametype][start+i][y];
				if(word_mult[gametype][start+i][y] == 3)
					triple_used = true;
				// Check new vertical word
				full_word(new_word, start+i, y, word[i], VERTICAL);
				if(strlen(new_word) > 1) {
					int new_word_score = 0;
					words_created++;
					for(const char* c = new_word; *c; c++)
						new_word_score += letter_score(*c);
					new_word_score += (letter_score(word[i]) * (letter_mult[gametype][start+i][y] - 1));
					second_score += (new_word_score * word_mult[gametype][start+i][y]);
				}

			}
			// Existing tile
			else
				score += letter_score(word[i]);
		}
	}
	if(direction == VERTICAL) {
		for(unsigned int i = 0; i < strlen(word); i++) {
			// Tile we placed
			if(board[x][start+i] == ' ') {
				tile_count++;
				score += letter_score(word[i]) * letter_mult[gametype][x][start+i];
				score_mult *= word_mult[gametype][x][start+i];
				if(word_mult[gametype][x][start+i] == 3)
					triple_used = true;
				// Check new horizontal words
				full_word(new_word, x, start+i, word[i], HORIZONTAL);
				if(strlen(new_word) > 1) {
					int new_word_score = 0;
					words_created++;
					for(const char* c = new_word; *c; c++)
						new_word_score += letter_score(*c);
					new_word_score += (letter_score(word[i]) * (letter_mult[gametype][x][start+i] - 1));
					second_score += (new_word_score * word_mult[gametype][x][start+i]);
				}
			}
			// Existing tile
			else
				score += letter_score(word[i]);
		}
	}

	score *= score_mult;
	score +=	(tile_count == 7 ? all_tiles_bonus[gametype] : 0) +
				(second_score);
	// Weight score
	// +20% if triple word is used
	// -5% for each word created over 2
	// -20% for 2 letter words
	float weight_score = score;
	if(triple_used)
		weight_score *= 1.2;
	for(int i = 0; i < (words_created - 2); i++)
		weight_score *= 0.95;
	if(strlen(word) == 2)
		weight_score *= 0.8;

	return std::make_pair(score, static_cast<int>(weight_score));
}

void Scrabble::get_perms (const char* tiles, unsigned int length, const char* base) {
	for(int i = 0; tiles[i]; i++) {
		if(tiles[i] != '~') { // Skip used tiles
			char* temp = new char[8](); // Zeroing this is important!

			if(base)
				strcpy(temp, base);

			temp[strlen(temp)] = tiles[i];

			if(strlen(temp) == length) {
				if(!perms.insert(temp).second)
					delete[] temp;
			} else {
				char temp2[8];
				strcpy(temp2, tiles);
				// Replace used tiles with ~
				temp2[i] = '~';
				get_perms(temp2, length, temp);
				delete[] temp;
			}
		}
	}
}
