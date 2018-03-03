#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace gui_qt {

    class Schematic : public QDialog
    {
        Q_OBJECT

        static constexpr qreal schWidth = 1000;
        static constexpr qreal schHeight = schWidth / 2;

        public slots:

        public:
            Schematic(QWidget *parent = 0);
            virtual ~Schematic();

        private:
            QGraphicsScene* m_scene;
            QGraphicsView* m_view;

            void tmpGuiTest();
    };

}

#endif
