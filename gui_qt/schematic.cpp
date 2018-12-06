#include <QtWidgets>
#include <iostream>
#include <schematic.h>

namespace gui_qt {

    qreal SchComponent::bodyThick = 0.0;
    qreal SchComponent::termThick = 0.0;

    Schematic::Schematic(gui::GuiSchematicInterfaceInt* ifc, QWidget *parent)
        : QDialog(parent), m_ifc(ifc) {

        m_scene = new QGraphicsScene();
        m_view = new GraphicsView(m_scene);

        m_scene->setSceneRect(0, 0, L2P(g_logSchWidth), L2P(g_logSchHeight));
        m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        m_view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
        m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        m_view->showMaximized();

        setThickness(Qt::Key_1);
        grabKeyboard();

        tmpGuiTest();
    }

    Schematic::~Schematic() {
        delete m_view;
        delete m_scene;
    }

    void Schematic::tmpGuiTest() {
        //Temporary test
        SchComponent* res = new Resistor("LEFT", "1", "R1", "1000k");
        res->setComponentPos("2", QPointF(30, 10));
        m_scene->addItem(res);

        SchComponent* cap = new Capacitor("UP", "1", "C123", "1n");
        cap->setComponentPos("1", QPointF(40, 10));
        m_scene->addItem(cap);

        SchComponent* tran = new NpnTransistor("DOWN", "3", "Q2", "BC548B");
        tran->setComponentPos("2", QPointF(50, 10));
        m_scene->addItem(tran);
    }

    void Schematic::toggleGrid(int /* key */) const {
        m_view->toggleGrid();
    }

    void Schematic::setThickness(int key) const {
        if(key == Qt::Key_1) {
            SchComponent::setThickness(0.3, 0.1);
        } else if(key == Qt::Key_2) {
            SchComponent::setThickness(0.4, 0.2);
        } else if(key == Qt::Key_3) {
            SchComponent::setThickness(0.6, 0.3);
        }
        m_scene->update();
    }

    void Schematic::zoomSchematic(int key) const {
        if(key == Qt::Key_PageUp) {
            m_view->scale(g_scaleUp, g_scaleUp);
        } else if(key == Qt::Key_PageDown) {
            m_view->scale(g_scaleDown, g_scaleDown);
        }
    }

    void Schematic::scrollSchematic(int key) const {
        constexpr int delta = 20;
        auto valH = m_view->horizontalScrollBar()->value();
        auto valV = m_view->verticalScrollBar()->value();

        if(key == Qt::Key_Left) {
            valH -= delta;
        } else if(key == Qt::Key_Right) {
            valH += delta;
        } else if(key == Qt::Key_Up) {
            valV -= delta;
        } else if(key == Qt::Key_Down) {
            valV += delta;
        }

        m_view->horizontalScrollBar()->setValue(valH);
        m_view->verticalScrollBar()->setValue(valV);
    }

    void Schematic::showAll(int /* key */) const {
        m_view->fitInView(0,
                          0,
                          L2P(g_logSchWidth),
                          L2P(g_logSchHeight),
                          Qt::KeepAspectRatio);
    }

    void Schematic::showOptimal(int /* key */) const {
        m_view->fitInView(m_scene->itemsBoundingRect(),
                          Qt::KeepAspectRatio);
    }

    void Schematic::keyPressEvent(QKeyEvent* e) {
        const auto key = e->key();
        auto it = m_keyCommands.find(key);
        if(it != m_keyCommands.end()) {
            auto fun = std::bind(it->second, this, key);
            fun();
        }

        QDialog::keyPressEvent(e);
    }

}
