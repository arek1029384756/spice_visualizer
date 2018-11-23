#ifndef SCHEMATIC_COMPONENT
#define SCHEMATIC_COMPONENT

#include <schematic_component_base.hpp>

namespace gui_qt {

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
        Resistor(const std::string& recomm,
                const std::string& refTerm,
                const std::string& name,
                const std::string& value)
            : SchComponent(logLength,
                    logWidth,
                    logMargin,
                    recomm,
                    refTerm,
                    name,
                    value,
                    {
                        { "1", QPointF(0, logWidth / 2) },
                        { "2", QPointF(logLength, logWidth / 2) }
                    },
                    {
                        //tmp - this would be taken from graph traversal
                        { "net1", "1" },
                        { "net2", "2" }
                    }) {
        }

        virtual ~Resistor() = default;

    };

    class Capacitor : public SchComponent {

        static constexpr qreal logLength = 4;
        static constexpr qreal logWidth = 6;
        static constexpr qreal logMargin = 1;

        protected:
        void drawBody(QPainter* painter) const override {
            painter->drawLine(getL() / 2 - getM() / 2, getM(), getL() / 2 - getM() / 2, getW() - getM());
            painter->drawLine(getL() / 2 + getM() / 2, getM(), getL() / 2 + getM() / 2, getW() - getM());
        }

        void drawTerminals(QPainter* painter) const override {
            painter->drawLine(0, getW() / 2, getM() * 3 / 2, getW() / 2);
            painter->drawLine(getL() - getM() * 3 / 2, getW() / 2, getL(), getW() / 2);
        }

        public:
        Capacitor(const std::string& recomm,
                const std::string& refTerm,
                const std::string& name,
                const std::string& value)
            : SchComponent(logLength,
                    logWidth,
                    logMargin,
                    recomm,
                    refTerm,
                    name,
                    value,
                    {
                        { "1", QPointF(0, logWidth / 2) },
                        { "2", QPointF(logLength, logWidth / 2) }
                    },
                    {
                        //tmp - this would be taken from graph traversal
                        { "net1", "1" },
                        { "net2", "2" }
                    }) {
        }

        virtual ~Capacitor() = default;
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
        NpnTransistor(const std::string& recomm,
                const std::string& refTerm,
                const std::string& name,
                const std::string& value)
            : SchComponent(logLength,
                    logWidth,
                    logMargin,
                    recomm,
                    refTerm,
                    name,
                    value,
                    {
                        //1 - col, 2 - bas, 3 - emi (NGSPICE documentation)
                        { "1", QPointF(logLength - logMargin * 3, 0) },
                        { "2", QPointF(0, logWidth / 2) },
                        { "3", QPointF(logLength - logMargin * 3, logWidth) }
                    },
                    {
                        //tmp - this would be taken from graph traversal
                        { "net1", "1" },
                        { "net2", "2" },
                        { "net3", "3" }
                    }) {
        }

        virtual ~NpnTransistor() = default;
    };

}

#endif
