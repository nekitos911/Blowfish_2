#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <vector>
#include "iostream"
#include "cstring"
#include "cmath"
#include "sstream"

typedef uint32_t DWORD;
typedef unsigned char BYTE;

union aword { // little endian
    DWORD dword;
    BYTE byte [4];
      struct {
        unsigned int byte3 : 8;
        unsigned int byte2 : 8;
        unsigned int byte1 : 8;
        unsigned int byte0 : 8;
      }byteStruct;
    };

class Blowfish
{
private:
    DWORD p[18];
    DWORD s[4][256];
    const BYTE *IV = reinterpret_cast<const BYTE *>("qwertyui");

    void setupKey(const BYTE *key,int length);
    void encipher(DWORD *xl,DWORD *xr);
    void decipher(DWORD *xl,DWORD *xr);
    DWORD F(aword value);
public:
    explicit Blowfish(const std::vector<BYTE> &key);

    std::vector<BYTE> decrypt(const std::vector<BYTE> &dataInput);
    std::vector<BYTE> encrypt(const std::vector<BYTE> &dataInput);

    int getOutputLength(std::vector<BYTE> &data);
};
#endif // BLOWFISH_H
