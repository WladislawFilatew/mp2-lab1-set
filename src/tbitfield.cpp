// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw "длина меньше 0";
    BitLen = len;
    if (len % (8 * sizeof(TELEM)))
        MemLen = len / (8 * sizeof(TELEM)) + 1;
    else
        MemLen = len / (8 * sizeof(TELEM));
    pMem = new TELEM[MemLen]{};
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[]pMem;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen)
        throw "неверный индекс";
    return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen)
        throw "неверный индекс";
    return 1 << ((8 * sizeof(TELEM)) - (n % (8 * sizeof(TELEM))) - 1);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen)
        throw "неверный индекс";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen)
        throw "неверный индекс";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen)
        throw "неверный индекс";
    /*return pMem[GetMemIndex(n)] &= GetMemMask(n);*/
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == 0) return 0;
    else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    if (this->BitLen != bf.BitLen)
        delete[]pMem;
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        this->pMem[i] = bf.pMem[i];
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->BitLen != bf.BitLen)
        return false;
    for (int i = 0; i < MemLen; i++)
        if (this->pMem[i] != bf.pMem[i])
            return false;
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !this->operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField temp(max(this->BitLen,bf.BitLen));
    for (int i = 0; i < min(this->MemLen, bf.MemLen); i++) {
        temp.pMem[i] = this->pMem[i] | bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField temp(max(this->BitLen, bf.BitLen));
    for (int i = 0; i < min(this->MemLen, bf.MemLen); i++) {
        temp.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(this->BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (this->GetBit(i) == 0)
            temp.SetBit(i);
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len; istr >> len;
    if (len != bf.BitLen)
        throw "Неверный размер";
    for (int i = 0; i < len; i++) {
        int x; istr >> x;
        if (x == 1)
            bf.SetBit(i);
        else if (x == 0)
            bf.ClrBit(i);
        else
            throw "Неверный символ";
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    ostr << bf.BitLen << "\n";
    for (int i = 0; i < bf.BitLen; i++)
        if (bf.GetBit(i) == 0)
            ostr << 0;
        else
            ostr << 1;
    ostr <<  "\n";
    ostr << bf.pMem[0] << endl;
    return ostr;
}
