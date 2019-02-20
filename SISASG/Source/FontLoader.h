#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "Mtx44.h"
struct FontInfo
{
    // INFO
    std::string name;
    int fntsize;
    int bold;
    int italic;
    std::string charset;
    int unicode;
    int stretchH;
    int AA;
    int Smooth;
    int outline;
    int paddingUp;
    int paddingDown;
    int paddingLeft;
    int paddingRight;

    int spacingHorizontal;
    int spacingVertical;

    // common
    unsigned int lineHeight;
    unsigned int base;
    unsigned int scaleW;
    unsigned int scaleH;
    unsigned int pages;
    unsigned int packed;
    unsigned int channels[4];

};

struct intpair
{
    int a;
    int b;
    intpair(int a = 0, int b = 0) { Set(a, b); }
    void Set(int a, int b) { this->a = a; this->b = b; }
};
struct charData
{
    unsigned int id = 0;

    intpair Coordinate = intpair();
    intpair characterSize = intpair();
    intpair offset = intpair();
    int advance = 0;
    int page = 0;
    int channel = 0;
};

struct FontResult
{
    charData font;
    unsigned int index;
};

class FontLoader
{
public:
    FontLoader();
    ~FontLoader();
    FontResult getFontData(unsigned int index);
    bool Loadfnt(std::string path);
    FontInfo fntInfo;
    std::unordered_map<char, charData> characters;
};
