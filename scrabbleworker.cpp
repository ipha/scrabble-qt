#include <cstring>
#include <ctime>
#include <cstdio>
#include "scrabbleworker.h"

ScrabbleWorker::ScrabbleWorker(const char* filename, int gametype) {
	this->gametype = gametype;
	c_start = std::clock();
	game = new Scrabble(filename, gametype);
	c_end = std::clock();
	printf("Worker Thread: load took %.0f ms\n", 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC);
}

ScrabbleWorker::~ScrabbleWorker() {
	delete game;
}

void ScrabbleWorker::setBoard(int x, int y, char c) {
	game->board[x][y] = c;
}

void ScrabbleWorker::setTiles(QString tiles) {
	this->tiles = tiles;
}

void ScrabbleWorker::solve() {
	c_start = std::clock();
	game->gametype = gametype;
	game->solve(qPrintable(tiles));
	c_end = std::clock();
	printf("Worker Thread: solve took %.0f ms\n", 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC);
	emit solveDone();
}
