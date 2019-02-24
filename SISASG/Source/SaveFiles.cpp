#include "SaveFiles.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
bool INIFile::setValue(const std::string key,const std::string value)
{
    if (this->data.find(key) != this->data.end())
    {
        this->data.insert_or_assign(key,value);
        return true;
    }
    else
    {
        this->data[key] = value;
        return true;
    }
    return false;
}
std::map<std::string, std::string> INIFile::getValuesAll()
{
    return this->data;
}
INIFile::INIFile()
{
}

void INIFile::parseBuffer(std::string line)
{
    std::vector<std::string> seglist;
    std::stringstream ssteam(line);
    std::string segment;
    while (std::getline(ssteam, segment, '='))
    {
        seglist.push_back(segment);
    }
    if (seglist.size() != 2)
    {
    }
    else
    {
        this->data.insert(std::pair<std::string,std::string>(seglist[0], seglist[1]));
    }
}

bool INIFile::saveFile(const std::string filename)
{

    if (this->loaded)
    {
        std::fstream stream(filename);
        std::string buff;
        std::map<std::string, std::string>::iterator it;
        if (stream.is_open())
        {
            for (it = this->data.begin(); it != this->data.end(); it++)
            {
                buff = it->first + "=" + it->second + "\n";
                stream.write(buff.c_str(), buff.size());
            }
        }
        else
        {
            return false;
        }
        
    }
    return true;
}

bool INIFile::loadFile(const std::string filename)
{
    std::string buffer = "";
    std::fstream fileStream(filename);
    if (fileStream.is_open())
    {
        while (!fileStream.eof())
        {
            std::getline(fileStream, buffer);
            if (buffer.find('#') == -1)
            {
                parseBuffer(buffer);
            }
        }
    }
    this->loaded = true;
    return true;
}

std::string INIFile::getValueString(const std::string key)
{
    std::map<std::string, std::string>::iterator it;
    it = this->data.find(key);
    if (it != this->data.end())
    {
        return it->second;
    }
    else
    {
        return "";
    }
}

bool INIFile::getValueBool(const std::string value, bool defaults)
{
    std::string val = this->data[value].c_str();
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    if (val == "true" || val == "1")
    {
        return true;
    }
    else if (val == "false" || val == "0")
    {
        return false;
    }
    return defaults;
}

double INIFile::getValueDouble(const std::string key, double defaults)
{
    std::string val = this->getValueString(key);
    if (val == "") // tis empty
    {
        return defaults;
    }
    else
    {
        return std::stod(val);
    }
    
}

int INIFile::getValueint(const std::string key, int defaults)
{
    std::string val = this->getValueString(key);
    if (val == "")
    {
        return defaults;
    }
    else
    {
        return std::stoi(val);
    }
    
}

INIFile::INIFile(const std::string filename)
{
    this->loadFile(filename);
}

INIFile::~INIFile()
{
    this->loaded = false;
}

// Gets a value which is seperated by a delimiter.
// By default, its a ",".
std::vector<std::string> INIFile::getValueMulti(std::string key,char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    if (this->getValueString(key) == "")
    {
        return tokens;
    }
    else
    {
        std::istringstream tokenStream(this->getValueString(key));
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
}