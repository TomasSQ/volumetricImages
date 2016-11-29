/********************************************************************************
** Form generated from reading UI file 'rendering.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERING_H
#define UI_RENDERING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_rendering
{
public:
    QDialogButtonBox *buttonBox;
    QGraphicsView *graphicsView;
    QSlider *horizontalSlider_A;
    QSlider *horizontalSlider_B;
    QSlider *horizontalSlider_C;
    QRadioButton *radioButton_MIP;
    QRadioButton *radioButton_AGP;

    void setupUi(QDialog *rendering)
    {
        if (rendering->objectName().isEmpty())
            rendering->setObjectName(QStringLiteral("rendering"));
        rendering->resize(842, 534);
        buttonBox = new QDialogButtonBox(rendering);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(420, 390, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        graphicsView = new QGraphicsView(rendering);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(40, 20, 441, 311));
        horizontalSlider_A = new QSlider(rendering);
        horizontalSlider_A->setObjectName(QStringLiteral("horizontalSlider_A"));
        horizontalSlider_A->setGeometry(QRect(40, 350, 160, 16));
        horizontalSlider_A->setMaximum(360);
        horizontalSlider_A->setSingleStep(30);
        horizontalSlider_A->setOrientation(Qt::Horizontal);
        horizontalSlider_B = new QSlider(rendering);
        horizontalSlider_B->setObjectName(QStringLiteral("horizontalSlider_B"));
        horizontalSlider_B->setGeometry(QRect(40, 380, 160, 16));
        horizontalSlider_B->setMaximum(360);
        horizontalSlider_B->setSingleStep(30);
        horizontalSlider_B->setOrientation(Qt::Horizontal);
        horizontalSlider_C = new QSlider(rendering);
        horizontalSlider_C->setObjectName(QStringLiteral("horizontalSlider_C"));
        horizontalSlider_C->setGeometry(QRect(40, 420, 160, 16));
        horizontalSlider_C->setMaximum(360);
        horizontalSlider_C->setSingleStep(10);
        horizontalSlider_C->setValue(180);
        horizontalSlider_C->setOrientation(Qt::Horizontal);
        radioButton_MIP = new QRadioButton(rendering);
        radioButton_MIP->setObjectName(QStringLiteral("radioButton_MIP"));
        radioButton_MIP->setGeometry(QRect(590, 80, 100, 21));
        radioButton_MIP->setChecked(true);
        radioButton_AGP = new QRadioButton(rendering);
        radioButton_AGP->setObjectName(QStringLiteral("radioButton_AGP"));
        radioButton_AGP->setGeometry(QRect(590, 130, 171, 21));

        retranslateUi(rendering);
        QObject::connect(buttonBox, SIGNAL(accepted()), rendering, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), rendering, SLOT(reject()));

        QMetaObject::connectSlotsByName(rendering);
    } // setupUi

    void retranslateUi(QDialog *rendering)
    {
        rendering->setWindowTitle(QApplication::translate("rendering", "Rendering Window", 0));
        radioButton_MIP->setText(QApplication::translate("rendering", "MIP", 0));
        radioButton_AGP->setText(QApplication::translate("rendering", "Aggregate Projections", 0));
    } // retranslateUi

};

namespace Ui {
    class rendering: public Ui_rendering {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERING_H
