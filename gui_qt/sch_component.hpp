#ifndef SCH_COMPONENT
#define SCH_COMPONENT

#include <QGraphicsItem>
#include <tuple>
#include <map>
#include <sch_params.hpp>

namespace gui_qt {

    class SchComponent : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        std::map<std::string, QPointF> m_logTerminals;

        static qreal bodyThick;
        static qreal termThick;

        inline static const std::map<std::tuple<int, int, std::string>, qreal> rotationMap = {
            { { 1,  0, "RIGHT"  },   0 },
            { { 1,  0, "LEFT"   }, 180 },
            { { 1,  0, "UP"     }, -90 },
            { { 1,  0, "DOWN"   },  90 },
            { {-1,  0, "RIGHT"  }, 180 },
            { {-1,  0, "LEFT"   },   0 },
            { {-1,  0, "UP"     },  90 },
            { {-1,  0, "DOWN"   }, -90 },
        };

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

        void addTerminal(const std::string& name, const QPointF& logPoint) {
            m_logTerminals.emplace(name, logPoint);
        }

        qreal getRotationAngle(const std::string& recomm, const QPointF& termLogPos) const {
            auto center = P2L(boundingRect().center());
            auto refVec = termLogPos - center;
            auto signum = [](auto val) -> int { return (decltype(val)(0) < val) - (val < decltype(val)(0)); };

            auto key = std::make_tuple(signum(refVec.x()), signum(refVec.y()), recomm);
            auto it = rotationMap.find(key);
            if(it != rotationMap.end()) {
                return it->second;
            }

            throw std::runtime_error(std::string("GUI angle key '<") +
                    std::to_string(std::get<0>(key)) + ", " +
                    std::to_string(std::get<1>(key)) + ", " +
                    std::get<2>(key) +
                    ">' not found!");
        }

        void setComponentPos(const std::string& recomm, const std::string& refTermName, const QPointF& refTermLogPos) {
            auto it = m_logTerminals.find(refTermName);
            if(it != m_logTerminals.end()) {
                auto itemPos = refTermLogPos - it->second;
                auto angle = getRotationAngle(recomm, it->second);
                setTransformOriginPoint(L2P(it->second));
                setPos(L2P(itemPos));
                setRotation(angle);
            } else {
                throw std::runtime_error(std::string("GUI component terminal '") +
                        refTermName +
                        "' not found!");
            }
        }

        static QPen getDrawingPen_T1() {
            return getTerminalPen();
        }

        public:
        static void setThickness(qreal thBody, qreal thTerm) {
            bodyThick = thBody;
            termThick = thTerm;
        }

        SchComponent(qreal logLength, qreal logWidth, qreal logMargin)
            : m_length(L2P(logLength)),
            m_width(L2P(logWidth)),
            m_margin(L2P(logMargin)) {
        }

        virtual ~SchComponent() = default;

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
        Resistor(const QPointF& pos, const std::string& recomm, const std::string& refTerm)
            : SchComponent(logLength, logWidth, logMargin) {
            addTerminal("1", QPointF(0, logWidth / 2));
            addTerminal("2", QPointF(logLength, logWidth / 2));
            setComponentPos(recomm, refTerm, pos);
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
        Capacitor(const QPointF& pos, const std::string& recomm, const std::string& refTerm)
            : SchComponent(logLength, logWidth, logMargin) {
            addTerminal("1", QPointF(0, logWidth / 2));
            addTerminal("2", QPointF(logLength, logWidth / 2));
            setComponentPos(recomm, refTerm, pos);
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
        NpnTransistor(const QPointF& pos, const std::string& recomm, const std::string& refTerm)
            : SchComponent(logLength, logWidth, logMargin) {
            //1 - col, 2 - bas, 3 - emi
            addTerminal("1", QPointF(logLength - logMargin * 3, 0));
            addTerminal("2", QPointF(0, logWidth / 2));
            addTerminal("3", QPointF(logLength - logMargin * 3, logWidth));
            setComponentPos(recomm, refTerm, pos);
        }

    };

}

#endif
