#include "FontLoader.h"
#include "Utility.h"

FontLoader::FontLoader()
{

}

enum LineType
{
    LT_INFO,
    LT_COMMON,
    LT_PAGE,
    LT_CHARS,
    LT_CHAR,
    LT_UNKNOWN,
    LT_COUNT
};

charData ReadChar(std::string charLine)
{
    std::string data = charLine.substr(4);
    charData buff;
    sscanf_s(charLine.c_str(),
        "char id=%u x=%i y=%i width=%i height=%i xoffset=%i yoffset=%i xadvance=%i page=%i chnl=%i",
        &buff.id,
        &buff.Coordinate.a,
        &buff.Coordinate.b,
        &buff.characterSize.a,
        &buff.characterSize.b,
        &buff.offset.a,
        &buff.offset.b,
        &buff.advance,
        &buff.page,
        &buff.channel
    );
    return buff;
}

LineType getLineType(std::string line)
{
    switch (line[0])
    {
    case 'c':
    {
        switch (line[1])
        {
        case 'o':
        {
            return LineType::LT_COMMON;
        }
        case 'h':
        {
            switch (line[4])
            {
            case 's':
            {
                return LineType::LT_CHARS;
            }
            case ' ':
            {
                return LineType::LT_CHAR;
            }
            default:
                return LineType::LT_UNKNOWN;
            }
        }
        default:
            return LineType::LT_UNKNOWN;
        }
    }
    case 'i':
    {
        return LineType::LT_INFO;
    }
    case 'p':
    {
        return LineType::LT_PAGE;
    }
    default:
        return LineType::LT_UNKNOWN;
    }
}

FontResult FontLoader::getFontData(unsigned int index)
{
    FontResult res;
    for (size_t i = 0; i < this->characters.size(); i++)
    {
        if (this->characters[i].id == index)
        {
            res.font = this->characters[i];
            res.index = i;
            return res;
        }
    }
    return res;
}

bool FontLoader::Loadfnt(std::string file_path)
{
    std::ifstream infile(file_path);
    std::string buff = "";
    std::cout << "Reading File. Please Have Patience... [";
    if (infile.is_open())
    {
        
        while (!infile.eof())
        {
            std::getline(infile, buff);
            LineType LTType = getLineType(buff);
            if (LTType == LineType::LT_CHAR)
            {
                
                std::cout << "Ch";
                this->characters.push_back(ReadChar(buff));
            }
            else if (LTType == LineType::LT_INFO)
            {
                &(this->fntInfo).fntsize;
                
                #pragma warning(suppress : 4996)
                sscanf(buff.c_str(), "info face=\"%[^\"]\" size=%u bold=%u italic=%u charset=\"%[^\"]\" unicode=%i stretchH=%i smooth=%i aa=%i padding=%i,%i,%i,%i spacing=%i,%i outline=%i",
                       & (this->fntInfo.name),
                       & (this->fntInfo.fntsize),
                       & (this->fntInfo.bold),
                       & (this->fntInfo.italic),
                       & (this->fntInfo.charset),
                       & (this->fntInfo.unicode),
                       & (this->fntInfo.stretchH),
                       & (this->fntInfo.Smooth),
                       & (this->fntInfo.AA),
                       & (this->fntInfo.paddingUp),
                       & (this->fntInfo.paddingRight),
                       & (this->fntInfo.paddingDown),
                       & (this->fntInfo.paddingLeft),
                       & (this->fntInfo.spacingHorizontal),
                       & (this->fntInfo.spacingVertical),
                       & (this->fntInfo.outline));
            }
            else if (LTType == LineType::LT_COMMON)
            {
                std::cout << "CO" << buff;
                #pragma warning(suppress : 4996)
                sscanf(buff.c_str(), "common lineHeight=%u base=%u scaleW=%u scaleH=%u pages=%u packed=%u alphaChnl=%u redChnl=%u greenChnl=%u blueChnl=%u",
                    & (this->fntInfo.lineHeight),
                    & (this->fntInfo.base),
                    & (this->fntInfo.scaleW),
                    & (this->fntInfo.scaleH),
                    & (this->fntInfo.pages),
                    & (this->fntInfo.packed),
                    & (this->fntInfo.channels[0]),
                    & (this->fntInfo.channels[1]),
                    & (this->fntInfo.channels[2]),
                    & (this->fntInfo.channels[3])
                    );
            }
            else
            {
                std::cout << "?";
            }
        }
    }
    else
    {
        std::cout << "!!ERROR!!]" << std::endl;
        printf_s("Can\'t load font. Unable to read: \"%s\". Check if the file exists correctly at \"%s\".",
                 file_path.c_str(),CurrentDirectory().c_str());
        return false;
    }
    std::cout << "]" << std::endl;
    return true;
}

FontLoader::~FontLoader()
{
}