#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "iostream"
#include "fstream"
#include "iostream"
#include "ostream"
#include "qplaintextedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SourceFile();
    void Clang_Compile();
    int Clang_Errors();
    void Clang_Intermediate_Representation();
    void Gdb_Disassembler();
    void CreateGdbCmds();
    void Run();
    void Thread();
    void Output();
    void KeyPressEvent(QKeyEvent *event) ;

private slots:

    void on_plainTextEdit_Code_cursorPositionChanged();

    void on_plainTextEdit_Code_textChanged();

    void on_toolButton_ForLoop_pressed();

private:
    Ui::MainWindow *ui;
    std::ifstream FileIn;
    std::ofstream FileOut;
    std::string theString;
    QTimer *myTimer=new QTimer();
    double myTime;
};
#endif // MAINWINDOW_H
