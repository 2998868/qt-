 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QTime>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QLineF>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QFile>
#include <QPointF>
#include<QColorDialog>
#include <QBrush>
#include <QtCore>
#include<QColor>
#include <cmath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(-300,-200,600,400);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    itemInfo=new QLabel("itemInfo:");
    itemInfo->setMinimumWidth(150);
    ui->statusbar->addWidget(itemInfo);


    connect(ui->graphicsView,&mygraphicsview::mousePress,this,&MainWindow::mousePress_slot);
    connect(ui->graphicsView,&mygraphicsview::mouseDoubleClick,this,&MainWindow::mouseDoubleClick_slot);
    connect(ui->graphicsView,&mygraphicsview::mouseRelease,this,&MainWindow::mouseRelease_slot);
    //connect(ui->graphicsView,&mygraphicsview::mouseMove,this,&MainWindow::mouseMove_slot);



}

MainWindow::~MainWindow()
{
    delete ui;
}


template <class T>
void setBrush(T *item)
{//函数模板
    QColor color = item->brush().color();
    color = QColorDialog::getColor(color,NULL,"选择填充颜色");
    if (color.isValid())
        item->setBrush(QBrush(color));
}

//void MainWindow::save(const QRect &rect)
//{
//    saverect.append(rect);

//   QString filename=QFileDialog::getSaveFileName(this,tr("save paint"),"",tr("Text Files(*.txt)"));

//    QString filename=QString("新建文本文档1.txt");
//    if(!filename.isEmpty())
//    {
//        QFile file(filename);
//        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
//        {
//            QTextStream s(&file);
//            s<<"Rectangle: (" << rect.x() << ", " << rect.y() << "), Width: " << rect.width()
//            << ", Height: " << rect.height()<<"/n";
//            file.close();

//        }
//    }
//    else { // 如果文件打开失败，则显示错误消息框提示用户
//           QMessageBox::warning(this, tr("Error"), tr("Failed to save the image to %1").arg(filename));
//    }
//}

void MainWindow::mousePress_slot(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        // 绘图时
        //qDebug()<<"1";
        if ((cur_status == DrawLine
                || cur_status == DrawRect
                || cur_status == DrawEllipse
                || cur_status == DrawPolygon
                || cur_status == DrawCurve
                || cur_status == DrawPolyline)&&isDraw)
        {
            QGraphicsItem *select_item=scene->itemAt(e->pos(),QTransform());
            if(select_item){isDraw=false;}
            isSaved = false;
            startpoint = ui->graphicsView->mapToScene(e->pos());
            if(cur_status==DrawPolyline)
            {
                if(points.isEmpty()){
                group=new QGraphicsItemGroup;
                scene->addItem(group);
                }
                points.append(startpoint);
                if(points.size()>=2)
                {
                    for (int i = 0; i < points.size() - 1; i++) {
                        QGraphicsLineItem *line = new QGraphicsLineItem(points[i].x(), points[i].y(),
                                                                       points[i + 1].x(), points[i + 1].y());
                        group->setFlags(QGraphicsItem::ItemIsMovable
                                                   | QGraphicsItem::ItemIsSelectable
                                                   | QGraphicsItem::ItemIsFocusable);
                        scene->addItem(line);

                        group->addToGroup(qgraphicsitem_cast<QGraphicsItem*>(line));
                    }
                }
                if(e->button()==Qt::RightButton){
                    double len=0.0;
                    for(int i=0;i<points.size();i++){
                    len += qSqrt((points[i].x() - points[i + 1].x()*(points[i].x() - points[i + 1].x()))
                            + ((points[i].y() - points[i + 1].y())*(points[i].y() - points[i + 1].y())));
                    }

                    group->setData(areaORlen_key, QVariant(len));
                }

            }
            if(cur_status==DrawPolygon){
                if(points.isEmpty()){
                group=new QGraphicsItemGroup;
                scene->addItem(group);
                }
                points.append(startpoint);
                if(points.size()>=2)
                {
                    for (int i = 0; i < points.size() - 1; i++) {
                        QGraphicsLineItem *line = new QGraphicsLineItem(points[i].x(), points[i].y(),
                                                                       points[i + 1].x(), points[i + 1].y());
                        group->setFlags(QGraphicsItem::ItemIsMovable
                                                   | QGraphicsItem::ItemIsSelectable
                                                   | QGraphicsItem::ItemIsFocusable);
                        scene->addItem(line);

                        group->addToGroup(qgraphicsitem_cast<QGraphicsItem*>(line));
                    }

                }

            }
        }
        else if (cur_status == Select
                 || cur_status == ColorChange
                 || cur_status == Translate
                 || cur_status == Delete){
            QPointF pointScene=ui->graphicsView->mapToScene(e->pos());
            QGraphicsItem *item=nullptr;
            item=scene->itemAt(pointScene,ui->graphicsView->transform());
            if(cur_status==Translate){

                TranslateDialog *tranDialog=new TranslateDialog(this);


                connect(tranDialog,& TranslateDialog::translate_x_y,
                        [=](float x,float y){
                    item->setPos(QPointF(item->x()+x,item->y()+y));

                });
                tranDialog->exec();

            }
            if(cur_status==ColorChange){
                 if(item==nullptr){qDebug()<<"未选中item！";}
                 else{
                switch (item->type()) {
                case QGraphicsRectItem::Type:{


                    QGraphicsRectItem *theitem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
                    setBrush(theitem);
                    break;
                }
                case QGraphicsEllipseItem::Type: // 椭圆
                {
                  QGraphicsEllipseItem *theItem;
                  theItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                  setBrush(theItem);
                  break;
                }
                case QGraphicsPolygonItem::Type: //多边形
                {
                  QGraphicsPolygonItem *theItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
                  setBrush(theItem);
                  break;
                }
                case QGraphicsLineItem::Type: //直线，设置线条颜色
                {
                  QGraphicsLineItem *theItem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
                  QPen pen = theItem->pen();
                  QColor color = theItem->pen().color();
                  color = QColorDialog::getColor(color,this,"选择线条颜色");
                  if (color.isValid())
                  {
                      pen.setColor(color);
                      theItem->setPen(pen);
                  }


                  break;
                }
               case QGraphicsPathItem::Type:
               {
                    QGraphicsPathItem *theItem;
                    theItem = qgraphicsitem_cast<QGraphicsPathItem*>(item);
                    QColor color = QColorDialog::getColor(color,this,"选择线条颜色");
                    QPen pen = theItem->pen();
                    pen.setColor(color);
                    theItem->setPen(pen);
                    break;

               }
                default:{
                    qDebug()<<"未选中item！";
                }
          }
                 }
          scene->clearSelection();


                }
            if(cur_status==Delete)
            {
                scene->removeItem(item);
            }
        }
        }

    if(e->button()==Qt::RightButton)
    {

        if(cur_status==DrawPolygon)
        {
             QList<QGraphicsItem *> select_item=scene->selectedItems();
             for(int i=0;i<select_item.count();i++)
             {
             scene->removeItem(select_item[i]);
             }

            QGraphicsPolygonItem* item=new QGraphicsPolygonItem(QPolygonF(points));
            item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
            scene->addItem(item);
            double area=0.0;
               // 使用 Shoelace 公式计算面积
               for (int i = 0; i < points.size(); i++) {
                   double x1 = points[i].x();
                   double y1 = points[i].y();

                   double x2, y2;
                   if (i == points.size() - 1) {
                       x2 = points[0].x();
                       y2 = points[0].y();
                   } else {
                       x2 = points[i + 1].x();
                       y2 = points[i + 1].y();
                   }

                   area += (x1 * y2 - x2 * y1);
               }
               // 取绝对值并除以2得到最终面积
               area = abs(area) / 2.0;
               item->setData(areaORlen_key,area);
               item->setData(shape_key,"多边形");
            points.clear();
        }
        if(cur_status==DrawPolyline){
            QList<QGraphicsItem *> select_item=scene->selectedItems();
            for(int i=0;i<select_item.count();i++)
            {
            scene->removeItem(select_item[i]);
            }
            QPainterPath path;
                path.addPolygon(QPolygonF(points));
                double len=path.length();
                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                pathItem->setData(areaORlen_key,len);
                pathItem->setData(shape_key,"折线");

                // 将图形项添加到场景中
                scene->addItem(pathItem);
                points.clear();
//            for (int i = 0; i < points.size() - 1; i++) {
//                QGraphicsLineItem *line = new QGraphicsLineItem(points[i].x(), points[i].y(),
//                                                               points[i + 1].x(), points[i + 1].y());
//                group->setFlags(QGraphicsItem::ItemIsMovable
//                                           | QGraphicsItem::ItemIsSelectable
//                                           | QGraphicsItem::ItemIsFocusable);
//                scene->addItem(line);

//                group->addToGroup(qgraphicsitem_cast<QGraphicsItem*>(line));
//                double len=0.0;
//                len += qSqrt((points[i].x() - points[i + 1].x()*(points[i].x() - points[i + 1].x()))
//                  + ((points[i].y() - points[i + 1].y())*(points[i].y() - points[i + 1].y())));
//                group->setData(areaORlen_key,QVariant(len));
//            }
        }
//        if(cur_status==DrawPolyline){
//            double len=0.0;
//            for(int i=0;i<points.size();i++){
//            len += qSqrt((points[i].x() - points[i + 1].x()*(points[i].x() - points[i + 1].x()))
//                    + ((points[i].y() - points[i + 1].y())*(points[i].y() - points[i + 1].y())));
//            }

//            group->setData(areaORlen_key, QVariant(len));
//        }
    }
}


void MainWindow::mouseRelease_slot(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        // 获取鼠标释放时的坐标
        //qDebug()<<"1";
        endpoint = ui->graphicsView->mapToScene(e->pos());
        switch(cur_status) {
        case DrawLine:  // 绘制直线
        {
            QGraphicsLineItem *line_item = new QGraphicsLineItem(QLineF(startpoint, endpoint));//x,y 为左上角的图元局部坐标，图元中心点为0,0
            line_item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);

            QPen pen;
            pen.setWidth(4);
            // 计算长度
            double len = qSqrt((startpoint.x() - endpoint.x())*(startpoint.x() - endpoint.x()) +
                               (startpoint.y() - endpoint.y())*(startpoint.y() - endpoint.y()));
            line_item->setData(areaORlen_key, QVariant(len));
            line_item->setData(shape_key, "直线");
            scene->addItem(line_item);
            break;
        }
        case DrawRect:
        {
            QGraphicsRectItem* rect_item=new QGraphicsRectItem(QRectF(startpoint,endpoint));
            rect_item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
            scene->addItem(rect_item);
            double area=(startpoint.x() - endpoint.x())*(startpoint.y() - endpoint.y());
            rect_item->setData(areaORlen_key,QVariant(area));
            rect_item->setData(shape_key,"矩形");

            break;

        }
        case DrawEllipse:
        {
            QRectF rect(startpoint, endpoint);
            QGraphicsEllipseItem *Ellips_item=new QGraphicsEllipseItem(rect);

            Ellips_item->setFlags(QGraphicsItem::ItemIsFocusable|
                           QGraphicsItem::ItemIsSelectable|
                           QGraphicsItem::ItemIsMovable);
            scene->addItem(Ellips_item);
            double area=3.14159*0.25*(startpoint.x()-endpoint.x())*(startpoint.y()-endpoint.y());
            Ellips_item->setData(areaORlen_key,area);
            Ellips_item->setData(shape_key,"椭圆");

            break;

        }
        case DrawPolygon:
        {
            points.append(startpoint);
        }
        default:return ;
        }
    }

}

void MainWindow::mouseDoubleClick_slot(QMouseEvent *e)
{
    QPointF pointScene = ui->graphicsView->mapToScene(e->pos());
    QGraphicsItem *item = NULL;
    if(item==NULL){qDebug()<<"未选中item";}

    item = scene->itemAt(pointScene, ui->graphicsView->transform());

    if (item) {

        QString info =item->data(shape_key).toString()+ "面积或长度";
        QString str;
        if (item->group()) str = item->group()->data(areaORlen_key).toString();
        else str = item->data(areaORlen_key).toString();
        QMessageBox::information(this, info, str);
    }



}

void MainWindow::on_actionjuxing_triggered()
{
    cur_status=DrawRect;
    startpoint=endpoint=QPointF();
    isDraw=true;

}

void MainWindow::on_actiontuoyuan_triggered()
{
    cur_status=DrawEllipse;
    startpoint=endpoint=QPointF();
isDraw=true;

}

void MainWindow::on_actionxianduan_triggered()
{
    cur_status=DrawLine;
    startpoint=endpoint=QPointF();
    isDraw=true;

}

void MainWindow::on_actiongaise_triggered()
{
    cur_status=ColorChange;
}

void MainWindow::on_actionPolyline_triggered()
{
    cur_status=DrawPolyline;
    startpoint=endpoint=QPointF();
    isDraw=true;
}

void MainWindow::on_actionpolygon_triggered()
{
    cur_status=DrawPolygon;
    startpoint=endpoint=QPointF();
    isDraw=true;
}

void MainWindow::on_actionxingjian_triggered()
{
    QString curPath=QDir::currentPath();
    QString fileName=QFileDialog::getSaveFileName(this,tr("保存文件"),curPath,tr("*txt"));
    QFile file(fileName);
    if(!(file.open(QIODevice::WriteOnly | QIODevice::Truncate))){return;}
    else{
        QDataStream stream(&file);
        QList<QGraphicsItem *>	item_list = scene->items();
        // 保存数据
        for (int i = 0; i < item_list.size(); i++) {
            switch(item_list[i]->type())
            {
            case QGraphicsRectItem::Type: //矩形框
            {
                QGraphicsRectItem *theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->rect();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;

            }
            case QGraphicsEllipseItem::Type:
            {
                QGraphicsEllipseItem *theItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->rect();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;
            }
            case QGraphicsPolygonItem::Type:{
                QGraphicsPolygonItem *theItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->polygon();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;
            }
            case QGraphicsLineItem::Type:{
                QGraphicsLineItem *theItem = qgraphicsitem_cast<QGraphicsLineItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->line();
                stream << theItem->data(areaORlen_key).toDouble();
                stream << theItem->pen().color();
                break;
            }
            case QGraphicsPathItem::Type:{
                QGraphicsPathItem *theItem = qgraphicsitem_cast<QGraphicsPathItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->path();
                stream << theItem->data(areaORlen_key).toDouble();
                stream << theItem->pen().color();
                break;
            }

            }
        }

    }
    file.close();
}

void MainWindow::on_actionpingyi_triggered()
{
    cur_status=Translate;
}

void MainWindow::on_actiondakai_triggered()
{
    QString curPath=QDir::currentPath();
    QString fileName=QFileDialog::getOpenFileName(this,tr("保存文件"),curPath,tr("*txt"));
    QFile file(fileName);
    if (!(file.open(QIODevice::ReadOnly)))
    {return;}
    QDataStream stream(&file);
    scene->clear();
    while (!file.atEnd())
    {
        QString type;
        stream >> type;
        if (type == "矩形")
        {
            QRectF rect;
            stream >> rect;
            QGraphicsRectItem *rect_item = new QGraphicsRectItem(rect);
            rect_item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
            double area;
            stream >> area;
            rect_item->setData(areaORlen_key, QVariant(area));
            rect_item->setData(shape_key, "矩形");
            scene->addItem(rect_item);

            QColor color;
            stream >> color;
            if (color.isValid())
                rect_item->setBrush(QBrush(color));
        }
        else if (type == "椭圆")
        {
            QRectF rect;
            stream >> rect;
            QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect);
            item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
            double area;
            stream >> area;
            item->setData(areaORlen_key, area);
            item->setData(shape_key, "椭圆");
            scene->addItem(item);
            QColor color;
            stream >> color;
            if (color.isValid())
                item->setBrush(QBrush(color));
        }
        else if (type == "多边形")
        {
            QPolygonF polygon;
            stream >> polygon;
            QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon);
            item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
            double area;
            stream >> area;
            item->setData(areaORlen_key, area);
            item->setData(shape_key, "多边形");
            scene->addItem(item);
            QColor color;
            stream >> color;
            if (color.isValid())
                item->setBrush(QBrush(color));
        }
        else if (type == "直线")
        {
            QLineF line;
            stream >> line;
            QGraphicsLineItem *line_item = new QGraphicsLineItem(line);
            line_item->setFlags(QGraphicsItem::ItemIsMovable
                           | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);

            QPen pen;
            pen.setWidth(2);
            line_item->setPen(pen);
            double len;
            stream >> len;
            line_item->setData(areaORlen_key, QVariant(len));
            line_item->setData(shape_key, "直线");
            scene->addItem(line_item);
            QColor color;
            stream >> color;
            if (color.isValid())
                line_item->setPen(QPen(color));
        }
        else if (type == "折线") {
            QPainterPath path;
                stream >> path;
                QGraphicsPathItem *item = new QGraphicsPathItem;
                item->setPath(path);
                item->setFlags(QGraphicsItem::ItemIsMovable
                               | QGraphicsItem::ItemIsSelectable
                               | QGraphicsItem::ItemIsFocusable);
                double len;
                stream >> len;
                item->setData(areaORlen_key, len);
                item->setData(shape_key, "折线");
                scene->addItem(item);
                QColor color;
                stream >> color;
                if (color.isValid())
                    item->setPen(QPen(color));

        }
}
    file.close();
}

void MainWindow::on_actionxuanze_2_triggered()
{
    cur_status=Select;
}

void MainWindow::on_actionbaocun_triggered()
{
    QString curPath=QDir::currentPath();
    QString fileName=QFileDialog::getSaveFileName(this,tr("保存文件"),curPath,tr("*txt"));
    QFile file(fileName);
    if(!(file.open(QIODevice::Append|QIODevice::WriteOnly))){return;}
    else{
        QDataStream stream(&file);
        QList<QGraphicsItem *>	item_list = scene->items();
        // 保存数据
        for (int i = 0; i < item_list.size(); i++) {
            switch(item_list[i]->type())
            {
            case QGraphicsRectItem::Type: //矩形框
            {
                QGraphicsRectItem *theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->rect();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;

            }
            case QGraphicsEllipseItem::Type:
            {
                QGraphicsEllipseItem *theItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->rect();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;
            }
            case QGraphicsPolygonItem::Type:{
                QGraphicsPolygonItem *theItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->polygon();
                stream << theItem->data(areaORlen_key).toDouble();
                if (theItem->brush() != QBrush()) stream << theItem->brush().color();
                else stream << QColor();
                break;
            }
            case QGraphicsLineItem::Type:{
                QGraphicsLineItem *theItem = qgraphicsitem_cast<QGraphicsLineItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->line();
                stream << theItem->data(areaORlen_key).toDouble();
                stream << theItem->pen().color();
                break;
            }
            case QGraphicsPathItem::Type:{
                QGraphicsPathItem *theItem = qgraphicsitem_cast<QGraphicsPathItem*>(item_list[i]);
                stream << theItem->data(shape_key).toString();
                stream << theItem->path();
                stream << theItem->data(areaORlen_key).toDouble();
                stream << theItem->pen().color();
                break;
            }

            }
        }

    }
    file.close();
}

void MainWindow::on_actionDelete_triggered()
{
   cur_status=Delete;


}
