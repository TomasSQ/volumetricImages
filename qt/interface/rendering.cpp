#include "rendering.h"
#include "ui_rendering.h"

#include "libscnqt.h"
#include <iostream>

rendering::rendering(QWidget *parent, Image3D &img) :
    QDialog(parent),
      ui(new Ui::rendering),
      scene(new QGraphicsScene(this)),
    img(img)
{
    ui->setupUi(this);

}

rendering::~rendering()
{
    delete scene;
    delete ui;
}


void rendering::on_horizontalSlider_A_valueChanged(int v){
    writeImage();
}


void rendering::on_horizontalSlider_B_valueChanged(int v){
    writeImage();

}
void rendering::on_horizontalSlider_C_valueChanged(int v){
    writeImage();

}


void rendering::writeImage(){

    tmR1.rotation(0,ui->horizontalSlider_A->value());
    tmR2.rotation(1,ui->horizontalSlider_B->value());
    tmR3.rotation(2,ui->horizontalSlider_C->value());
    tm = tmR3*tmR2*tmR1;
    img.MIP(MIP,tm);


    libScnQt.generateInMemoryImg(MIP, &qBA);

    pix.loadFromData(qBA, "PPM");
    scene->clear();
    scene->setSceneRect(pix.rect());

    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();


}

