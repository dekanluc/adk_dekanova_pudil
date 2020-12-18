#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    int n;
    double z_min, z_max, dz;



public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_lineEdit_4_editingFinished();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
