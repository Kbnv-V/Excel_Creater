/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *patch_file;
    QPushButton *select_file;
    QComboBox *list;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QTextEdit *logs;
    QPushButton *start;
    QLineEdit *column;
    QLabel *label_4;
    QSpinBox *start_row;
    QSpinBox *end_row;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(792, 459);
        QFont font;
        font.setBold(false);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        patch_file = new QLineEdit(centralwidget);
        patch_file->setObjectName("patch_file");
        patch_file->setGeometry(QRect(50, 40, 381, 31));
        select_file = new QPushButton(centralwidget);
        select_file->setObjectName("select_file");
        select_file->setGeometry(QRect(50, 80, 171, 31));
        list = new QComboBox(centralwidget);
        list->addItem(QString());
        list->addItem(QString());
        list->setObjectName("list");
        list->setGeometry(QRect(50, 160, 231, 31));
        list->setDuplicatesEnabled(false);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 140, 231, 20));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 280, 131, 20));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(220, 280, 131, 20));
        logs = new QTextEdit(centralwidget);
        logs->setObjectName("logs");
        logs->setGeometry(QRect(470, 30, 301, 371));
        start = new QPushButton(centralwidget);
        start->setObjectName("start");
        start->setGeometry(QRect(50, 360, 171, 31));
        QFont font1;
        font1.setBold(true);
        start->setFont(font1);
        start->setCheckable(false);
        column = new QLineEdit(centralwidget);
        column->setObjectName("column");
        column->setGeometry(QRect(50, 240, 113, 28));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 220, 63, 20));
        start_row = new QSpinBox(centralwidget);
        start_row->setObjectName("start_row");
        start_row->setGeometry(QRect(50, 300, 121, 29));
        end_row = new QSpinBox(centralwidget);
        end_row->setObjectName("end_row");
        end_row->setGeometry(QRect(220, 300, 121, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 792, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Excel Corrector", nullptr));
        select_file->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\204\320\260\320\271\320\273", nullptr));
        list->setItemText(0, QCoreApplication::translate("MainWindow", "Email", nullptr));
        list->setItemText(1, QCoreApplication::translate("MainWindow", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275\320\275\321\213\320\271 \320\275\320\276\320\274\320\265\321\200", nullptr));

        list->setCurrentText(QString());
        list->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\321\203\320\265\320\274\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\321\203\320\265\320\274\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\320\265\321\207\320\275\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        start->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\321\203", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\276\320\275\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
