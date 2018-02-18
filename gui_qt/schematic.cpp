#include <QtWidgets>
#include <iostream>
#include <schematic.h>
#include <sch_component.hpp>

namespace gui_qt {

    Schematic::Schematic(QWidget *parent)
        : QDialog(parent) {

        m_scene = new QGraphicsScene();
        m_view = new QGraphicsView(m_scene);

        m_scene->setSceneRect(0, 0, schWidth, schHeight);
        m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        m_view->show();


        //Test
        m_view->fitInView(200, 50, 300, 150, Qt::KeepAspectRatio);

        SchComponent* comp = new Resistor();
        comp->setX(300);
        comp->setY(100);
        m_scene->addItem(comp);

        comp = new Capacitor();
        comp->setX(400);
        comp->setY(100);
        m_scene->addItem(comp);
        //

    }

    Schematic::~Schematic() {
        delete m_view;
        delete m_scene;
    }
}
