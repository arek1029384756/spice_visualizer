######################################################################
# spice_visualizer.pro
######################################################################

TEMPLATE = app
TARGET = spice_visualizer

#LOGMODE = -D_SILENT
LOGMODE = -D_VERBOSE

LOCAL_QT5_PFX = x86_64-linux-gnu

# Qt 5.5.x
LOCAL_QT5_INC = $$LOCAL_QT5_PFX

# Qt 5.2.x
# LOCAL_QT5_INC =

LOCAL_QT5_LIB = $$LOCAL_QT5_PFX
INCLUDEPATH += .
INCLUDEPATH += ./file_io ./parser
INCLUDEPATH += /usr/include/$$LOCAL_QT5_INC/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++1y $$LOGMODE
LIBS += -L/usr/lib/$$LOCAL_QT5_LIB -lQt5Widgets

# Input
HEADERS += \
file_io/file_reader.hpp \
parser/parser_ngspice.hpp \
parser/parser_interface.hpp

SOURCES += main.cpp
