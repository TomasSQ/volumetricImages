#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libscn.cpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QFileDialog>

Image3D imgOriginal;
Image3D img;
Voxel v_max, v_min;


void generateInMemoryImg(Image3D& img, QByteArray* qtBA){
    QDataStream qDS(qtBA, QIODevice::ReadWrite);
    char buf[1000];
    /* write magic string */
    qDS.writeRawData((char*)"P6\n", 3);
    /* write nx,ny */
    int x = sprintf(buf, "%d %d\n",img.n[1], img.n[0]);
    qDS.writeRawData((char*) buf, x);
    qDS.writeRawData((char*)"255\n", 4);
    /* write data */
    for(int ix=0;ix<img.n[0];++ix){
    for(int iy=0;iy<img.n[1];++iy){
        int iz=0;                 {
            int nc=img.data[ix][iy][iz].value.size();
            for(int ic=0;ic<3;++ic){
                char c[1];
                c[0]=img.data[ix][iy][iz].value[ic%nc][0].to_ulong();
                qDS.writeRawData((char*) c, 1);
            }

    }}}
}

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

}

void MainWindow::on_radio_linear_toogle(bool t){
    if (ui->radioButton_std->isChecked()){
        fill_with_slides(&imgOriginal, ui->horizontalSlider->value()/100.0);
        img = imgOriginal;
        return;
    }
    int I1,I2,k1,k2;

    int Imin = v_min.to_ulong();
    int Imax = v_max.to_ulong();


    if(ui->radioButton_negative->isChecked()){
        I1=Imin;I2=Imax;k1=Imax;k2=Imin;
    } else if (ui->radioButton_threshold){
        I1= ui->horizontalSlider_transformationsA->value();I2= ui->horizontalSlider_transformationsB->value();k1= 000;k2= 255;
    } else {
         I1=Imin;I2=Imax;k1= ui->horizontalSlider_transformationsA->value();k2= ui->horizontalSlider_transformationsB->value();
    }
    std::cout << "TESTEEEEE" << std::endl;
    std::cout.flush();
    img = imgOriginal;
    img.linear_transform(I1,I2,k1,k2);
    fill_with_slides(&img, ui->horizontalSlider->value()/100.0);
}

void MainWindow::on_openAction(bool s){
     ImgFormat::detect::read(img,QFileDialog::getOpenFileName().toStdString());
     imgOriginal = img;
     scene = new QGraphicsScene(this);
     scene2 = new QGraphicsScene(this);
     scene3 = new QGraphicsScene(this);
     fill_with_slides(&img, -1);
     v_max = img.top(std::less<Voxel>());
     v_min = img.top(std::greater<Voxel>());
}


MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete scene2;
    delete scene3;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    fill_with_slides(&img, value/100.0);
}

void MainWindow::on_horizontalSlider_transformationsB_valueChanged(int value)
{

    on_radio_linear_toogle(false);
}

void MainWindow::on_horizontalSlider_transformationsA_valueChanged(int value)
{

     on_radio_linear_toogle(false);
}


void MainWindow::fill_with_slides(void* v, float percent){
    Image3D *img = (Image3D*) v;
    int value1 = -1, value2 = -1, value3 = -1;
//    std::cout << percent << std::endl;
    if (percent >= 0){
        value1 = img->get_n(0) * percent;
        value2 = img->get_n(1) * percent;
        value3 = img->get_n(2) * percent;
//        std::cout << value1 <<"  " << value2 << " " << value3 << std::endl;
    }
    Image3D cut, cut2, cut3;
    img->ort_plane(cut, 0, value1, 0, 1, 1);
    img->ort_plane(cut2, 1,value2, 0, 1,1);
    img->ort_plane(cut3, 2,value3, 0, 0,1);

    QByteArray qBA;
    generateInMemoryImg(cut, &qBA);

    pix.loadFromData(qBA, "PPM");
    scene->setSceneRect(pix.rect());
    scene->clear();
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();

    qBA.clear();
    generateInMemoryImg(cut2, &qBA);
    pix.loadFromData(qBA, "PPM");
    scene2->clear();
    scene2->addPixmap(pix);
    scene2->setSceneRect(pix.rect());
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->update();

    qBA.clear();
    generateInMemoryImg(cut3, &qBA);
    pix.loadFromData(qBA, "PPM");
    scene3->clear();
    scene3->addPixmap(pix);
    scene3->setSceneRect(pix.rect());
    ui->graphicsView_3->setScene(scene3);
    ui->graphicsView_3->update();
}
