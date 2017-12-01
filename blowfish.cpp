#include "blowfish.h"
#include "constants.h"

Blowfish::Blowfish(const std::vector<BYTE> &key)
{
    try {
        if(key.size() < 4) {
            throw 4;
        }
        else if (key.size() > 56) {
            throw 56;
        }
        setupKey(key.data(),key.size());
    } catch(int i) {
        if(i == 4)
            std::cout << "lenght is < " << i << std::endl;
        else if(i == 56)
            std::cout << "lenght is > " << i << std::endl;
    }
}

void Blowfish::setupKey(const BYTE *key, int length) {
    int j = 0;
    union aword temp{};
    uint32_t data,xl,xr;

    std::memcpy(p,bf_P,sizeof(bf_P));
    std::memcpy(s,bf_S, sizeof(bf_S));

    for (unsigned int &i : p) {
        temp.uint32 = 0;
        temp.byteStruct.byte0 = key[j];
        temp.byteStruct.byte1 = key[(j + 1) % length];
        temp.byteStruct.byte2 = key[(j + 2) % length];
        temp.byteStruct.byte3 = key[(j + 3) % length];
        data = temp.uint32;
        i ^= data;
        j = (j + 4) % length;
    }
    xl = 0x00000000;
    xr = 0x00000000;

    for(size_t i = 0;i < N + 2;i += 2) {
        encipher(&xl, &xr);
        p[i] = xl;
        p[i + 1] = xr;
    }

    for (auto &i : s) {
        for (size_t k = 0; k < 256; k += 2) {
            encipher(&xl,&xr);
            i[k] = xl;
            i[k + 1] = xr;
        }
    }
}

void Blowfish::encipher(uint32_t *xl, uint32_t *xr) {
    union aword Xl{},Xr{};
    Xl.uint32 = *xl;
    Xr.uint32 = *xr;

    Xl.uint32 ^= p[0];

    for(size_t i = 0;i < 16;i += 2) {
        Xr.uint32 ^= (F(Xl) ^ p[i + 1]);
        Xl.uint32 ^= (F(Xr) ^ p[i + 2]);
    }

    Xr.uint32 ^= p[N + 1];

    *xr = Xl.uint32;
    *xl = Xr.uint32;
}

void Blowfish::decipher(uint32_t *xl, uint32_t *xr) {
    union aword Xl{},Xr{};
    Xl.uint32 = *xl;
    Xr.uint32 = *xr;

    Xl.uint32 ^= p[N + 1];
    for(size_t i = N;i > 0;i -= 2) {
        Xr.uint32 ^= (F(Xl) ^ p[i]);
        Xl.uint32 ^= (F(Xr) ^ p[i - 1]);
    }

    Xr.uint32 ^= p[0];

    *xl = Xr.uint32;
    *xr = Xl.uint32;
}

std::vector<BYTE> Blowfish::encrypt(const std::vector<BYTE> &dataInput) {
    std::vector<BYTE> dataOutput = dataInput;
    size_t padding_length;
    if(dataOutput.size() % 8 == 0)
        padding_length = 0;
    else
        padding_length = sizeof(uint64_t) - dataInput.size() % sizeof(uint64_t);
    for (size_t i = 0; i < padding_length; ++i) {
        dataOutput.push_back((BYTE)(((int)'0') + padding_length));
    }
    for (size_t i = 0; i < sizeof(uint64_t); ++i) {
        dataOutput.data()[i] ^= IV[i];
    }
    uint32_t *Xl = &reinterpret_cast<uint32_t *>(dataOutput.data())[0];
    uint32_t *Xr = &reinterpret_cast<uint32_t *>(dataOutput.data())[1];
    encipher(Xl,Xr);
    for (size_t i = 1; i < dataOutput.size() / sizeof(uint64_t); ++i) {
        uint64_t *firstBlock = &reinterpret_cast<uint64_t *>(dataOutput.data())[i - 1];
        uint64_t *secondBlock = &reinterpret_cast<uint64_t *>(dataOutput.data())[i];
        *secondBlock ^= *firstBlock;
        uint32_t *xl = &reinterpret_cast<uint32_t *>(dataOutput.data())[i * 2];
        uint32_t *xr = &reinterpret_cast<uint32_t *>(dataOutput.data())[i * 2 + 1];
        encipher(xl,xr);
    }
    return dataOutput;
}

std::vector<BYTE> Blowfish::decrypt(const std::vector<BYTE> &dataInput) {
    std::vector<BYTE> dataOutput = dataInput;

    for (size_t i = dataOutput.size() / sizeof(uint64_t) - 1; i >= 1 ; --i) {
        uint64_t *firstBlock = &reinterpret_cast<uint64_t *>(dataOutput.data())[i];
        uint32_t *xl = &reinterpret_cast<uint32_t *>(dataOutput.data())[i * 2];
        uint32_t *xr = &reinterpret_cast<uint32_t *>(dataOutput.data())[i * 2 + 1];
        decipher(xl,xr);
        uint64_t *secondBlock = &reinterpret_cast<uint64_t *>(dataOutput.data())[i - 1];
        *firstBlock ^= *secondBlock;
    }
    uint32_t *Xl = &reinterpret_cast<uint32_t *>(dataOutput.data())[0];
    uint32_t *Xr = &reinterpret_cast<uint32_t *>(dataOutput.data())[1];
    decipher(Xl,Xr);
    for (size_t j = 0; j < sizeof(uint64_t); ++j) {
        dataOutput.data()[j] ^= IV[j];
    }
    getOutputLength(dataOutput);

    return dataOutput;
}

uint32_t Blowfish::F(union aword value) {
    auto a = value.byteStruct.byte0;
    auto b = value.byteStruct.byte1;
    auto c = value.byteStruct.byte2;
    auto d = value.byteStruct.byte3;
    return ((s[0][a] + s[1][b]) ^ s[2][c]) + s[3][d];
}

int Blowfish::getOutputLength(std::vector<BYTE> &data) {
    if(data[data.size() - 1] == '\n')
        data.pop_back();
    size_t length = data[data.size() - 1] - '0';
    if(0 < length && length < 10)
        for (size_t i = 0; i < length; ++i) {
            data.pop_back();
        }
    return length;
}
