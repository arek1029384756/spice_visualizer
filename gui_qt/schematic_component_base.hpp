#ifndef SCHEMATIC_COMPONENT_BASE
#define SCHEMATIC_COMPONENT_BASE

#include <QGraphicsItem>
#include <tuple>
#include <map>
#include <cmath>
#include <schematic_params.hpp>

namespace gui_qt {

    class SchComponent : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        QRectF m_boundingRect;

        std::string m_name;
        std::string m_value;

        std::map<std::string, QPointF> m_termToLogItemPos;
        std::map<std::string, std::string> m_connToTerm;

        QRectF getComponentRect() const {
            return QRectF(0, 0, m_length, m_width);
        }

        void drawInfo(QPainter* painter) const {
            auto angle = rotation();
            auto vertical = std::abs(static_cast<std::int64_t>(angle)) % 180;
            auto align = (vertical) ? Qt::AlignVCenter | Qt::AlignLeft
                                    : Qt::AlignTop | Qt::AlignHCenter;
            auto offset = (vertical) ? QPointF((getL() + getW() - getM()) / 2, 0)
                                     : QPointF(0, getW() - getM() / 2);
            auto str = QString(m_name.c_str()) + QString("\n") + QString(m_value.c_str());

            painter->save();
            painter->translate(getComponentRect().center());
            painter->rotate(-angle);
            painter->translate(-getComponentRect().center());
            painter->translate(offset);
            painter->setFont(QFont("Courier New", 8, QFont::Normal));
            painter->drawText(getComponentRect(), align, str);
            painter->restore();
        }

        qreal getRotationAngle(const std::string& recomm, const QPointF& refTermLogItemPos) const {
            auto center = P2L(getComponentRect().center());
            auto refVec = refTermLogItemPos - center;
            auto signum = [](auto val) -> int { return (decltype(val)(0) < val) - (val < decltype(val)(0)); };

            auto key = std::make_tuple(signum(refVec.x()), signum(refVec.y()), recomm);
            auto it = rotationMap.find(key);
            if(it != rotationMap.end()) {
                return it->second;
            }

            throw std::runtime_error(std::string("GUI: angle key '<") +
                    std::to_string(std::get<0>(key)) + ", " +
                    std::to_string(std::get<1>(key)) + ", " +
                    std::get<2>(key) +
                    ">' for component '" +
                    m_name +
                    "' not found!");
        }

        void setComponentOrientation(const std::string& recomm, const std::string& refTermName) {
            auto termPos = getTermLogItemPos(refTermName);
            auto angle = getRotationAngle(recomm, termPos);
            setRotation(angle);
        }

        const std::string& getTermFromConn(const std::string& connection) const {
            auto it = m_connToTerm.find(connection);
            if(it != m_connToTerm.end()) {
                return it->second;
            }

            throw std::runtime_error(std::string("GUI: connection '") +
                    connection +
                    + "' for component '" +
                    m_name +
                    "' not found!");
        }

        const QPointF getTermLogItemPos(const std::string& term) const {
            auto it = m_termToLogItemPos.find(term);
            if(it != m_termToLogItemPos.end()) {
                return it->second;
            }

            throw std::runtime_error(std::string("GUI: terminal '") +
                    term +
                    + "' for component '" +
                    m_name +
                    "' not found!");
        }

        const QPointF getTermLogScenePos(const std::string& term) const {
            auto termPos = getTermLogItemPos(term);
            auto angle = rotation();
            return P2L(scenePos()) + rotateVector(termPos, angle);
        }

        static qreal bodyThick;
        static qreal termThick;

        inline static const auto rotateVector = [](auto vector, auto angle) {
            constexpr qreal pi = std::acos(-1);
            auto nangle = pi * angle / qreal(180);
            return decltype(vector)(std::round(vector.x() * std::cos(nangle) - vector.y() * std::sin(nangle)),
                                    std::round(vector.x() * std::sin(nangle) + vector.y() * std::cos(nangle)));
        };

        inline static const std::map<std::tuple<int, int, std::string>, qreal> rotationMap = {
            { { 1,  0, "RIGHT" },   0 }, { { 1,  0, "LEFT"  }, 180 }, { { 1,  0, "UP"    }, -90 }, { { 1,  0, "DOWN"  },  90 },
            { {-1,  0, "RIGHT" }, 180 }, { {-1,  0, "LEFT"  },   0 }, { {-1,  0, "UP"    },  90 }, { {-1,  0, "DOWN"  }, -90 },
            { { 1, -1, "RIGHT" },  90 }, { { 1, -1, "LEFT"  }, -90 }, { { 1, -1, "UP"    },   0 }, { { 1, -1, "DOWN"  }, 180 },
            { { 1,  1, "RIGHT" }, -90 }, { { 1,  1, "LEFT"  },  90 }, { { 1,  1, "UP"    }, 180 }, { { 1,  1, "DOWN"  },   0 },
            { {-1,  1, "RIGHT" }, 180 }, { {-1,  1, "LEFT"  },   0 }, { {-1,  1, "UP"    },  90 }, { {-1,  1, "DOWN"  }, -90 },
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

        static QPen getDrawingPen_T1() {
            return getTerminalPen();
        }

        public:
        static void setThickness(qreal thBody, qreal thTerm) {
            bodyThick = thBody;
            termThick = thTerm;
        }

        SchComponent(qreal logLength,
                    qreal logWidth,
                    qreal logMargin,
                    const std::string& recomm,
                    const std::string& refTermName,
                    const std::string& name,
                    const std::string& value,
                    const std::map<std::string, QPointF>& termToPos,
                    const std::map<std::string, std::string>& connToTerm)
            : m_length(L2P(logLength)),
            m_width(L2P(logWidth)),
            m_margin(L2P(logMargin)),
            m_boundingRect(getComponentRect()),
            m_name(name),
            m_value(value),
            m_termToLogItemPos(termToPos),
            m_connToTerm(connToTerm) {

            setComponentOrientation(recomm, refTermName);
        }

        virtual ~SchComponent() = default;

        void setComponentPos(const std::string& terminal, const QPointF& position) {
            auto termPos = getTermLogItemPos(terminal);
            auto angle = rotation();
            auto pos = position - rotateVector(termPos, angle);
            setPos(L2P(pos));
        }

        virtual QRectF boundingRect() const override {
            return m_boundingRect;
        }

        virtual void paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *,
                    QWidget *) override {
            painter->setPen(getBodyPen());
            drawBody(painter);

            painter->setPen(getTerminalPen());
            drawTerminals(painter);

            painter->setPen(Qt::black);
            drawInfo(painter);
        }

    };

}

#endif
