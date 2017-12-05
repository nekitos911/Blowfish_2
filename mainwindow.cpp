#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStringList"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->errorLabel->setText("Use Files->Open Files(Folders) for encrypt your files");
    ui->errorLabel->append("Use Files->Open Files and choose encrypted.bin for decrypt your files");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Files_triggered()
{

    QStringList files = QFileDialog::getOpenFileNames();
    for (auto &file : files) {
        ui->filesField->append(file);
        ui->filesField->setPlaceholderText("\n");
        inputFileNames.push_back(file.toStdString());
    }
}

void MainWindow::on_actionExit_triggered()
{

}
void MainWindow::FindFiles(QString rootPath, QStringList &result)
{
    foreach(QFileInfo fileInfo, QDir(rootPath).entryInfoList(QDir::NoDotAndDotDot | QDir::Files))
        result.push_back(fileInfo.absoluteFilePath());

    foreach(QString dir, QDir(rootPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs))
        FindFiles(rootPath +"/" + dir, result);
}

void MainWindow::on_actionOpen_Folders_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    QString folder = dialog.getExistingDirectory();
    QStringList files;
    FindFiles(folder,files);
    for (auto &file : files) {
        ui->filesField->append(file);
        ui->filesField->setPlaceholderText("\n");
        inputFileNames.push_back(file.toStdString());
    }
}

void MainWindow::on_encryptButton_clicked()
{
    try {
        for (auto &inputFileName : inputFileNames) {
            for(int i = 0;i < inputFileName.size();i++ )
                if(inputFileName[i] == '/')
                    inputFileName.replace(static_cast<unsigned int>(i), 1, "\\\\");
        }
   char ch;
        auto *files = new std::string[inputFileNames.size()];
   std::vector<BYTE> vec;
   int j = 0;
   for (auto &inputFileName : inputFileNames) {
       int i = inputFileName.size() - 1;
       while(inputFileName[i] != '\\') {
           i--;
       }
       files[j] = (inputFileName.substr(static_cast<unsigned int>(i + 1), inputFileName.size() - i));
       j++;
   }
    fout.open("file_names.bin",std::ios_base::binary);

   password = ui->password->text().toStdString();
   if (password.size()  > 56 || password.size() < 4) {
       error = "Password should be from 4 to 56";
       throw error;
    }
   std::copy(password.begin(),password.end(),std::back_inserter(inputPassword));
   bf = new Blowfish(inputPassword);
   for(int i = 0;i < inputFileNames.size();i++) {
       int count = 0;
       std::copy(files[i].begin(),files[i].end(),std::back_inserter(vec));
       vec = bf->encrypt(vec);
       count = vec.size();
                  for (unsigned char k : vec) {
                      fout << k;
                  }
                  fileNameSize.push_back(count);
                  vec.clear();
   }
   fout.close();
   fout.open("encrypted.bin",std::ios_base::binary);
   fout << bf->getIV();
   for (auto &inputFileName : inputFileNames) {
       fin.open(inputFileName,std::ios_base::binary);
       int count = 0;
       while (fin.get(ch)) {
        inputString.push_back((BYTE)ch);
       }
       inputString = bf->encrypt(inputString);
       count = (inputString.size());
       fileSize.push_back(reinterpret_cast<int &&>(count));
       for (unsigned char &it1 : inputString) {
           fout << it1;
       }
       inputString.clear();
       fin.close();
   }
   fout.close();
   fout.open("file_size.bin",std::ios_base::binary);
   for (int i : fileSize)
       fout << i << '\n';
   for(int i = 0; i < fileSize.size();i++)
       fout << fileNameSize[i] << '\n';
           fout.close();
           ui->filesField->clear();
           ui->password->clear();
           ui->errorLabel->clear();
           inputString.clear();
           inputFileNames.clear();
           fileSize.clear();
           IV.clear();
           delete[] files;
           delete bf;
    } catch(std::string errorStr) {
        QString str;
        ui->errorLabel->append(str.fromStdString(errorStr));
    }
}

void MainWindow::on_decryptButton_clicked()
{
    try {
        if(inputFileNames.size() != 1) {
            error = "Chose only encrypted.bin";
            throw error;
    }

   char ch;
   fin.open("file_size.bin",std::ios_base::binary);
   int filesCount = 0;
   while(fin.get(ch)) {
       if(ch == '\n') {
       filesCount++;
       }
   }
   filesCount /= 2;
   fin.close();
   fileSize.resize(static_cast<unsigned int>(filesCount));
   fileNameSize.resize(static_cast<unsigned int>(filesCount));
   fin.open("file_size.bin",std::ios_base::binary);
   if(!fin.is_open()) {
       error = "Can't open file whith names of encrypted files!!!";
       throw (error);
   }
  for(int i = 0;i < filesCount;i++) {
      fin >> fileSize[i];
  }
  for(int i = 0;i < filesCount;i++) {
      fin >> fileNameSize[i];
  }
   fin.close();
   fin.open("file_names.bin",std::ios_base::binary);
   if(!fin.is_open()) {
       error = "Can't open file whith names of encrypted files!!!";
       throw (error);
   }
    auto *decryptedFilesName = new std::string[filesCount];
   password = ui->password->text().toStdString();
   if (password.size()  > 56 || password.size() < 4) {
        error = "Password should be from 4 to 56";
       throw error;
    }
   std::copy(password.begin(),password.end(),std::back_inserter(inputPassword));
   bf = new Blowfish(inputPassword);
   auto it = inputFileNames.begin();
   std::ifstream IVStream;
   IVStream.open(*it,std::ios_base::binary);

   for(int i = 0;i < 8;i ++) {
       IVStream.get(ch);
       IV += ch;
   }
   IVStream.close();
   bf->setIV(IV);
   int tmpSize = 0;
   for(int i = 0;i < filesCount;i ++) {
       while(tmpSize < fileNameSize[i]) {
           fin.get(ch);
           inputString.push_back((BYTE)ch);
           tmpSize++;
       }
       inputString = bf->decrypt(inputString);
       for(auto &it : inputString)
           decryptedFilesName[i] += it;
       tmpSize = 0;
       inputString.clear();
   }
   fin.close();
   fin.open(*it,std::ios_base::binary);
   CreateDirectoryW(L"Decrypted", NULL);
   for(int i = 0;i < filesCount;i++) {
       while(tmpSize < fileSize[i]) {
           fin.get(ch);
           inputString.push_back((BYTE)ch);
           tmpSize++;
       }
       inputString = bf->decrypt(inputString);
       fout.open("Decrypted\\" + decryptedFilesName[i],std::ios_base::binary);
       for (auto &it1 : inputString) {
           fout << it1;
       }
       tmpSize = 0;
       inputString.clear();
       fout.close();
   }
   fin.close();
   ui->filesField->clear();
   ui->password->clear();
   ui->errorLabel->clear();
   inputString.clear();
   inputFileNames.clear();
   fileSize.clear();
   IV.clear();
   remove("file_size.bin");
   remove("encrypted.bin");
   remove("file_names.bin");
   } catch(std::string errorStr) {
       QString str;
       ui->errorLabel->append(str.fromStdString(errorStr));
   }

}

void MainWindow::on_clearLog_clicked()
{
    ui->errorLabel->clear();
}

void MainWindow::on_clearFiles_clicked()
{
    ui->filesField->clear();
    inputFileNames.clear();
    inputString.clear();
}
