/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionNormalization;
    QAction *actionBanana;
    QWidget *centralWidget;
    QSlider *horizontalSlider;
    QGraphicsView *graphicsView_3;
    QGraphicsView *graphicsView_2;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QRadioButton *radioButton_std;
    QRadioButton *radioButton_threshold;
    QRadioButton *radioButton_negative;
    QRadioButton *radioButton_Normalize;
    QSlider *horizontalSlider_transformationsA;
    QSlider *horizontalSlider_transformationsB;
    QPushButton *buttonColorize;
    QPushButton *buttonRendering;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1038, 659);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionNormalization = new QAction(MainWindow);
        actionNormalization->setObjectName(QStringLiteral("actionNormalization"));
        actionBanana = new QAction(MainWindow);
        actionBanana->setObjectName(QStringLiteral("actionBanana"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(30, 360, 160, 16));
        horizontalSlider->setMouseTracking(false);
        horizontalSlider->setSingleStep(1);
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksBelow);
        graphicsView_3 = new QGraphicsView(centralWidget);
        graphicsView_3->setObjectName(QStringLiteral("graphicsView_3"));
        graphicsView_3->setGeometry(QRect(290, 270, 271, 261));
        graphicsView_2 = new QGraphicsView(centralWidget);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(290, 0, 271, 261));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 0, 271, 261));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(false);
        groupBox->setGeometry(QRect(600, 10, 241, 301));
        radioButton_std = new QRadioButton(groupBox);
        radioButton_std->setObjectName(QStringLiteral("radioButton_std"));
        radioButton_std->setGeometry(QRect(40, 40, 100, 21));
        radioButton_std->setChecked(true);
        radioButton_threshold = new QRadioButton(groupBox);
        radioButton_threshold->setObjectName(QStringLiteral("radioButton_threshold"));
        radioButton_threshold->setGeometry(QRect(40, 70, 100, 21));
        radioButton_negative = new QRadioButton(groupBox);
        radioButton_negative->setObjectName(QStringLiteral("radioButton_negative"));
        radioButton_negative->setGeometry(QRect(40, 100, 100, 21));
        radioButton_Normalize = new QRadioButton(groupBox);
        radioButton_Normalize->setObjectName(QStringLiteral("radioButton_Normalize"));
        radioButton_Normalize->setGeometry(QRect(40, 130, 100, 21));
        horizontalSlider_transformationsA = new QSlider(groupBox);
        horizontalSlider_transformationsA->setObjectName(QStringLiteral("horizontalSlider_transformationsA"));
        horizontalSlider_transformationsA->setEnabled(false);
        horizontalSlider_transformationsA->setGeometry(QRect(50, 190, 160, 16));
        horizontalSlider_transformationsA->setMaximum(100);
        horizontalSlider_transformationsA->setOrientation(Qt::Horizontal);
        horizontalSlider_transformationsA->setTickPosition(QSlider::NoTicks);
        horizontalSlider_transformationsB = new QSlider(groupBox);
        horizontalSlider_transformationsB->setObjectName(QStringLiteral("horizontalSlider_transformationsB"));
        horizontalSlider_transformationsB->setEnabled(false);
        horizontalSlider_transformationsB->setGeometry(QRect(50, 220, 160, 16));
        horizontalSlider_transformationsB->setMaximum(100);
        horizontalSlider_transformationsB->setOrientation(Qt::Horizontal);
        buttonColorize = new QPushButton(centralWidget);
        buttonColorize->setObjectName(QStringLiteral("buttonColorize"));
        buttonColorize->setEnabled(false);
        buttonColorize->setGeometry(QRect(670, 330, 80, 23));
        buttonRendering = new QPushButton(centralWidget);
        buttonRendering->setObjectName(QStringLiteral("buttonRendering"));
        buttonRendering->setGeometry(QRect(670, 370, 80, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1038, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Volumetric Images Visualizer", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionNormalization->setText(QApplication::translate("MainWindow", "Normalization", 0));
        actionBanana->setText(QApplication::translate("MainWindow", "banana", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Transformations", 0));
        radioButton_std->setText(QApplication::translate("MainWindow", "Standard", 0));
        radioButton_threshold->setText(QApplication::translate("MainWindow", "Threshold", 0));
        radioButton_negative->setText(QApplication::translate("MainWindow", "Negative", 0));
        radioButton_Normalize->setText(QApplication::translate("MainWindow", "Normalize", 0));
        buttonColorize->setText(QApplication::translate("MainWindow", "Colorize", 0));
        buttonRendering->setText(QApplication::translate("MainWindow", "Rendering", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
