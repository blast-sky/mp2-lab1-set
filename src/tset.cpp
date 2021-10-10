// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

TSet::TSet(size_t mp) :
    maxPower(mp),
    bitField(mp)
{   };

// конструктор копирования
TSet::TSet(const TSet& s) :
    maxPower(s.maxPower),
    bitField(s.bitField)
{   };

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) :
    maxPower(bf.getLength()),
    bitField(bf)
{   };

TSet::operator TBitField()
{
    return bitField;
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const elType elem) const // элемент множества?
{
    if (elem >= maxPower)
        throw std::out_of_range("isMember");

    return bitField.getBit(elem);
}

void TSet::insElem(const elType elem) // включение элемента множества
{
    if (elem >= maxPower)
        throw std::out_of_range("insElem");

    bitField.setBit(elem);
}

void TSet::delElem(const elType elem) // исключение элемента множества
{
    if (elem >= maxPower)
        throw std::out_of_range("delElem");

    bitField.clrBit(elem);
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet& s) // присваивание
{
    maxPower = s.maxPower;
    bitField = s.bitField;
    return *this;
}

bool TSet::operator==(const TSet& s) const // сравнение
{
    if (maxPower != s.maxPower)
        return false;

    return bitField == s.bitField;
}

bool TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    size_t maxMaxPower = std::max(maxPower, s.maxPower);

    TSet result(maxMaxPower);

    for (size_t i = 0; i < getMaxPower(); ++i)
    {
        if (isMember(i))
            result.insElem(i);
    }

    for (size_t i = 0; i < s.getMaxPower(); ++i)
    {
        if (s.isMember(i))
            result.insElem(i);
    }

    return result;
}

TSet TSet::operator+(const elType elem) // объединение с элементом
{
    size_t needMaxPower = maxPower;
    if (elem >= maxPower)
        needMaxPower = elem + 1;

    TSet result(needMaxPower);
    result.bitField = bitField;
    result.insElem(elem);
    return result;
}

TSet TSet::operator-(const elType elem) // разность с элементом
{
    TSet result = *this;
    result.delElem(elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    size_t maxMaxPower = std::max(maxPower, s.maxPower);
    size_t minMaxPower = std::min(maxPower, s.maxPower);

    TSet result(maxMaxPower);

    for (size_t i = 0; i < minMaxPower; ++i)
    {
        if (isMember(i) && s.isMember(i))
            result.insElem(i);
    }

    return result;
}

TSet TSet::operator~() // дополнение
{
    TSet result(maxPower);

    result.bitField = ~bitField;

    return result;
}

// перегрузка ввода/вывода
std::istream& operator>>(std::istream& istr, TSet& s) // ввод
{
    istr >> s.bitField;
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    ostr << s.bitField;
    return ostr;
}