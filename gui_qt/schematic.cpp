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

        tmpGuiTest();
    }

    Schematic::~Schematic() {
        delete m_view;
        delete m_scene;
    }

    void Schematic::tmpGuiTest() {
        //Temporary test
        m_view->fitInView(200, 0, 700, 250, Qt::KeepAspectRatio);

        #if 1
        for(std::size_t i = 0; i <= schWidth; i += gridRaster) {
            m_scene->addLine(i, 0, i, schHeight, QPen(Qt::lightGray, 0));
        }

        for(std::size_t i = 0; i <= schHeight; i += gridRaster) {
            m_scene->addLine(0, i, schWidth, i, QPen(Qt::lightGray, 0));
        }
        #endif

        SchComponent* res = new Resistor();
        res->setX(30 * gridRaster);
        res->setY(10 * gridRaster);
        m_scene->addItem(res);

        SchComponent* cap = new Capacitor();
        cap->setX(42 * gridRaster);
        cap->setY(10 * gridRaster);
        m_scene->addItem(cap);

        SchComponent* tran = new NpnTransistor();
        tran->setX(50 * gridRaster);
        tran->setY(8 * gridRaster);
        m_scene->addItem(tran);

        auto resRect = res->boundingRect();
        auto capRect = cap->boundingRect();
        auto tranRect = tran->boundingRect();
        m_scene->addLine(res->x() + resRect.width(),
                         res->y() + resRect.height() / 2,
                         cap->x(),
                         cap->y() + capRect.height() / 2,
                         SchComponent::getConnectionPen());

        m_scene->addLine(cap->x() + capRect.width(),
                         cap->y() + capRect.height() / 2,
                         tran->x(),
                         tran->y() + tranRect.height() / 2,
                         SchComponent::getConnectionPen());
    }

}
