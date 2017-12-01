#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <vector>
#include "iostream"
#include "cstring"
#include "cmath"
#include "sstream"

typedef unsigned char BYTE;

union aword { // little endian
    uint32_t uint32;
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
    uint32_t p[18];
    uint32_t s[4][256];
    const BYTE *IV = reinterpret_cast<const BYTE *>("qwertyui");

    void setupKey(const BYTE *key,int length);
    void encipher(uint32_t *xl,uint32_t *xr);
    void decipher(uint32_t *xl,uint32_t *xr);
    uint32_t F(aword value);

    int getOutputLength(std::vector<BYTE> &data);
public:
    explicit Blowfish(const std::vector<BYTE> &key);

    std::vector<BYTE> decrypt(const std::vector<BYTE> &dataInput);
    std::vector<BYTE> encrypt(const std::vector<BYTE> &dataInput);

};
#endif // BLOWFISH_H
