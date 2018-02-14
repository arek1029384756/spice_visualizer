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
INCLUDEPATH += ./file_io ./parser ./graph
INCLUDEPATH += /usr/include/$$LOCAL_QT5_INC/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -std=c++14 $$LOGMODE
LIBS += -L/usr/lib/$$LOCAL_QT5_LIB -lQt5Widgets

# Input
HEADERS += \
graph/data_format.hpp \
file_io/file_reader.hpp \
parser/parser_ngspice.hpp \
parser/parser_interface.hpp

SOURCES += main.cpp
