#ifndef GRAPHICS_COMPONENT
#define GRAPHICS_COMPONENT

#include <QGraphicsItem>
#include <sch_params.hpp>

namespace gui_qt {

    class SchGrid : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        bool m_gridEnable;

        public:
        SchGrid() {
            setPos(0, 0);
        }

        virtual QRectF boundingRect() const override {
            return QRectF(0, 0, L2P(g_logSchWidth), L2P(g_logSchHeight));
        }

        virtual void paint(QPainter* painter,
                    const QStyleOptionGraphicsItem *,
                    QWidget *) override {

            if(m_gridEnable) {
                painter->setPen(QPen(Qt::lightGray, 0));
                for(std::size_t i = 0; i <= g_logSchWidth; ++i) {
                    painter->drawLine(L2P(i), 0, L2P(i), L2P(g_logSchHeight));
                }

                for(std::size_t i = 0; i <= g_logSchHeight; ++i) {
                    painter->drawLine(0, L2P(i), L2P(g_logSchWidth), L2P(i));
                }
            }
        }

        void enableGrid(bool enable) {
            m_gridEnable = enable;
            update();
        }

        void toggleGrid() {
            m_gridEnable = !m_gridEnable;
            update();
        }

    };

}

#endif
