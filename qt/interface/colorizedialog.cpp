#include "colorizedialog.h"
#include "ui_colorizedialog.h"
#include <iostream>
#include <QFileDialog>

ColorizeDialog::ColorizeDialog(QWidget *parent, Image3D &img, float percent) :
    QDialog(parent),
    ui(new Ui::ColorizeDialog),
    scene(new QGraphicsScene(this)),
    percent(percent),
    img(img)
{
    ui->setupUi(this);
//    this->img = img;
//    this->percent = percent;
//    this->scene =  new QGraphicsScene(this);
    connect(ui->radioButton_defaultPallete, SIGNAL(toggled(bool)), this, SLOT(radio_linear_toogle(bool)));
    connect(ui->radioButton_VanillaRainbow, SIGNAL(toggled(bool)), this, SLOT(radio_linear_toogle(bool)));
    writeImage();

}


Image3D ColorizeDialog::getImgLabel(){
    return this->imgLabel;
}

ColorizeDialog::~ColorizeDialog()
{
    delete scene;
    delete ui;
}


void ColorizeDialog::on_button_loadLabel_clicked(bool t){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   "/home",
                                                   tr("Image Label (*.scn)"));

    if(!fileName.isEmpty()){
        ImgFormat::detect::read(imgLabel,fileName.toStdString());
        img.colorize(imgLabel);
        ui->button_apply_pallete->setEnabled(false);
        this->colorizeValue = "label";
        writeImage();

    }
}

void ColorizeDialog::on_button_apply_pallete_clicked(bool t){
    radio_linear_toogle(false);
}

void ColorizeDialog::radio_linear_toogle(bool t){
    colorize = true;
    writeImage();
}

QString ColorizeDialog::getColorizeValue(){
    return colorizeValue;
}

void ColorizeDialog::writeImage(){

    Image3D cut;
    img.ort_plane(cut, 0, percent, 0, 1, 1);
    if(colorize){
        if(ui->radioButton_defaultPallete->isChecked()){
            colorizeValue = "default";
            cut.apply_pallete();
        } else {
            colorizeValue = "hsl2rgb";
            cut.apply_pallete_over_HSL2RGB_space();
        }
    }

    QByteArray qBA;
    libScnQt.generateInMemoryImg(cut, &qBA);

    pix.loadFromData(qBA, "PPM");
    scene->setSceneRect(pix.rect());
    scene->clear();
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();
}
