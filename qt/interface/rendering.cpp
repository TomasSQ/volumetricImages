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

    ui->graphicsView->setMouseTracking(true);
    connect(ui->radioButton_AGP, SIGNAL(toggled(bool)), this, SLOT(radio_linear_toogle(bool)));
    connect(ui->radioButton_MIP, SIGNAL(toggled(bool)), this, SLOT(radio_linear_toogle(bool)));
    connect(ui->radioButton_Phong, SIGNAL(toggled(bool)), this, SLOT(radio_linear_toogle(bool)));

    writeImage(true);


}

rendering::~rendering()
{
    delete scene;
    delete ui;
}



void rendering::radio_linear_toogle(bool v){
    writeImage(true);
}

void rendering::on_horizontalSlider_A_valueChanged(int v){
    writeImage(false);
}


void rendering::on_horizontalSlider_B_valueChanged(int v){
    writeImage(false);

}
void rendering::on_horizontalSlider_C_valueChanged(int v){
    writeImage(false);

}

void rendering::on_horizontalSlider_A_sliderReleased(){
    writeImage(true);
}

void rendering::on_horizontalSlider_B_sliderReleased(){
    writeImage(true);
}

void rendering::on_horizontalSlider_C_sliderReleased(){
    writeImage(true);
}

void rendering::writeImage(bool real){

    tmR1.rotation(0,ui->horizontalSlider_A->value());
    tmR2.rotation(1,ui->horizontalSlider_B->value());
    tmR3.rotation(2,ui->horizontalSlider_C->value());
    tm = tmR3*tmR2*tmR1;

    Image3D b;
    if(!real){
        img.project_quick2(b, tm);

    }else{
        if(ui->radioButton_AGP->isChecked()){
            img.aggregate_projections(b,tm);
        } else if(ui->radioButton_MIP->isChecked()) {
            img.MIP(b,tm);
        }else{
            img.project_aphong(b, tm);
        }
    }

    libScnQt.generateInMemoryImgParallel(b, &qBA);

    pix.loadFromData(qBA, "PPM");
    scene->clear();
    scene->setSceneRect(pix.rect());

    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();


}

