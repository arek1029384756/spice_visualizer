#ifndef SCHEMATIC
#define SCHEMATIC

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <qmath.h>
#include <functional>
#include <map>
#include <schematic_component.hpp>
#include <gui_schematic_interface.hpp>

namespace gui_qt {

    class GraphicsView : public QGraphicsView {
        bool m_gridEnable;

        protected:
            void wheelEvent(QWheelEvent* e) override {
                if(e->modifiers() & Qt::ControlModifier) {
                    auto exp = (e->delta() > 0) ? g_scaleExp : -g_scaleExp;
                    auto factor = qPow(qreal(2), exp);
                    scale(factor, factor);
                    e->accept();
                } else {
                    QGraphicsView::wheelEvent(e);
                }
            }

            void drawBackground(QPainter* painter, const QRectF& rect) override {
                if(m_gridEnable) {
                    painter->setClipRect(rect);
                    painter->setPen(QPen(Qt::lightGray, 0));
                    const qreal details = transform().m11();
                    const std::size_t delta = (details > 0.5) ? 1 : 10;

                    for(std::size_t i = 0; i <= g_logSchWidth; i += delta) {
                        painter->drawLine(L2P(i), 0, L2P(i), L2P(g_logSchHeight));
                    }

                    for(std::size_t i = 0; i <= g_logSchHeight; i += delta) {
                        painter->drawLine(0, L2P(i), L2P(g_logSchWidth), L2P(i));
                    }
                }
            }

        public:
            GraphicsView(QGraphicsScene* scene)
                : QGraphicsView(scene), m_gridEnable(true) {}
            virtual ~GraphicsView() = default;

            void toggleGrid() {
                auto* const v = viewport();
                m_gridEnable = !m_gridEnable;
                v->update();
            }
    };

    class Schematic : public QDialog {
        Q_OBJECT

        public slots:

        public:
            Schematic(gui::GuiSchematicInterfaceInt* ifc, QWidget *parent = 0);
            void keyPressEvent(QKeyEvent* e) override;
            virtual ~Schematic();

        private:
            QGraphicsScene* m_scene;
            GraphicsView* m_view;
            gui::GuiSchematicInterfaceInt* m_ifc;

            void toggleGrid(int key) const;
            void setThickness(int key) const;
            void zoomSchematic(int key) const;
            void scrollSchematic(int key) const;
            void showAll(int key) const;
            void showOptimal(int key) const;

            void tmpGuiTest();

            inline static const std::map<int, std::function<void(Schematic* const, int)>> m_keyCommands = {
                { Qt::Key_G,        &Schematic::toggleGrid },
                { Qt::Key_A,        &Schematic::showAll },
                { Qt::Key_O,        &Schematic::showOptimal },
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
