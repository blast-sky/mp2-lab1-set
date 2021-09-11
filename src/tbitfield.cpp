// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len) :
    bitLen(len),
    memLen(0),
    pMem(nullptr)
{
    memLen = getIndex(len - 1) + 1;

    pMem = new uint[memLen];

    for (size_t i = 0; i < memLen; ++i)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) : // конструктор копирования
    TBitField(bf.bitLen)
{
    for (int i = 0; i < memLen; ++i)
        pMem[i] = bf.pMem[i];
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return n / typeBitSize;
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    size_t offset = n % typeBitSize;

    return 1 << offset;
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n > bitLen - 1)
        throw std::out_of_range("setBit");

    size_t memIndex = getIndex(n);
    uint mask = getMask(n);

    pMem[memIndex] |= mask;
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n > bitLen - 1)
        throw std::out_of_range("clrBit");

    size_t memIndex = getIndex(n);
    uint inverseMask = ~getMask(n);

    pMem[memIndex] &= inverseMask;
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n > bitLen - 1)
        throw std::out_of_range("getBit");

    size_t memIndex = getIndex(n);
    uint mask = getMask(n);

    return pMem[memIndex] & mask;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    size_t neededMemLen = getIndex(bf.bitLen) + 1;

    if (memLen < neededMemLen)
    {
        delete pMem;
        pMem = new uint[neededMemLen];

        memLen = neededMemLen;
    }
    else
    {
        for (size_t i = 0; i < memLen; ++i)
            pMem[i] = 0;
    }

    for (int i = 0; i < neededMemLen; ++i)
        pMem[i] = bf.pMem[i];

    bitLen = bf.bitLen;

    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    size_t minBitLen = std::min(bitLen, bf.bitLen);
    size_t maxBitLen = std::max(bitLen, bf.bitLen);

    size_t minActualMemLen = getIndex(minBitLen) + 1;
    size_t maxActualMemLen = getIndex(maxBitLen) + 1;

    for (size_t i = 0; i < minActualMemLen; ++i)
        if (pMem[i] != bf.pMem[i])
            return false;

    uint* maxActualMemLenPMem = bitLen < bf.bitLen ? bf.pMem : pMem;

    for (size_t i = minActualMemLen; i < maxActualMemLen; ++i)
        if (maxActualMemLenPMem[i])
            return false;

    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    size_t maxBitLen = std::max(bitLen, bf.bitLen);
    size_t minActualMemLen = std::min(getIndex(bitLen) + 1, getIndex(bf.bitLen) + 1);

    TBitField result(maxBitLen);

    for (size_t i = 0; i < minActualMemLen; ++i)
        result.pMem[i] = pMem[i] | bf.pMem[i];

    uint* maxActualMemPmem = bitLen < bf.bitLen ? bf.pMem : pMem;

    for (size_t i = minActualMemLen; i < result.memLen; ++i)
        result.pMem[i] = maxActualMemPmem[i];

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    size_t minBitLen = std::min(bitLen, bf.bitLen);

    TBitField result(minBitLen);

    for (size_t i = 0; i < result.memLen; ++i)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    return result;
}

TBitField TBitField::operator~() // отрицание
{
    TBitField result(bitLen);

    for (size_t i = 0; i < bitLen; ++i)
        if (!getBit(i))
            result.setBit(i);
    
    return result;
}

TBitField::~TBitField()
{
    delete pMem;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    for (size_t i = 0; i < bf.bitLen; ++i)
    {
        bool bit; istr >> bit;
        if (bit) bf.setBit(i);
        else     bf.clrBit(i);
    }    

    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (size_t i = 0; i < bf.bitLen; ++i)
        ostr << bf.getBit(i) << "";

    return ostr;
}
