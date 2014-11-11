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
    playercontrols.h \
    playlistmodel.h \
    videowidget.h
SOURCES = main.cpp \
    csrplayer.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp

FORMS    += csrplayer.ui

OTHER_FILES +=

RESOURCES += \
    csrplayer.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/player
INSTALLS += target
