#ifndef SCRABBLEGUI_H
#define SCRABBLEGUI_H

#include <QMainWindow>
#include <QThread>
#include "ui_scrabblegui.h"
#include "scrabbleworker.h"

Q_DECLARE_METATYPE(result)

namespace Ui {
	class ScrabbleGui;
}

class ScrabbleGui : public QMainWindow {
	Q_OBJECT
public:
	explicit ScrabbleGui(QWidget *parent = 0);
	~ScrabbleGui();
private:
	Ui::ScrabbleGui *ui;
	QMenu* new_menu;
	ScrabbleWorker *solver;
	QThread* solverThread;
	void new_game(int gametype);
	void load(QString filename);
private slots:
	void solve();
	void solveDone();
	void save();
	void load();
	void new_action(QAction* action);
	void highlight(QListWidgetItem* item);
};

#endif // SCRABBLEGUI_H
