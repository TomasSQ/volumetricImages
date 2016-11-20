/********************************************************************************
** Form generated from reading UI file 'colorizedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORIZEDIALOG_H
#define UI_COLORIZEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ColorizeDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGraphicsView *graphicsView;
    QPushButton *button_apply_pallete;
    QPushButton *button_loadLabel;
    QRadioButton *radioButton_defaultPallete;
    QRadioButton *radioButton_VanillaRainbow;

    void setupUi(QDialog *ColorizeDialog)
    {
        if (ColorizeDialog->objectName().isEmpty())
            ColorizeDialog->setObjectName(QStringLiteral("ColorizeDialog"));
        ColorizeDialog->resize(526, 404);
        buttonBox = new QDialogButtonBox(ColorizeDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(130, 340, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        graphicsView = new QGraphicsView(ColorizeDialog);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(30, 20, 261, 261));
        button_apply_pallete = new QPushButton(ColorizeDialog);
        button_apply_pallete->setObjectName(QStringLiteral("button_apply_pallete"));
        button_apply_pallete->setGeometry(QRect(360, 150, 91, 21));
        button_loadLabel = new QPushButton(ColorizeDialog);
        button_loadLabel->setObjectName(QStringLiteral("button_loadLabel"));
        button_loadLabel->setGeometry(QRect(360, 180, 91, 21));
        radioButton_defaultPallete = new QRadioButton(ColorizeDialog);
        radioButton_defaultPallete->setObjectName(QStringLiteral("radioButton_defaultPallete"));
        radioButton_defaultPallete->setGeometry(QRect(360, 30, 131, 21));
        radioButton_defaultPallete->setChecked(true);
        radioButton_VanillaRainbow = new QRadioButton(ColorizeDialog);
        radioButton_VanillaRainbow->setObjectName(QStringLiteral("radioButton_VanillaRainbow"));
        radioButton_VanillaRainbow->setGeometry(QRect(360, 70, 131, 21));

        retranslateUi(ColorizeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ColorizeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ColorizeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ColorizeDialog);
    } // setupUi

    void retranslateUi(QDialog *ColorizeDialog)
    {
        ColorizeDialog->setWindowTitle(QApplication::translate("ColorizeDialog", "Dialog", 0));
        button_apply_pallete->setText(QApplication::translate("ColorizeDialog", "Apply Pallete", 0));
        button_loadLabel->setText(QApplication::translate("ColorizeDialog", "Load Label", 0));
        radioButton_defaultPallete->setText(QApplication::translate("ColorizeDialog", "Default Pallete", 0));
        radioButton_VanillaRainbow->setText(QApplication::translate("ColorizeDialog", "Vanilla Rainbow", 0));
    } // retranslateUi

};

namespace Ui {
    class ColorizeDialog: public Ui_ColorizeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORIZEDIALOG_H
