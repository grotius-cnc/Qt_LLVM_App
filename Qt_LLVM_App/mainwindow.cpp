#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(myTimer, &QTimer::timeout, this, &MainWindow::Thread);
    myTimer->start(1000);

    CreateGdbCmds();
    Run();

    ui->plainTextEdit_Code->setLineNumberEnabled(1);
    ui->plainTextEdit_Error->setLineNumberEnabled(1);
    ui->plainTextEdit_Gdb->setLineNumberEnabled(1);
    ui->plainTextEdit_Ir->setLineNumberEnabled(1);
    ui->plainTextEdit_Output->setLineNumberEnabled(1);
    ui->plainTextEdit_UserDefined->setLineNumberEnabled(1);

    // QFont aFont;
    // aFont.setPointSizeF(8.0);
    // ui->plainTextEdit_Code->setFont(aFont);
    // ~/Qt_LLVM_App/qmarkdowntextedit/markdownhighlighter.h Line 39 to set text height.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Thread()
{
    if(ui->checkBox_Update->isChecked()){
        myTime+=1;
        int error=Clang_Errors();
        if(!error){
            if(myTime==1){
                Clang_Intermediate_Representation();
            }
            if(myTime==2){
                Output();
            }
            if(myTime==3){
                Gdb_Disassembler();
            }
            if(myTime>4){
                myTime=0;
            }
            ui->plainTextEdit_Error->clear();
        } else {
            ui->plainTextEdit_Ir->clear();
            ui->plainTextEdit_Gdb->clear();
            ui->plainTextEdit_Output->clear();
        }
    }
}

void MainWindow::Output(){

    int argc=ui->lineEdit_argc->text().toUInt();
    std::string argv=ui->lineEdit_argv->text().toStdString();

    std::string command="./program ";
    command+=argc;
    command+=" ";
    command+=argv;
    command+=" > output.txt";
    system(command.c_str());

    // Or convention
    // system("./program > output.txt");
    ui->plainTextEdit_Output->clear();
    FileIn.open("output.txt",std::ios_base::in);
    if(FileIn.is_open()){
        // ui->plainTextEdit_Output->appendPlainText("```c");
        while(getline(FileIn,theString)){
            ui->plainTextEdit_Output->appendPlainText(QString::fromStdString(theString));
        }
        ui->plainTextEdit_Output->appendPlainText("run !");
    }
    FileIn.close();
}

void MainWindow::on_plainTextEdit_Code_cursorPositionChanged()
{
    // Run();
}
void MainWindow::on_plainTextEdit_Code_textChanged()
{
    Run();
}

void MainWindow::Run()
{
    // The clang compiler is quite fast.
    if(ui->checkBox_Update->isChecked()){
        SourceFile();
        Clang_Compile();
    }
}

void MainWindow::CreateGdbCmds(){
    FileOut.open("GdbCmds",std::ios_base::out);
    FileOut <<"disassemble main"<< std::endl;
    FileOut.close();
}

void MainWindow::SourceFile(){
    FileOut.open("program.cpp",std::ios_base::out);
    theString=ui->plainTextEdit_Code->toPlainText().toStdString();
    // theString.erase(0,6);
    // theString.erase(theString.size()-3,theString.size());
    FileOut << theString << std::endl;
    FileOut.close();
}

void MainWindow::Clang_Compile(){
    system("clang++ program.cpp -o program 2>debug.txt");
}

void MainWindow::Clang_Intermediate_Representation(){
    system("clang++ -emit-llvm -S program.cpp -o program.ll");
    ui->plainTextEdit_Ir->clear();
    FileIn.open("program.ll",std::ios_base::in);
    if(FileIn.is_open()){
        // ui->plainTextEdit_Ir->appendPlainText("```c");
        while(getline(FileIn,theString)){
            ui->plainTextEdit_Ir->appendPlainText(QString::fromStdString(theString));
        }
        // ui->plainTextEdit_Ir->appendPlainText("```");
    }
    FileIn.close();
}

int MainWindow::Clang_Errors(){
    ui->plainTextEdit_Error->clear();
    bool error=false;
    FileIn.open("debug.txt",std::ios_base::in);
    if(FileIn.is_open()){
        // ui->plainTextEdit_Error->appendPlainText("```c");
        while(getline(FileIn,theString)){
            ui->plainTextEdit_Error->appendPlainText(QString::fromStdString(theString));
            error=true;
        }
        // ui->plainTextEdit_Error->appendPlainText("```");
    }
    FileIn.close();
    if(error==true){
        return 1;
    }
    return 0;
}

void MainWindow::Gdb_Disassembler(){
    system("gdb program<GdbCmds>dump.txt");
    ui->plainTextEdit_Gdb->clear();
    FileIn.open("dump.txt",std::ios_base::in);
    if(FileIn.is_open()){
        // ui->plainTextEdit_Gdb->appendPlainText("```c");
        while(getline(FileIn,theString)){
            ui->plainTextEdit_Gdb->appendPlainText(QString::fromStdString(theString));
        }
        // ui->plainTextEdit_Gdb->appendPlainText("```");
    }
    FileIn.close();
}

void MainWindow::on_toolButton_ForLoop_pressed()
{
    QString theText=ui->plainTextEdit_UserDefined->toPlainText();
    ui->plainTextEdit_Code->insertPlainText(theText);
}


















