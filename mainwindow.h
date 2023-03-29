#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /* Рисуем */
    void drawGraph(bool notEmpty = 0);
    /* Функция получения данных */
    void getData();
    /* Функция уравнения */
    double f(double x);
    /* Функция пересчитывания пикселей */
    void recountPixels();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::MainWindow *ui;
    double leftX,rightX;
    double leftY,rightY;
    int pictWidth,pictHeight;
    double step; /* Шаг */

    double onePixelX,onePixelY;
    double Ox,Oy;
    double height, width;

    double a, b, c, d; // Коэффициенты уравнения
    double k; // Коэффициент масштабирования
};
#endif // MAINWINDOW_H
