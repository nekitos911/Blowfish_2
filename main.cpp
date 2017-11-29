#include <iostream>
#include "blowfish.h"
//#include "blowfish1.h"
using namespace std;

int main()
{
    std::string pwd = "qwerty";
    std::string str = "qwertyuiqwert";
    std::vector<BYTE> _pwd;
    std::vector<BYTE> _str;
    std::string pwd1 = "qwerty";
    std::string str1 = "qwertyuiqwert";
    std::vector<BYTE> _pwd1;
    std::vector<BYTE> _str1;
    for (char &i : pwd) {
        _pwd.push_back(reinterpret_cast<unsigned char &&>(i));
    }
    for (char &j : str) {
        _str.push_back(reinterpret_cast<unsigned char &&>(j));
    }
    for (char &i : pwd1) {
        _pwd1.push_back(reinterpret_cast<unsigned char &&>(i));
    }
    for (char &j : str1) {
        _str1.push_back(reinterpret_cast<unsigned char &&>(j));
    }
    auto *blowfish = new Blowfish(_pwd);
    std::vector<BYTE> encryptedStr = blowfish->encrypt(_str);
    std::vector<BYTE> decryptedStr = blowfish->decrypt(encryptedStr);

    auto *blowfish2 = new Blowfish(_pwd1);
    decryptedStr = blowfish2->decrypt(encryptedStr);
    for (unsigned char k : decryptedStr) {
        std::cout << k;
    }
    return 0;
}
