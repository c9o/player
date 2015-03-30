TEMPLATE = app
TARGET = csrplayer

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets \
	  gui

HEADERS = \
    csrplayer.h \
    clickslider.h \
    filedialog.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h
SOURCES = main.cpp \
    csrplayer.cpp \
    clickslider.cpp \
    filedialog.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp

FORMS    += csrplayer.ui

OTHER_FILES +=

RESOURCES += csrplayer.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/player
INSTALLS += target
