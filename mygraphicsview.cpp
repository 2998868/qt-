#include "mygraphicsview.h"
#include<QDebug>
mygraphicsview::mygraphicsview(QWidget *parent) : QGraphicsView(parent)
{

}

void mygraphicsview::mouseMoveEvent(QMouseEvent *e)
{
    emit mouseMove(e);
    QGraphicsView::mouseMoveEvent(e);

}

void mygraphicsview::mousePressEvent(QMouseEvent *e)
{

    emit mousePress(e);

    QGraphicsView::mousePressEvent(e);
}

void mygraphicsview::mouseReleaseEvent(QMouseEvent *e)
{

    emit mouseRelease(e);


    QGraphicsView::mouseReleaseEvent(e);
}

void mygraphicsview::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit mouseDoubleClick(e);
    QGraphicsView::mouseDoubleClickEvent(e);
}
