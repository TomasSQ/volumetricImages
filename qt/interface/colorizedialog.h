#ifndef COLORIZEDIALOG_H
#define COLORIZEDIALOG_H

#include <QDialog>
#include "libscn.hpp"
#include "libscnqt.h"
#include <QGraphicsScene>

namespace Ui {
class ColorizeDialog;
}

class ColorizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorizeDialog(QWidget *parent, Image3D &img, float percent);
    ~ColorizeDialog();
    QString getColorizeValue();
    Image3D getImgLabel();

private slots:
     void on_button_apply_pallete_clicked(bool);
     void on_button_loadLabel_clicked(bool);
     void radio_linear_toogle(bool);


private:
    void writeImage();
    Ui::ColorizeDialog *ui;
    Image3D &img, imgLabel;
    QGraphicsScene *scene;
    QPixmap pix;
    libscnqt libScnQt;
    float percent;
    bool colorize = false;
    QString colorizeValue = "nocolor";
};

#endif // COLORIZEDIALOG_H
