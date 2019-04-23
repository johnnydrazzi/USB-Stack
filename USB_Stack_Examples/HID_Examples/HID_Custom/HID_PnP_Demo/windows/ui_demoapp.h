/********************************************************************************
** Form generated from reading UI file 'demoapp.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEMOAPP_H
#define UI_DEMOAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DemoApp
{
public:
    QWidget *centralWidget;
    QProgressBar *progressBar;
    QPushButton *pushButton;
    QLabel *pushbuttonStatus;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *deviceConnectedStatus;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DemoApp)
    {
        if (DemoApp->objectName().isEmpty())
            DemoApp->setObjectName(QString::fromUtf8("DemoApp"));
        DemoApp->resize(461, 165);
        centralWidget = new QWidget(DemoApp);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setGeometry(QRect(20, 100, 421, 21));
        progressBar->setMaximum(1024);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(20, 50, 111, 31));
        pushbuttonStatus = new QLabel(centralWidget);
        pushbuttonStatus->setObjectName(QString::fromUtf8("pushbuttonStatus"));
        pushbuttonStatus->setEnabled(false);
        pushbuttonStatus->setGeometry(QRect(210, 50, 231, 20));
        pushbuttonStatus->setCursor(QCursor(Qt::ArrowCursor));
        pushbuttonStatus->setInputMethodHints(Qt::ImhNone);
        pushbuttonStatus->setFrameShadow(QFrame::Plain);
        pushbuttonStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setEnabled(false);
        label_2->setGeometry(QRect(40, 80, 401, 20));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(true);
        label_3->setGeometry(QRect(410, 10, 41, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        deviceConnectedStatus = new QLineEdit(centralWidget);
        deviceConnectedStatus->setObjectName(QString::fromUtf8("deviceConnectedStatus"));
        deviceConnectedStatus->setEnabled(true);
        deviceConnectedStatus->setGeometry(QRect(20, 10, 391, 21));
        deviceConnectedStatus->setReadOnly(true);
        DemoApp->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DemoApp);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 461, 22));
        DemoApp->setMenuBar(menuBar);
        statusBar = new QStatusBar(DemoApp);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DemoApp->setStatusBar(statusBar);

        retranslateUi(DemoApp);

        QMetaObject::connectSlotsByName(DemoApp);
    } // setupUi

    void retranslateUi(QMainWindow *DemoApp)
    {
        DemoApp->setWindowTitle(QApplication::translate("DemoApp", "DemoApp", nullptr));
        pushButton->setText(QApplication::translate("DemoApp", "ToggleLED(s)", nullptr));
        pushbuttonStatus->setText(QApplication::translate("DemoApp", "Pushbutton State: Unknown", nullptr));
        label_2->setText(QApplication::translate("DemoApp", "ANx/POT Voltage", nullptr));
        label_3->setText(QApplication::translate("DemoApp", "Status", nullptr));
        deviceConnectedStatus->setText(QApplication::translate("DemoApp", "Device Not Detected: Verify Connection/Correct Firmware", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DemoApp: public Ui_DemoApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEMOAPP_H
