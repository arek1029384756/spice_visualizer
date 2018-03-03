#ifndef SCHCOMPONENT_H
#define SCHCOMPONENT_H

#include <QGraphicsItem>

namespace gui_qt {

    constexpr qreal gridRaster = 10;

    class SchComponent : public QGraphicsItem {

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        static QPen getBodyPen() {
            return QPen(Qt::black, gridRaster * 0.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        static QPen getTerminalPen() {
            return QPen(Qt::black, gridRaster * 0.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        protected:
        virtual void drawBody(QPainter* painter) const = 0;
        virtual void drawTerminals(QPainter* painter) const = 0;

        public:
        static QPen getConnectionPen() {
            return getTerminalPen();
        }

        SchComponent(qreal length, qreal width, qreal margin)
            : m_length(length), m_width(width), m_margin(margin) {
        }

        virtual ~SchComponent() {
        }

        virtual QRectF boundingRect() const override {
            return QRectF(0, 0, m_length, m_width);
        }

        virtual void paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *,
                    QWidget *) override {
            painter->setPen(getBodyPen());
            drawBody(painter);

            painter->setPen(getTerminalPen());
            drawTerminals(painter);
        }

    };

    class Resistor : public SchComponent {

        static constexpr qreal length = gridRaster * 7;
        static constexpr qreal width = gridRaster * 4;
        static constexpr qreal margin = gridRaster;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawRect(margin, margin, length - margin * 2, width - margin * 2);
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, width / 2, margin, width / 2);
            painter->drawLine(length - margin, width / 2, length, width / 2);
        }

        public:
        Resistor()
            : SchComponent(length, width, margin) {
        }

    };

    class Capacitor : public SchComponent {

        static constexpr qreal length = gridRaster * 4;
        static constexpr qreal width = gridRaster * 4;
        static constexpr qreal margin = gridRaster;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawLine(length / 2 - margin / 2, 0, length / 2 - margin / 2, width);
            painter->drawLine(length / 2 + margin / 2, 0, length / 2 + margin / 2, width);
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, width / 2, margin * 3 / 2, width / 2);
            painter->drawLine(length - margin * 3 / 2, width / 2, length, width / 2);
        }

        public:
        Capacitor()
            : SchComponent(length, width, margin) {
        }

    };

    class NpnTransistor : public SchComponent {

        static constexpr qreal length = gridRaster * 8;
        static constexpr qreal width = gridRaster * 8;
        static constexpr qreal margin = gridRaster;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawLine(length / 2 - margin, margin * 2.5, length / 2 - margin, width - margin * 2.5);
            painter->drawEllipse(QPointF(length / 2, width / 2), length / 2 - margin, width / 2 - margin);

            painter->setPen(getConnectionPen());
            painter->drawLine(length / 2 - margin, width / 2, length - margin * 3, margin * 2);
            painter->drawLine(length / 2 - margin, width / 2, length - margin * 3, width - margin * 2);

            static const QPointF arrowPoints[] = { QPointF(length - margin * 3, width - margin * 2),
                                                   QPointF(length - margin * 3.75, width - margin * 3.25),
                                                   QPointF(length - margin * 4.25, width - margin * 2.75),
                                                };
            painter->setPen(getConnectionPen());
            painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
            painter->drawPolygon(arrowPoints, 3);
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, width / 2, length / 2 - margin, width / 2);
            painter->drawLine(length - margin * 3, width, length - margin * 3, width - margin * 2);
            painter->drawLine(length - margin * 3, 0, length - margin * 3, margin * 2);
        }

        public:
        NpnTransistor()
            : SchComponent(length, width, margin) {
        }

    };

}

#endif
