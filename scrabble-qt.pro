# CONFIG	+=	debug
QT		+=	core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET	 =	scrabble-qt
TEMPLATE =	app

SOURCES	+=	main.cpp \
			scrabble.cpp \
			scrabblegui.cpp \

HEADERS	+=	scrabble.h \
			scrabblerules.h \
			scrabblegui.h \
			hashset.h \
			hashquick.h

FORMS	+=	scrabblegui.ui
