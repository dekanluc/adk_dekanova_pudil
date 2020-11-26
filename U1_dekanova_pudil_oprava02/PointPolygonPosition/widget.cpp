#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "draw.h"
#include <QFileDialog>
#include <QFile>
#include <fstream>
using namespace std;
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
    //Change drawing mode
    ui->Canvas->changeMode();
}

void Widget::on_pushButton_2_clicked()
{
        QPointF q = ui->Canvas->getPoint();
        vector<QPolygonF> polygons = ui->Canvas->getPolygons();

        vector<int> result;
        Algorithms alg;
        vector<QPointF> pol;
        QPolygonF polygon;

        int res=0;
        int l = 0;
   //getting a current polygon
   for ( unsigned int i = 0; i < polygons.size(); i++)
        {

       polygon = polygons[i];

       for (int  j= 0; j< polygon.size(); j++)
            {

           pol.push_back(polygon[j]);

            }

        for (unsigned int k = 0; k < pol.size(); k++)
            {
           if (q == pol[k])
               l = l + 1;
            }

           if (ui->comboBox->currentIndex() == 0)
           {
               res = alg.getPositionWinding(q,pol);
           }

           else
            {
               res = alg.getPositionRay(q,pol);
            }

               result.push_back(res); //adding the result to the vector


   int K=0;
   for(unsigned int a = 0; a < result.size(); a++)
               {
                if (result[a] == -1)

                     K = -5;

                else if (result[a] == 1)
                {
                     K = K + 1;
                }
                else
                     K = K + 0;
                }

                //setting label due to the point position
            if (K > 0)
            ui->label->setText("Inside");

            else if (K == 0)
            ui->label->setText("Outside");

            else
            ui->label->setText("On the edge");

            pol.clear();
            l=0;
        }
            ui->Canvas->setResult(result);
            ui->Canvas->repaint();

}


void Widget::on_pushButton_3_clicked()
{
        ui->Canvas->erasePoints();
}

void Widget::on_load_file_clicked()
{
    QString path(QFileDialog::getOpenFileName
                 (this, tr("Opening of file"),
                  "../",
                  tr("Text Files (*.txt)")));
       string path_ = path.toStdString();

       ui->Canvas->txtFile(path_);
}


