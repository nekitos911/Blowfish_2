#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtCore"
#include "QtGui"
#include "QFileDialog"
#include "blowfish.h"
#include <fstream>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionOpen_Folders_triggered();

    void on_decryptButton_clicked();

    void on_pushButton_clicked();

    void on_clearLog_clicked();

    void on_encryptButton_clicked();

private:
    Ui::MainWindow *ui;
    Blowfish *bf;
    std::vector<BYTE> inputPassword;
    std::vector<BYTE> inputString;
    std::string password;
    std::list<std::string> inputFileNames;
    std::vector<int> fileSize;
    std::vector<int> fileNameSize;
    //std::string inputFileName;
    std::ifstream fin;
    std::ofstream fout;
    std::string error;
};

#endif // MAINWINDOW_H
