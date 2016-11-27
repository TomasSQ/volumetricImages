#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libscn.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QFileDialog>
#include "rendering.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_openAction(bool)));
    connect(ui->radioButton_negative, SIGNAL(toggled(bool)), this, SLOT(on_radio_linear_toogle(bool)));
    connect(ui->radioButton_Normalize, SIGNAL(toggled(bool)), this, SLOT(on_radio_linear_toogle(bool)));
    connect(ui->radioButton_std, SIGNAL(toggled(bool)), this, SLOT(on_radio_linear_toogle(bool)));
    connect(ui->radioButton_threshold, SIGNAL(toggled(bool)), this, SLOT(on_radio_linear_toogle(bool)));
    ui->horizontalSlider_transformationsA->setVisible(false);
    ui->horizontalSlider_transformationsB->setVisible(false);
    ui->horizontalSlider->setEnabled(false);

}

void MainWindow::on_radio_linear_toogle(bool t){
    if (ui->radioButton_std->isChecked()){
        linear_transform = false;
        fill_with_slices(&img, ui->horizontalSlider->value()/100.0);
        ui->horizontalSlider_transformationsA->setEnabled(false);
        ui->horizontalSlider_transformationsB->setEnabled(false);
         ui->horizontalSlider_transformationsA->setVisible(false);
        ui->horizontalSlider_transformationsB->setVisible(false);
        return;
    }

    linear_transform = true;
    int Imin = v_min.to_ulong();
    int Imax = v_max.to_ulong();


    if(ui->radioButton_negative->isChecked()){
        ui->horizontalSlider_transformationsA->setVisible(false);
        ui->horizontalSlider_transformationsB->setVisible(false);
        ui->horizontalSlider_transformationsA->setEnabled(false);
        ui->horizontalSlider_transformationsB->setEnabled(false);
        I1=Imin;I2=Imax;k1=Imax;k2=Imin;
    } else if (ui->radioButton_threshold->isChecked()){
        ui->horizontalSlider_transformationsA->setVisible(true);
        ui->horizontalSlider_transformationsB->setVisible(false);
        ui->horizontalSlider_transformationsA->setMaximum(255);
        I1= ui->horizontalSlider_transformationsA->value();I2= ui->horizontalSlider_transformationsA->value();k1= 000;k2= 255;
        ui->horizontalSlider_transformationsA->setEnabled(true);
        ui->horizontalSlider_transformationsB->setEnabled(false);
    } else {
        ui->horizontalSlider_transformationsA->setVisible(true);
        ui->horizontalSlider_transformationsB->setVisible(true);
        ui->horizontalSlider_transformationsA->setMaximum(100);
        ui->horizontalSlider_transformationsB->setMaximum(100);
        ui->horizontalSlider_transformationsA->setEnabled(true);
        ui->horizontalSlider_transformationsB->setEnabled(true);
        int b = 100 - ui->horizontalSlider_transformationsA->value();
        int c = 100 - ui->horizontalSlider_transformationsB->value();
        b = (b/100.0) * 255; c= (c / 100.00) * 255;
        I1= (2*b - c)/2;
        I2=b + I1;
        k1= 000;k2= 255;
    }
    fill_with_slices(&img, ui->horizontalSlider->value()/100.0);
}

void MainWindow::on_openAction(bool s){
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Scene Images (*.scn)"));
     if(!fileName.isEmpty()){
         ImgFormat::detect::read(img,fileName.toStdString());
         ui->groupBox->setEnabled(true);
         ui->horizontalSlider->setEnabled(true);
         ui->buttonColorize->setEnabled(true);
         scene = new QGraphicsScene(this);
         scene2 = new QGraphicsScene(this);
         scene3 = new QGraphicsScene(this);
         fill_with_slices(&img, -1);
         v_max = img.top(std::less<Voxel>());
         v_min = img.top(std::greater<Voxel>());
     }
}


MainWindow::~MainWindow()
{
    delete scene;
    delete scene2;
    delete scene3;
    delete ui;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    fill_with_slices(&img, value/100.0);
}

void MainWindow::on_horizontalSlider_transformationsB_valueChanged(int value)
{

    on_radio_linear_toogle(false);
}

void MainWindow::on_horizontalSlider_transformationsA_valueChanged(int value)
{

     on_radio_linear_toogle(false);
}


void MainWindow::on_buttonColorize_clicked(bool b){
    float value1 = img.get_n(0) * (ui->horizontalSlider->value()/100.0);
    c = new ColorizeDialog(this, img, value1);
    if (c->exec()) {
        colorize = c->getColorizeValue();
        if(colorize == "label"){
            this->imgLabel = c->getImgLabel();
        }
    }

    delete c;
    fill_with_slices(&img, ui->horizontalSlider->value()/100.0);
}


void MainWindow::on_buttonRendering_clicked(bool b){

    r = new rendering(this,img);
    r->exec();
    delete r;
}

void MainWindow::fill_with_slices(void* v, float percent){
    Image3D *img = (Image3D*) v;
    int value1 = -1, value2 = -1, value3 = -1;
    if (percent >= 0){
        value1 = img->get_n(0) * percent;
        value2 = img->get_n(1) * percent;
        value3 = img->get_n(2) * percent;
    }
    Image3D cut, cut2, cut3;
    img->ort_plane(cut, 0, value1, 0, 1, 1);
    img->ort_plane(cut2, 1,value2, 0, 1,1);
    img->ort_plane(cut3, 2,value3, 0, 0,1);

    if(linear_transform){
        cut.linear_transform(I1,I2,k1,k2);
        cut2.linear_transform(I1,I2,k1,k2);
        cut3.linear_transform(I1,I2,k1,k2);
    }


    if (colorize != "nocolor"){
        if(colorize == "default"){
            cut.apply_pallete();
            cut2.apply_pallete();
            cut3.apply_pallete();
        } else if(colorize == "hsl2rgb") {
            cut.apply_pallete_over_HSL2RGB_space();
            cut2.apply_pallete_over_HSL2RGB_space();
            cut3.apply_pallete_over_HSL2RGB_space();
        } else{
            Image3D labelCut, labelCut2, labelCut3;
            imgLabel.ort_plane(labelCut, 0, value1, 0, 1, 1);
            imgLabel.ort_plane(labelCut2, 1,value2, 0, 1,1);
            imgLabel.ort_plane(labelCut3, 2,value3, 0, 0,1);
            cut.colorize(labelCut);
            cut2.colorize(labelCut2);
            cut3.colorize(labelCut3);

        }
    }

    libScnQt.generateInMemoryImg(cut, &qBA);

    pix.loadFromData(qBA, "PPM");
    scene->setSceneRect(pix.rect());
    scene->clear();
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();

    qBA.clear();
    libScnQt.generateInMemoryImg(cut2, &qBA);
    pix2.loadFromData(qBA, "PPM");
    scene2->clear();
    scene2->addPixmap(pix2);
    scene2->setSceneRect(pix2.rect());
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->update();

    qBA.clear();
    libScnQt.generateInMemoryImg(cut3, &qBA);
    pix3.loadFromData(qBA, "PPM");
    scene3->clear();
    scene3->addPixmap(pix3);
    scene3->setSceneRect(pix3.rect());
    ui->graphicsView_3->setScene(scene3);
    ui->graphicsView_3->update();
}
