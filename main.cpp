#include "mainwindow.h"
#include <QApplication>
#include "blowfish.h"
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
//    char ch;
//        std::vector<BYTE> inputString;
//        std::vector<BYTE> outputString;
//        std::vector<BYTE> inputPassword;
//        std::string password = "qwerty";
//        std::ifstream fin("D:\\new.txt",std::ios_base::binary);
//        std::ofstream fout("D:\\encrypted.bin",std::ios_base::binary);
//        if(!fin) {
//            std::cout << "error" << endl;
//            return -1;
//        }
//        while (fin.get(ch))
//            inputString.push_back((BYTE)ch);
//        fin.close();
//        std::copy(password.begin(),password.end(),std::back_inserter(inputPassword));
//        auto *bf = new Blowfish(inputPassword);
//        outputString = bf->encrypt(inputString);

//        for (unsigned char &it : outputString) {
//            fout << it;
//        }
//        fout.close();
//        inputString.clear();
//        outputString.clear();
//        fin.open("D:\\encrypted.bin",std::ios_base::binary);
//        fout.open("D:\\decrypted.txt",std::ios_base::binary);
//        while (fin.get(ch))
//            inputString.push_back((BYTE)ch);
//        password = "qwerty";
//        std::copy(password.begin(),password.end(),std::back_inserter(inputPassword));
//        auto *bf1 = new Blowfish(inputPassword);
//        outputString = bf1->decrypt(inputString);
//        for (unsigned char &it : outputString) {
//            fout << it;
//        }
//        return 0;
}
