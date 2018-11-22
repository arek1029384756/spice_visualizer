#ifndef SCHEMATIC_COMPONENT_BASE
#define SCHEMATIC_COMPONENT_BASE

#include <QGraphicsItem>
#include <tuple>
#include <map>
#include <schematic_params.hpp>

namespace gui_qt {

    class SchComponent : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        std::string m_name;
        std::string m_value;

        std::map<std::string, QPointF> m_logTerminals;

        void drawInfo(QPainter* painter) const {
            auto angle = rotation();
            auto vertical = std::abs(static_cast<std::int64_t>(angle)) % 180;
            auto align = (vertical) ? Qt::AlignVCenter | Qt::AlignLeft
                                    : Qt::AlignTop | Qt::AlignHCenter;
            auto offset = (vertical) ? QPointF((getL() + getW() - getM()) / 2, 0)
                                     : QPointF(0, getW() - getM() / 2);
            auto str = QString(m_name.c_str()) + QString("\n") + QString(m_value.c_str());

            painter->save();
            painter->translate(boundingRect().center());
            painter->rotate(-angle);
            painter->translate(-boundingRect().center());
            painter->translate(offset);
            painter->setFont(QFont("Courier New", 8, QFont::Normal));
            painter->drawText(boundingRect(), align, str);
            painter->restore();
        }

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

        qreal getRotationAngle(const std::string& recomm, const QPointF& refTermLogItemPos) const {
            auto center = P2L(boundingRect().center());
            auto refVec = refTermLogItemPos - center;
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

        void setComponentPos(const std::string& recomm, const std::string& refTermName, const QPointF& refTermLogScenePos) {
            auto it = m_logTerminals.find(refTermName);
            if(it != m_logTerminals.end()) {
                auto itemPos = refTermLogScenePos - it->second;
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

        SchComponent(qreal logLength,
                    qreal logWidth,
                    qreal logMargin,
                    const std::string& name,
                    const std::string& value)
            : m_length(L2P(logLength)),
            m_width(L2P(logWidth)),
            m_margin(L2P(logMargin)),
            m_name(name),
            m_value(value) {
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

            painter->setPen(Qt::black);
            drawInfo(painter);
        }

    };

}

#endif
