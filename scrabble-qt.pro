QMAKE_CXXFLAGS += -std=c++11

TEMPLATE =	app

SOURCES	+=	scrabble.cpp \
			ternarytree.cpp

HEADERS	+=	scrabble.h \
			scrabblerules.h \
			ternarytree.h

CONFIG(test) {
	TARGET	 =	scrabble-test
	SOURCES += test.cpp

} else {

	TARGET	 =	scrabble-qt

	QT		+=	core gui
	greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

	SOURCES	+=	main.cpp \
				scrabblegui.cpp \
				scrabbleworker.cpp

	HEADERS	+=	scrabblegui.h \
				scrabbleworker.h

	FORMS	+=	scrabblegui.ui
}
