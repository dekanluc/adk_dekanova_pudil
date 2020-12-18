#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "triangle.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0;
    z_max = 500;
    dz = 10;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_7_clicked()
{
    //Create contour lines
    Algorithms a;
    std::vector<Edge> dt;

    z_min = ui -> lineEdit -> text().toDouble();
    z_max = ui -> lineEdit_2 -> text().toDouble();

    //DT needs to be created
    if(dt.size() == 0)
    {
        //Get points
        std::vector<QPoint3D> points = ui->Canvas->getPoints();

    if (points.empty())
    {
        ui->label_7->setText("Generate points please");
    }
    else
    {
        ui->label_7->setText("");
        //Create DT
        dt = a.DT(points);

        //Set DT
        ui->Canvas->setDT(dt);
    }
    }

    //Create contour lines
    std::vector<Edge> contours = a.contourLines(dt, z_min, z_max, dz);

    std::vector<Edge> main_contours = a.contourLines(dt, z_min, z_max, dz*5);
    //Set contours
    ui->Canvas->setContours(contours);
    ui->Canvas->setMainContours(main_contours);

    //Repaint
    repaint();
}


void Widget::on_pushButton_11_clicked()
{
    //Get points
    std::vector<QPoint3D> &points = ui->Canvas->getPoints();

    //Clear points
    points.clear();

    //Repaint
    repaint();
}


void Widget::on_pushButton_12_clicked()
{
    //Get DT and contours
    std::vector<Edge> &dt = ui->Canvas->getDT();
    std::vector<Edge> &contours = ui->Canvas->getContours();

    //Clear DT and contour lines
    dt.clear();
    contours.clear();

    //Repaint
    repaint();
}


void Widget::on_lineEdit_editingFinished()
{
    //Set z_min
    z_min = ui -> lineEdit -> text().toDouble();
}


void Widget::on_lineEdit_2_editingFinished()
{
    //Set z_max
    z_max = ui -> lineEdit_2 -> text().toDouble();
}


void Widget::on_lineEdit_3_editingFinished()
{
    //Set dz
    dz = ui -> lineEdit_3 -> text().toDouble();
}


void Widget::on_pushButton_2_clicked()
{
    Algorithms a;
    std::vector<Edge> dt;

    //DT needs to be created

    if(dt.size() == 0)
    {
        //Get points
        std::vector<QPoint3D> points = ui->Canvas->getPoints();

    if (points.empty())
        {
        ui->label_7->setText("Generate points please");
        }
    else
        {
        ui->label_7->setText("");
        //Create DT
        dt = a.DT(points);

        //Set DT
        ui->Canvas->setDT(dt);
        }
    }

    //Initialize boolean variables
    bool slope = FALSE;
    bool aspect = FALSE;
    bool panchromatic = FALSE;
    bool colorful = FALSE;

    //Analyze DTM
    std::vector<Triangle> dtm = a.analyzeDTM(dt);

    //Set analysis
    ui->Canvas->setDTM(dtm);

    //Show selected analyze
    if (ui->comboBox->currentIndex()==0)
    {
        slope = TRUE;
        aspect = FALSE;
    }
    else if (ui->comboBox->currentIndex()==1){
        slope = FALSE;
        aspect = TRUE;
    }

    //select color
    if (ui->comboBox_2->currentIndex()==0){
        panchromatic = TRUE;
        colorful = FALSE;
    }
    else if (ui->comboBox_2->currentIndex()==1){
        panchromatic = FALSE;
        colorful = TRUE;
    }

    ui->Canvas->setAspect(aspect);
    ui->Canvas->setSlope(slope);
    ui->Canvas->setPanchromatic(panchromatic);
    ui->Canvas->setColorful(colorful);

    //Repaint
    repaint();
}


void Widget::on_pushButton_3_clicked()
{
    //Clear DTM analysis
    std::vector<Edge> &dt = ui->Canvas->getDT();
    std::vector<Triangle> &dtm = ui->Canvas->getDTM();

    //Clear DTM
    dt.clear();
    dtm.clear();

    //Repaint
    repaint();
}


void Widget::on_pushButton_4_clicked()
{
    //Get all
    std::vector<QPoint3D> &points = ui->Canvas->getPoints();
    std::vector<Edge> &dt = ui->Canvas->getDT();
    std::vector<Edge> &contours = ui->Canvas->getContours();
    std::vector<Triangle> &dtm = ui->Canvas->getDTM();
    std::vector<Edge> &maincontours = ui->Canvas->getMainContours();
    std::vector<Edge> &labels = ui->Canvas->getLabelContours();
    //Clear all
    points.clear();
    dt.clear();
    contours.clear();
    dtm.clear();
    maincontours.clear();
    labels.clear();
    //Repaint
    repaint();
}

void Widget::on_pushButton_clicked()
{
    std::vector<QPoint3D> points;
    QSizeF canvas_size = ui->Canvas->size();
    int h=canvas_size.height();
    int w=canvas_size.width();
    QString path(QFileDialog::getOpenFileName(this,
    tr("Open file with points"),
    "../", tr("Text Files (*.txt)")));
    std::string path_std = path.toStdString();
    ui->Canvas->loadFile(path_std,points,h,w,z_min,z_max);
    ui->Canvas->setPoints(points);
    ui->Canvas->repaint();

}

void Widget::on_pushButton_5_clicked()
{
   std::vector<Edge> label_contours = ui -> Canvas -> getMainContours();
   if (label_contours.empty())
       {
       ui->label_7->setText("Generate points please");
       }
   else
       {
       ui->label_7->setText("");
       ui->Canvas->setLabelContours(label_contours);
       repaint();
       }
}



void Widget::on_pushButton_6_clicked()
{
    repaint();
    n = ui->lineEdit_4->text().toDouble();
    QSize size = ui->Canvas->size();
    int height = size.height();
    int width = size.width();
    Algorithms alg;
    std::vector<QPoint3D> points;

    if (ui->comboBox_3->currentIndex() == 0)
        points = alg.generateStack(n, height, width);
    else if(ui->comboBox_3->currentIndex() == 1)
        points = alg.generateRidge(n, height, width);
    else if(ui->comboBox_3->currentIndex() == 2)
        points = alg.generateValley(n, height, width);
    else if(ui->comboBox_3->currentIndex() == 3)
        points = alg.generateSettling(n, height, width);

    ui->Canvas->setPoints(points);
    repaint();
}

void Widget::on_lineEdit_4_editingFinished()
{
    n = ui->lineEdit_4->text().toDouble();
}
