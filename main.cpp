#include <iostream>
#include "blowfish.h"
//#include "blowfish1.h"
using namespace std;

int main()
{
    std::string pwd = "qwerty";
    std::string str = "qwertyuiqwertyui";
    std::vector<BYTE> _pwd;
    std::vector<BYTE> _str;
    for (char &i : pwd) {
        _pwd.push_back(reinterpret_cast<unsigned char &&>(i));
    }
    for (char &j : str) {
        _str.push_back(reinterpret_cast<unsigned char &&>(j));
    }
    auto *blowfish = new Blowfish(_pwd);
    std::vector<BYTE> encryptedStr = blowfish->encrypt(_str);
    std::vector<BYTE> decryptedStr = blowfish->decrypt(encryptedStr);

    for (unsigned char k : decryptedStr) {
        std::cout << k;
    }
    return 0;
}
