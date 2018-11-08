#include <QtWidgets>
#include <iostream>
#include <schematic.h>

namespace gui_qt {

    qreal SchComponent::bodyThick = 0.0;
    qreal SchComponent::termThick = 0.0;

    Schematic::Schematic(gui::GuiSchematicInterfaceInt* ifc, QWidget *parent)
        : QDialog(parent), m_ifc(ifc) {

        m_scene = new QGraphicsScene();
        m_view = new QGraphicsView(m_scene);

        m_scene->setSceneRect(0, 0, L2P(g_logSchWidth), L2P(g_logSchHeight));
        m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        m_view->showMaximized();

        m_grid = new SchGrid();
        m_grid->enableGrid(true);
        m_scene->addItem(m_grid);

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
        m_view->fitInView(200, 0, 700, 250, Qt::KeepAspectRatio);

        SchComponent* res = new Resistor(QPointF(10, 9), "DOWN", "2");
        m_scene->addItem(res);

        SchComponent* cap = new Capacitor(QPointF(30, 10), "LEFT", "1");
        m_scene->addItem(cap);

        SchComponent* tran = new NpnTransistor(QPointF(50, 8), "UP", "2");
        m_scene->addItem(tran);
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

    void Schematic::keyPressEvent(QKeyEvent* e) {
        const auto key = e->key();
        if(key == Qt::Key_G) {
            m_grid->toggleGrid();
        } else {
            setThickness(key);
        }

        QDialog::keyPressEvent(e);
    }

}
