#ifndef SCHEMATIC
#define SCHEMATIC

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <schematic_component.hpp>
#include <graphics_component.hpp>
#include <gui_schematic_interface.hpp>

namespace gui_qt {

    class Schematic : public QDialog {
        Q_OBJECT

        public slots:

        public:
            Schematic(gui::GuiSchematicInterfaceInt* ifc, QWidget *parent = 0);
            virtual ~Schematic();
            void keyPressEvent(QKeyEvent* e);

        private:
            QGraphicsScene* m_scene;
            QGraphicsView* m_view;
            SchGrid* m_grid;
            gui::GuiSchematicInterfaceInt* m_ifc;

            void setThickness(int key) const;
            void tmpGuiTest();
    };

}

#endif
