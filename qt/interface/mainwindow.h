#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "colorizedialog.h"
#include "libscn.hpp"
#include "libscnqt.h"
#include "rendering.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fill_with_slices(void*, float percent = -1);


public slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_openAction(bool);
    void on_radio_linear_toogle(bool);
    void on_horizontalSlider_transformationsB_valueChanged(int);
    void on_horizontalSlider_transformationsA_valueChanged(int);
    void on_horizontalSlider_sliderMoved(int);
    void on_buttonColorize_clicked(bool);
    void on_buttonRendering_clicked(bool);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene,*scene2,*scene3;
    QPixmap pix, pix2, pix3;
    libscnqt libScnQt;
    Image3D img, imgLabel;
    Voxel v_max, v_min;
    bool linear_transform = false;
    QString colorize = "nocolor";
    int I1,I2,k1,k2;
    ColorizeDialog *c;
    rendering *r;
    QByteArray qBA;



};

#endif // MAINWINDOW_H
