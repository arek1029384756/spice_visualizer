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


        //Temporary test
        m_view->fitInView(200, 50, 500, 150, Qt::KeepAspectRatio);

        SchComponent* res = new Resistor();
        res->setX(300);
        res->setY(100);
        m_scene->addItem(res);

        SchComponent* cap = new Capacitor();
        cap->setX(400);
        cap->setY(100);
        m_scene->addItem(cap);

        auto resRect = res->boundingRect();
        auto capRect = cap->boundingRect();
        m_scene->addLine(res->x() + resRect.width(),
                         res->y() + resRect.height() / 2,
                         cap->x(),
                         cap->y() + capRect.height() / 2,
                         SchComponent::getConnectionPen());
        //

    }

    Schematic::~Schematic() {
        delete m_view;
        delete m_scene;
    }
}
