#ifndef SCHCOMPONENT_H
#define SCHCOMPONENT_H

#include <QGraphicsItem>
#include <sch_params.hpp>

namespace gui_qt {

    class SchComponent : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        static qreal bodyThick;
        static qreal termThick;

        static QPen getBodyPen() {
            return QPen(Qt::black, L2P(bodyThick), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        static QPen getTerminalPen() {
            return QPen(Qt::black, L2P(termThick), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        protected:
        virtual void drawBody(QPainter* painter) const = 0;
        virtual void drawTerminals(QPainter* painter) const = 0;

        qreal getL() const {
            return m_length;
        }

        qreal getW() const {
            return m_width;
        }

        qreal getM() const {
            return m_margin;
        }

        static QPen getDrawingPen_T1() {
            return getTerminalPen();
        }

        public:
        static void setThickness(qreal thBody, qreal thTerm) {
            bodyThick = thBody;
            termThick = thTerm;
        }

        SchComponent(qreal logX, qreal logY, qreal logLength, qreal logWidth, qreal logMargin)
            : m_length(L2P(logLength)),
            m_width(L2P(logWidth)),
            m_margin(L2P(logMargin)) {

            setPos(L2P(logX), L2P(logY));
        }

        SchComponent(QPointF logPointA, QPointF logPointB, qreal logMargin)
            : SchComponent(logPointA.x(),
                    logPointA.y(),
                    std::abs((logPointB - logPointA).x()),
                    std::abs((logPointB - logPointA).y()),
                    logMargin) {
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

    class SchConnLine : public SchComponent {

        public:
        SchConnLine(QPointF logPointA, QPointF logPointB)
            : SchComponent(logPointA, logPointB, 0) {
        }

        protected:
        void drawBody(QPainter*) const override {
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, 0, getL(), getW());
        }

    };

    class Resistor : public SchComponent {

        static constexpr qreal logLength = 7;
        static constexpr qreal logWidth = 4;
        static constexpr qreal logMargin = 1;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawRect(getM(), getM(), getL() - getM() * 2, getW() - getM() * 2);
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, getW() / 2, getM(), getW() / 2);
            painter->drawLine(getL() - getM(), getW() / 2, getL(), getW() / 2);
        }

        public:
        Resistor(qreal logX, qreal logY)
            : SchComponent(logX, logY, logLength, logWidth, logMargin) {
        }

    };

    class Capacitor : public SchComponent {

        static constexpr qreal logLength = 4;
        static constexpr qreal logWidth = 4;
        static constexpr qreal logMargin = 1;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawLine(getL() / 2 - getM() / 2, 0, getL() / 2 - getM() / 2, getW());
            painter->drawLine(getL() / 2 + getM() / 2, 0, getL() / 2 + getM() / 2, getW());
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, getW() / 2, getM() * 3 / 2, getW() / 2);
            painter->drawLine(getL() - getM() * 3 / 2, getW() / 2, getL(), getW() / 2);
        }

        public:
        Capacitor(qreal logX, qreal logY)
            : SchComponent(logX, logY, logLength, logWidth, logMargin) {
        }

    };

    class NpnTransistor : public SchComponent {

        static constexpr qreal logLength = 8;
        static constexpr qreal logWidth = 8;
        static constexpr qreal logMargin = 1;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawLine(getL() / 2 - getM(), getM() * 2.5, getL() / 2 - getM(), getW() - getM() * 2.5);
            painter->drawEllipse(QPointF(getL() / 2, getW() / 2), getL() / 2 - getM(), getW() / 2 - getM());

            painter->setPen(getDrawingPen_T1());
            painter->drawLine(getL() / 2 - getM(), getW() / 2, getL() - getM() * 3, getM() * 2);
            painter->drawLine(getL() / 2 - getM(), getW() / 2, getL() - getM() * 3, getW() - getM() * 2);

            static const QPointF arrowPoints[] = { QPointF(getL() - getM() * 3, getW() - getM() * 2),
                                                   QPointF(getL() - getM() * 3.75, getW() - getM() * 3.25),
                                                   QPointF(getL() - getM() * 4.25, getW() - getM() * 2.75),
                                                };
            painter->setPen(getDrawingPen_T1());
            painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
            painter->drawPolygon(arrowPoints, 3);
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, getW() / 2, getL() / 2 - getM(), getW() / 2);
            painter->drawLine(getL() - getM() * 3, getW(), getL() - getM() * 3, getW() - getM() * 2);
            painter->drawLine(getL() - getM() * 3, 0, getL() - getM() * 3, getM() * 2);
        }

        public:
        NpnTransistor(qreal logX, qreal logY)
            : SchComponent(logX, logY, logLength, logWidth, logMargin) {
        }

    };

}

#endif
