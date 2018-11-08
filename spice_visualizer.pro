######################################################################
# spice_visualizer.pro
######################################################################

QT += widgets

QMAKE_PROJECT_NAME = spice_visualizer

#LOGMODE = -D_SILENT
LOGMODE = -D_VERBOSE

INCLUDEPATH += .
INCLUDEPATH += ./file_io ./parser ./circuit ./task ./gui ./gui_qt
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++17 $$LOGMODE

# Input
HEADERS += gui_qt/schematic.h \
gui_qt/sch_params.hpp \
gui_qt/sch_component.hpp \
gui_qt/graphics_component.hpp \
gui_qt/progress.h \
gui/gui_schematic_interface.hpp \
gui/gui_schematic_qt.hpp \
gui/gui_progress_interface.hpp \
gui/gui_progress_qt.hpp \
circuit/circuit.hpp \
circuit/processor.hpp \
task/base_thread.hpp \
file_io/file_reader.hpp \
parser/parser_ngspice.hpp \
parser/parser_interface.hpp

SOURCES += main.cpp \
gui_qt/schematic.cpp \
gui_qt/progress.cpp
