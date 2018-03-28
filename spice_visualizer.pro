######################################################################
# spice_visualizer.pro
######################################################################

QT += widgets

QMAKE_PROJECT_NAME = spice_visualizer

#LOGMODE = -D_SILENT
LOGMODE = -D_VERBOSE

INCLUDEPATH += .
INCLUDEPATH += ./file_io ./parser ./circuit ./gui_qt
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++14 $$LOGMODE

# Input
HEADERS += gui_qt/schematic.h \
gui_qt/sch_params.hpp \
gui_qt/sch_component.hpp \
gui_qt/graphics_component.hpp \
circuit/data_format.hpp \
file_io/file_reader.hpp \
parser/parser_ngspice.hpp \
parser/parser_interface.hpp

SOURCES += main.cpp \
gui_qt/schematic.cpp
