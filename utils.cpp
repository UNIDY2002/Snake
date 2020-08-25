#include "utils.h"

void paintRect(QPainter &painter, const Point &p, const Qt::GlobalColor &color) {
    painter.fillRect(QRect(p.x * WIDTH, p.y * HEIGHT, WIDTH, HEIGHT), color);
}
