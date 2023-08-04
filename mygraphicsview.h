#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
class mygraphicsview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit mygraphicsview(QWidget *parent = nullptr);
    //void set_cur_status(STATUS s) { status = s; }

private:
    void mouseMoveEvent(QMouseEvent *e)Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e)Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e)Q_DECL_OVERRIDE;

signals:
    void mouseMove(QMouseEvent *e); //鼠标移动
    void mousePress(QMouseEvent *e); //鼠标单击
    void mouseRelease(QMouseEvent *e);
    void mouseDoubleClick(QMouseEvent *e); //双击事件
    void keyPress(QKeyEvent *e); //按键事件


};

#endif // MYGRAPHICSVIEW_H
