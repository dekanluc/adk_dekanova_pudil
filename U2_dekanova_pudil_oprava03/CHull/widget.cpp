#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "draw.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //Get points
    QPolygon points = ui->Canvas->getPoints();
    if (points.empty())
        {
        ui->label_2->setText("Generate Points please");
        }
    else {
        ui->label_2->setText("");
    //Create Convex hull
    QPolygon ch;
    Algorithms alg;

    QElapsedTimer timer;
    timer.start();
    //Jarvis Scan
    if (ui->comboBox->currentIndex()==0)
        ch = alg.jarvis(points);

    //QHull
    else if (ui->comboBox->currentIndex()==1)
        ch = alg.qhull(points);

    //Sweep line
    else
        ch = alg.sweepLine(points);

    //Set Convex hull
    ui -> Canvas -> setCH(ch);

    //how long does the function take
    ui -> label -> setText(QString::number(timer.elapsed()) + " ms");

    //Repaint screen
    repaint();
    }
}

void Widget::on_pushButton_2_clicked()
{
    //Get Convex Hull
    QPolygon &ch = ui->Canvas->getCH();

    //Clear points
    ch.clear();

    //Repaint screen
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    QSize size = ui -> Canvas -> size();
    int height = size.height();
    int width = size.width();
    QPolygon generatedpoints;
    int n = ui->lineEdit->text().toInt();

    if (ui -> comboBox_2 -> currentIndex() == 0)
        generatedpoints = ui -> Canvas -> circle(width, height, n);
    if (ui -> comboBox_2 -> currentIndex() == 1)
        generatedpoints = ui -> Canvas -> randompoints(width, height, n);
    if (ui -> comboBox_2 -> currentIndex() == 2)
        generatedpoints = ui -> Canvas -> ellips(width, height, n);
    if (ui -> comboBox_2 -> currentIndex() == 3)
        generatedpoints = ui -> Canvas -> grid(width, height, n);
    if (ui -> comboBox_2 -> currentIndex() == 5)
        generatedpoints = ui -> Canvas -> square(width, height, n);
    if (ui -> comboBox_2 -> currentIndex() == 4)
        generatedpoints = ui -> Canvas -> star(width, height, n);

    ui->Canvas->setPoints(generatedpoints);
    ui->Canvas->repaint();
}

void Widget::on_pushButton_3_clicked()
{
    //Get polygon
    QPolygon &polygon = ui->Canvas->getPolygon();

    //Clear polygon
    polygon.clear();

    //Repaint screen
    ui->Canvas->setPoints(polygon);
    repaint();
}
