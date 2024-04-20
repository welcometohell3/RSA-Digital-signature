#ifndef MD4_H
#define MD4_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <sstream>

class MD4
{
public:
    std::string hash(const std::string &message);

private:
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;

    std::vector<uint8_t> padMessage(const std::string &message);
    void processBlock(const std::vector<uint8_t> &block, size_t offset);
    void FF(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s);
    void GG(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s);
    void HH(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s);
    uint32_t rotateLeft(uint32_t x, uint32_t n);
    uint32_t swapEndian(uint32_t value);
};

#endif

std::string MD4::hash(const std::string &message)
{
    std::vector<uint8_t> paddedMessage = padMessage(message);

    for (size_t i = 0; i < paddedMessage.size(); i += 64)
    {
        processBlock(paddedMessage, i);
    }

    std::ostringstream result;
    result << std::hex << swapEndian(A) << swapEndian(B) << swapEndian(C) << swapEndian(D);
    return result.str();
}

std::vector<uint8_t> MD4::padMessage(const std::string &message)
{
    std::vector<uint8_t> paddedMessage(message.begin(), message.end());
    size_t originalLength = message.length();

    paddedMessage.push_back(0x80);
    size_t paddedLength = paddedMessage.size();
    size_t zeroPadding = (paddedLength + 8) % 64;
    zeroPadding = zeroPadding == 0 ? 0 : 64 - zeroPadding;
    paddedMessage.resize(paddedLength + zeroPadding + 8);

    uint64_t bitLength = originalLength * 8;
    for (int i = 0; i < 8; ++i)
    {
        paddedMessage[paddedLength + zeroPadding + i] = (bitLength >> (i * 8)) & 0xFF;
    }
    return paddedMessage;
}

void MD4::processBlock(const std::vector<uint8_t> &block, size_t offset)
{
    std::vector<uint32_t> X(16);

    for (int i = 0; i < 16; ++i)
    {
        X[i] = block[offset + i * 4] |
               (block[offset + i * 4 + 1] << 8) |
               (block[offset + i * 4 + 2] << 16) |
               (block[offset + i * 4 + 3] << 24);
    }

    uint32_t AA = A;
    uint32_t BB = B;
    uint32_t CC = C;
    uint32_t DD = D;

    FF(A, B, C, D, X[0], 3);
    FF(D, A, B, C, X[1], 7);
    FF(C, D, A, B, X[2], 11);
    FF(B, C, D, A, X[3], 19);
    FF(A, B, C, D, X[4], 3);
    FF(D, A, B, C, X[5], 7);
    FF(C, D, A, B, X[6], 11);
    FF(B, C, D, A, X[7], 19);
    FF(A, B, C, D, X[8], 3);
    FF(D, A, B, C, X[9], 7);
    FF(C, D, A, B, X[10], 11);
    FF(B, C, D, A, X[11], 19);
    FF(A, B, C, D, X[12], 3);
    FF(D, A, B, C, X[13], 7);
    FF(C, D, A, B, X[14], 11);
    FF(B, C, D, A, X[15], 19);

    GG(A, B, C, D, X[0], 3);
    GG(D, A, B, C, X[4], 5);
    GG(C, D, A, B, X[8], 9);
    GG(B, C, D, A, X[12], 13);
    GG(A, B, C, D, X[1], 3);
    GG(D, A, B, C, X[5], 5);
    GG(C, D, A, B, X[9], 9);
    GG(B, C, D, A, X[13], 13);
    GG(A, B, C, D, X[2], 3);
    GG(D, A, B, C, X[6], 5);
    GG(C, D, A, B, X[10], 9);
    GG(B, C, D, A, X[14], 13);
    GG(A, B, C, D, X[3], 3);
    GG(D, A, B, C, X[7], 5);
    GG(C, D, A, B, X[11], 9);
    GG(B, C, D, A, X[15], 13);

    HH(A, B, C, D, X[0], 3);
    HH(D, A, B, C, X[8], 9);
    HH(C, D, A, B, X[4], 11);
    HH(B, C, D, A, X[12], 15);
    HH(A, B, C, D, X[2], 3);
    HH(D, A, B, C, X[10], 9);
    HH(C, D, A, B, X[6], 11);
    HH(B, C, D, A, X[14], 15);
    HH(A, B, C, D, X[1], 3);
    HH(D, A, B, C, X[9], 9);
    HH(C, D, A, B, X[5], 11);
    HH(B, C, D, A, X[13], 15);
    HH(A, B, C, D, X[3], 3);
    HH(D, A, B, C, X[11], 9);
    HH(C, D, A, B, X[7], 11);
    HH(B, C, D, A, X[15], 15);

    A += AA;
    B += BB;
    C += CC;
    D += DD;
}

void MD4::FF(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s)
{
    A += ((B & C) | (~B & D)) + X;
    A = rotateLeft(A, s);
}

void MD4::GG(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s)
{
    A += ((B & (C | D)) | (C & D)) + X + 0x5A827999;
    A = rotateLeft(A, s);
}

void MD4::HH(uint32_t &A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t s)
{
    A += (B ^ C ^ D) + X + 0x6ED9EBA1;
    A = rotateLeft(A, s);
}

uint32_t MD4::rotateLeft(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t MD4::swapEndian(uint32_t value)
{
    return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) |
           ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
}
