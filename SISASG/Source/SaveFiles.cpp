#include "SaveFiles.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
bool SaveFiles::setValue(const std::string key,const std::string value)
{
    if (this->data.find(key) != this->data.end())
    {
        this->data.insert(std::pair<std::string, std::string>(key, value));
        return true;
    }
    else
    {
        this->data[key] = value;
        return true;
    }
    return false;
}
SaveFiles::SaveFiles()
{
}

void SaveFiles::parseBuffer(std::string line)
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

bool SaveFiles::saveFile(const std::string filename)
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

bool SaveFiles::loadFile(const std::string filename)
{
    std::string buffer = "";
    std::fstream fileStream(filename);
    if (fileStream.is_open())
    {
        while (!fileStream.eof())
        {
            std::getline(fileStream, buffer);
            parseBuffer(buffer);
        }
    }
    this->loaded = true;
    return true;
}

std::string SaveFiles::getValueString(const std::string value)
{
    return this->data[value];
}

bool SaveFiles::getValueBool(const std::string value)
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
    
}

double SaveFiles::getValueDouble(const std::string value)
{
    return std::stod(this->data[value]);
}

int SaveFiles::getValueint(const std::string value)
{
    return std::stoi(this->data[value]);
}

SaveFiles::SaveFiles(const std::string filename)
{
    this->loadFile(filename);
}

SaveFiles::~SaveFiles()
{
    this->loaded = false;
}
