/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "lyricpage.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QListWidget *listWidget;
    QPushButton *addFileBtn;
    QPushButton *clearBtn;
    QPushButton *playBtn;
    QSlider *sliderVolume;
    QSlider *sliderPosition;
    QPushButton *soundBtn;
    QPushButton *removeBtn;
    QPushButton *loopBtn;
    QLabel *timeLabel;
    QPushButton *previousBtn;
    QPushButton *nextBtn;
    LyricPage *lyricDisplayWidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(689, 402);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(390, 70, 231, 181));
        addFileBtn = new QPushButton(centralwidget);
        addFileBtn->setObjectName("addFileBtn");
        addFileBtn->setGeometry(QRect(400, 280, 75, 24));
        clearBtn = new QPushButton(centralwidget);
        clearBtn->setObjectName("clearBtn");
        clearBtn->setGeometry(QRect(500, 280, 75, 24));
        playBtn = new QPushButton(centralwidget);
        playBtn->setObjectName("playBtn");
        playBtn->setGeometry(QRect(240, 320, 75, 24));
        sliderVolume = new QSlider(centralwidget);
        sliderVolume->setObjectName("sliderVolume");
        sliderVolume->setGeometry(QRect(360, 90, 16, 160));
        sliderVolume->setValue(50);
        sliderVolume->setOrientation(Qt::Orientation::Vertical);
        sliderPosition = new QSlider(centralwidget);
        sliderPosition->setObjectName("sliderPosition");
        sliderPosition->setEnabled(true);
        sliderPosition->setGeometry(QRect(70, 280, 160, 16));
        sliderPosition->setMaximum(100000);
        sliderPosition->setSliderPosition(1);
        sliderPosition->setOrientation(Qt::Orientation::Horizontal);
        sliderPosition->setInvertedAppearance(false);
        sliderPosition->setInvertedControls(false);
        soundBtn = new QPushButton(centralwidget);
        soundBtn->setObjectName("soundBtn");
        soundBtn->setGeometry(QRect(350, 280, 31, 21));
        removeBtn = new QPushButton(centralwidget);
        removeBtn->setObjectName("removeBtn");
        removeBtn->setGeometry(QRect(400, 320, 75, 24));
        loopBtn = new QPushButton(centralwidget);
        loopBtn->setObjectName("loopBtn");
        loopBtn->setGeometry(QRect(80, 320, 75, 24));
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setGeometry(QRect(260, 280, 101, 20));
        previousBtn = new QPushButton(centralwidget);
        previousBtn->setObjectName("previousBtn");
        previousBtn->setGeometry(QRect(160, 320, 75, 24));
        nextBtn = new QPushButton(centralwidget);
        nextBtn->setObjectName("nextBtn");
        nextBtn->setGeometry(QRect(320, 320, 75, 24));
        lyricDisplayWidget = new LyricPage(centralwidget);
        lyricDisplayWidget->setObjectName("lyricDisplayWidget");
        lyricDisplayWidget->setGeometry(QRect(80, 80, 261, 181));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        addFileBtn->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\226\207\344\273\266", nullptr));
        clearBtn->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\210\227\350\241\250\346\255\214\346\233\262", nullptr));
        playBtn->setText(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276", nullptr));
        soundBtn->setText(QCoreApplication::translate("MainWindow", "\351\235\231\351\237\263", nullptr));
        removeBtn->setText(QCoreApplication::translate("MainWindow", "\347\247\273\351\231\244", nullptr));
        loopBtn->setText(QCoreApplication::translate("MainWindow", "\345\276\252\347\216\257\346\222\255\346\224\276", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00\357\274\23200/00:00", nullptr));
        previousBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\351\246\226", nullptr));
        nextBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\351\246\226", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
