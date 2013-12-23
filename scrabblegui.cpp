#include <ctime>
#include <fstream>
#include <QFileDialog>
#include <QMenu>
#include "scrabblegui.h"

ScrabbleGui::ScrabbleGui(QWidget *parent) : QMainWindow(parent), ui(new Ui::ScrabbleGui) {
	ui->setupUi(this);

	int gametype = WORDSWITHFRIENDS;

	solver = new Scrabble("wordlist.txt", gametype);

	// Create new menu
	QMenu* new_menu = new QMenu();
	new_menu->addAction(ui->newScrabble);
	new_menu->addAction(ui->newWordsWithFriends);
	ui->new_btn->setMenu(new_menu);
	QObject::connect(new_menu, SIGNAL(triggered(QAction*)),
					this, SLOT(new_action(QAction*)));

	// Set reults font
	QFont monofont("monospace");
	monofont.setStyleHint(QFont::Monospace);

	ui->results->setFont(monofont);

	// Set up board items
	for(int x = 0; x < 15; x++) {
		for(int y = 0; y < 15; y++) {
			QTableWidgetItem *item = new QTableWidgetItem();

			item->setTextAlignment(Qt::AlignCenter);
			item->setFont(QFont("Sans", 10, QFont::Bold));

			ui->board->setItem(y, x, item);
		}
	}

	new_game(gametype);

	// If debug.save exists load it and start a solve
	std::ifstream debug_file("debug.save");
	if(debug_file) {
		// Read game type
		char c = debug_file.get();
		debug_file.get();
		new_game(c == '0' ? SCRABBLE : WORDSWITHFRIENDS);

		// Read tiles
		char tiles[8];
		debug_file.getline(tiles,8);
		ui->letters->setText(tiles);

		// Read board
		for(int y = 0; y < 15; y++) {
			for(int x = 0; x < 15; x++) {
				do {
					c = debug_file.get();
				} while(c == ',' || c == '\n');
				ui->board->item(y, x)->setText(QChar(c));
			}
		}
		solve();
	}
}

ScrabbleGui::~ScrabbleGui() {
	delete ui;
	delete solver;
}

void ScrabbleGui::highlight(QListWidgetItem* item) {
	result r = item->data(Qt::UserRole).value<result>();
	ui->board->clearSelection();
	if(r.direction == HORIZONTAL) {
		ui->board->setRangeSelected(
			QTableWidgetSelectionRange(r.y, r.x,
									   r.y, r.x + strlen(r.word) - 1),
			true);
	} else if(r.direction == VERTICAL) {
		ui->board->setRangeSelected(
			QTableWidgetSelectionRange(r.y, r.x,
									   r.y + strlen(r.word) - 1, r.x),
			true);
	}
}

void ScrabbleGui::solve() {
	ui->results->clear();

	std::clock_t c_start = std::clock();
	// Start timer
	//
	// Read board
	for(int x = 0; x < 15; x++) {
		for(int y = 0; y < 15; y++) {
			QString tile = ui->board->item(y, x)->text();
			if(tile.length() == 1 && ((tile[0] >= 'A' && tile[0] <= 'z') || tile[0] == ' ')) {
				solver->board[x][y] = tile[0].toLatin1();
			} else {
				qWarning("Error at tile %2i, %2i: %s", x, y, qPrintable(tile));
				ui->board->item(y, x)->setText(" ");
				solver->board[x][y] = ' ';
			}
		}
	}
	// Start solve
	solver->solve(qPrintable(ui->letters->displayText()));
	// Display results
	for(auto it=solver->results.begin(); it!=solver->results.end(); ++it) {
		QListWidgetItem* item = new QListWidgetItem(
			QString("%1: x: %2  y: %3 Weight: %4 Score: %5 Word: %6").
					arg(it->direction ? "Vert" : "Horz").
					arg(it->x + 1, 2).
					arg(it->y + 1, 2).
					arg(it->weight, 3).
					arg(it->score, 3).
					arg(it->word));
		item->setData(Qt::UserRole, qVariantFromValue((result)*it));
		ui->results->addItem(item);
	}
	//
	// End timer
	std::clock_t c_end = std::clock();
	printf("Solve took %.0f ms\n", 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC);
	ui->results->addItem((QString("Solve took %1 ms").arg(1000.0 * (c_end-c_start) / CLOCKS_PER_SEC)));
	ui->results->scrollToBottom();

}

void ScrabbleGui::save() {
	QString file_name = QFileDialog::getSaveFileName(this, "Load Game", "", "Scrabble (*.save)");
	if(!file_name.endsWith(".save"))
		file_name.append(".save");

	std::ofstream file(qPrintable(file_name), std::fstream::trunc);

	// TODO: Better file format
	file << solver->gametype << "\n";
	file << qPrintable(ui->letters->displayText()) << "\n";
	for(int y = 0; y < 15; y++) {
		for(int x = 0; x < 15; x++) {
			QString tile = ui->board->item(y, x)->text();
			if(tile.length() == 1 && ((tile[0] >= 'A' && tile[0] <= 'z') || tile[0] == ' ')) {
				file << tile[0].toLatin1() << ",";
			} else {
				qWarning("Error at tile %2i, %2i: %s", x, y, qPrintable(tile));
				ui->board->item(y, x)->setText(" ");
				file << " " << ",";
			}
		}
		file << "\n";
	}
	file.close();
}

// TODO: Error handlig in load
void ScrabbleGui::load() {
	QString file_name = QFileDialog::getOpenFileName(this, "Load Game", "", "Scrabble (*.save)");

	std::ifstream file(qPrintable(file_name));

	// Read game type
	char c = file.get();
	file.get();
	new_game(c == '0' ? SCRABBLE : WORDSWITHFRIENDS);

	// Read tiles
	char tiles[8];
	file.getline(tiles,8);
	ui->letters->setText(tiles);

	// Read board
	for(int y = 0; y < 15; y++) {
		for(int x = 0; x < 15; x++) {
			do {
				c = file.get();
			} while(c == ',' || c == '\n');
			ui->board->item(y, x)->setText(QChar(c));
		}
	}
}

void ScrabbleGui::new_action(QAction* action) {
	qDebug(qPrintable(action->text()));
	int gametype = action->text() == "Scrabble" ? SCRABBLE : WORDSWITHFRIENDS;
	new_game(gametype);
}

void ScrabbleGui::new_game(int gametype) {
	solver->gametype = gametype;
	ui->letters->setText("");
	ui->results->clear();
	// Set up board colors
	for(int x = 0; x < 15; x++) {
		for(int y = 0; y < 15; y++) {
			ui->board->item(y, x)->setText(" ");
			// Double letter
			if(letter_mult[gametype][x][y] == 2)
				ui->board->item(y, x)->setBackground(QColor(0, 0, 127, 100));
			// Triple letter
			else if(letter_mult[gametype][x][y] == 3)
				ui->board->item(y, x)->setBackground(QColor(0, 170, 0, 100));
			// Double word
			else if(word_mult[gametype][x][y] == 2)
				ui->board->item(y, x)->setBackground(QColor(170, 0, 0, 100));
			// Triple word
			else if(word_mult[gametype][x][y] == 3)
				ui->board->item(y, x)->setBackground(QColor(255, 170, 0, 100));
			// Center
			else if(x == 7 && y == 7)
				ui->board->item(y, x)->setBackground(QColor(0, 0, 0, 100));
			else
				ui->board->item(y, x)->setBackground(QColor(255, 255, 255, 255));
		}
	}
}
