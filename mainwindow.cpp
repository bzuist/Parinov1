#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QDebug>
#include <QStaticText>
using namespace std;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Косметическая подготовка приложения
      this->resize(960,660);          // Устанавливаем размеры окна приложения
      this->setFixedSize(960,660);

    scene = new QGraphicsScene();   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(890,430);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(0,0,600,300); // Устанавливаем размер сцены

    pictHeight = 400;
    pictWidth = 950;
    step = 1.0;
    leftX = -5; rightX = 5;
    leftY = -10; rightY = 10;
    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getData()
{
    a = ui->lineEdit->text().toDouble();
    b = ui->lineEdit_2->text().toDouble();
    c = ui->lineEdit_3->text().toDouble();
    d = ui->lineEdit_4->text().toDouble();

    k = ui->lineEdit_5->text().toDouble();

    leftX = ui->lineEdit_6->text().toDouble();
    rightX = ui->lineEdit_7->text().toDouble();

    leftY = ui->lineEdit_8->text().toDouble();
    rightY = ui->lineEdit_9->text().toDouble();

    pictWidth = ui->outputGraph->width();
    pictHeight = ui->outputGraph->height();

}

double MainWindow::f(double x)
{
    getData();
    return (a*pow(x,3) + b*pow(x,2) + c*x + d)*(-1);
}

void MainWindow::recountPixels()
{
    onePixelX = (pictWidth-10)*k/(rightX-leftX);
    onePixelY = (pictHeight-10)*k/(rightY-leftY);
    Ox = fabs(rightX); Oy = rightY;
}

void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(pictWidth*k,pictHeight*k);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0, pictWidth*k,pictHeight*k);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight*k);
    paint.drawLine(0,Oy*onePixelY,pictWidth*k,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));

    QFont font = paint.font();
          font.setPixelSize(12);
          paint.setFont(font);

    for(double i = leftX;i<=rightX;i+=step)
    {
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
        // Координаты оси X
        QString str = QString::number(i);
        paint.drawStaticText((i+Ox)*onePixelX-3,Oy*onePixelY, QStaticText(str));
    }
    for(double i = leftY;i<=rightY;i+=step)
    {
        paint.drawPoint(Ox*onePixelX,(i+Oy)*onePixelY);
        // Координаты оси Y
        QString str = QString::number(i);
        paint.drawStaticText(Ox*onePixelX+5,(i+Oy)*onePixelY-5, QStaticText(str));
}

    if(!notEmpty) {
        paint.end();

        scene->clear();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(graph); // Создаём графический элемент
        item->setFlag(QGraphicsItem::ItemIsMovable); // делаем элемент перемещаемым
        item->setFlag(QGraphicsItem::ItemIsSelectable);//выделяем элемент, который перемещаем
        scene->addItem(item);   // Добавляем элемент на графическую сцену
        return;
    }

    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    bool first[4] = {1,1};

    for(double i = leftX;i<=rightX;i+=step) {
        if(!isnan(f(i))) {
            if(first[0]) {
                path.moveTo((i+Ox)*onePixelX,(f(i)+Oy)*onePixelY);

                first[0] = false;
                    }
        else
            path.lineTo((i+Ox)*onePixelX,(f(i)+Oy)*onePixelY);
       }

    }
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.setRenderHint(QPainter::SmoothPixmapTransform, true);
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.setPen(QPen(Qt::blue,2,Qt::SolidLine));
    paint.drawPath(path);
    paint.end();

    scene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(graph); // Создаём графический элемент
    item->setFlag(QGraphicsItem::ItemIsMovable); // делаем элемент перемещаемым
    item->setFlag(QGraphicsItem::ItemIsSelectable);//выделяем элемент, который перемещаем
    scene->addItem(item);   // Добавляем элемент на графическую сцену
    item->setPos(0, 0);   // Устанавливаем позицию элемента
    return;
}

/* Нажатие кнопки "Построить график" */
void MainWindow::on_pushButton_clicked()
{
   getData();
   recountPixels();
   drawGraph(1);

}

/* Нажатие кнопки "Очистить" */
void MainWindow::on_pushButton_2_clicked()
{
   recountPixels();
   drawGraph();
}

/* Нажатие кнопки "Закрыть" */
void MainWindow::on_pushButton_3_clicked()
{
 this->close();
}

