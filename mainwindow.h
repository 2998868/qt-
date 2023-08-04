#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QMessageBox>
#include <QGraphicsView>
#include <QLabel>
#include <QVector>
#include <QGraphicsItemGroup>
#include "translatedialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Status {
             Select,
             ColorChange,
             Translate,
             Delete,
             DrawLine,
             DrawRect,
             DrawEllipse,
             DrawPolygon,
             DrawCurve,
             DrawPolyline,


           };
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Status cur_status;

    QPointF startpoint;
    QPointF endpoint;
    bool draw;
    QList<QRect> saverect;
    QGraphicsScene *scene;
    QLabel *itemInfo;
    QVector<QPointF> points;
    QGraphicsItemGroup* group;
    bool isDraw=true;

  //  QPolygonF polyline;


    const int areaORlen_key = 1;  // 图形项长度/面积关键字
    const int shape_key = 2;  // 图形项类型关键字

    int group_id = 0;
    const int group_id_key = 3;  // 组合id关键字

    bool isSaved; // 是否保存

    // QWidget interface
protected:


    // QWidget interface
//protected:
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);



    void save(const QRect &rect);

private slots:

    void mousePress_slot(QMouseEvent *e);
    void mouseRelease_slot(QMouseEvent *e);
    void mouseDoubleClick_slot(QMouseEvent *e);

    void on_actionjuxing_triggered();
    void on_actiontuoyuan_triggered();
    void on_actionxianduan_triggered();
    void on_actiongaise_triggered();
    void on_actionPolyline_triggered();
    void on_actionpolygon_triggered();
    void on_actionxingjian_triggered();
    void on_actionpingyi_triggered();
    void on_actiondakai_triggered();
    void on_actionxuanze_2_triggered();
    void on_actionbaocun_triggered();
    void on_actionDelete_triggered();
};
#endif // MAINWINDOW_H
