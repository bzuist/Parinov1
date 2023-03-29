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
    pictHeight = 390;
    pictWidth = 660;
    step = 1.0;
    leftX = -10; rightX = 10;
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
        ui->outputGraph->setPixmap(graph);
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
    ui->outputGraph->setPixmap(graph);
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

