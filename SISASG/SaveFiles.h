#pragma once
#include <map>
#include <fstream>
#include <string>
class SaveFiles
{
    std::string name;
    bool loaded;
    std::map<std::string, std::string>data;
public:
    bool saveFile();
    bool loadFile(const std::string filename);
    bool getValue(const std::string value);
    bool setValue(const std::string value);
    SaveFiles();
    SaveFiles(const std::string filename);
    ~SaveFiles();
};

