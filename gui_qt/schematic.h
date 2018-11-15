#ifndef SCHEMATIC
#define SCHEMATIC

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <functional>
#include <map>
#include <schematic_component.hpp>
#include <graphics_component.hpp>
#include <gui_schematic_interface.hpp>

namespace gui_qt {

    class GraphicsView : public QGraphicsView {
        protected:
            void wheelEvent(QWheelEvent* e) override {
                if(e->modifiers() & Qt::ControlModifier) {
                    auto factor = (e->delta() > 0) ? g_scaleUp : g_scaleDown;
                    scale(factor, factor);
                    e->accept();
                } else {
                    QGraphicsView::wheelEvent(e);
                }
            }

        public:
            GraphicsView(QGraphicsScene* scene) : QGraphicsView(scene) {}
            virtual ~GraphicsView() = default;
    };

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

            void toggleGrid(int key) const;
            void setThickness(int key) const;
            void zoomSchematic(int key) const;
            void scrollSchematic(int key) const;

            void tmpGuiTest();

            inline static const std::map<int, std::function<void(Schematic* const, int)>> m_keyCommands = {
                { Qt::Key_G,        &Schematic::toggleGrid },
                { Qt::Key_1,        &Schematic::setThickness },
                { Qt::Key_2,        &Schematic::setThickness },
                { Qt::Key_3,        &Schematic::setThickness },
                { Qt::Key_PageUp,   &Schematic::zoomSchematic },
                { Qt::Key_PageDown, &Schematic::zoomSchematic },
                { Qt::Key_Left,     &Schematic::scrollSchematic },
                { Qt::Key_Right,    &Schematic::scrollSchematic },
                { Qt::Key_Up,       &Schematic::scrollSchematic },
                { Qt::Key_Down,     &Schematic::scrollSchematic }
            };
    };

}

#endif
