#ifndef SCRABBLEGUI_H
#define SCRABBLEGUI_H

#include <QMainWindow>
#include "ui_scrabblegui.h"
#include "scrabble.h"

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
	Scrabble *solver;
private slots:
	void solve();
	void save();
	void load();
	void highlight(QListWidgetItem* item);
};

#endif // SCRABBLEGUI_H
