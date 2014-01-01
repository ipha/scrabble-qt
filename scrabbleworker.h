#ifndef SCRABBLEWORKER_H
#define SCRABBLEWORKER_H

#include <QObject>
#include <ctime>
#include "scrabble.h"

class ScrabbleWorker : public QObject {
	Q_OBJECT
	QString tiles;
	std::clock_t c_start;
	std::clock_t c_end;
public:
	Scrabble* game;
	int gametype;
	ScrabbleWorker(const char* filename, int gametype);
	~ScrabbleWorker();
	void setBoard(int x, int y, char c);
	void setTiles(QString tiles);
signals:
	void solveDone();
public slots:
	void solve();
};

#endif // SCRABBLEWORKER_H
