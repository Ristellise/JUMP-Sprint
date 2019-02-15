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
    bool saveFile(const std::string filename);
    bool loadFile(const std::string filename);
    std::string getValueString(const std::string value);
    bool getValueBool(const std::string value);
    double getValueDouble(const std::string value);
    int getValueint(const std::string value);
    bool setValue(const std::string key, const std::string value);
    SaveFiles();
    void parseBuffer(std::string line);
    SaveFiles(const std::string filename);
    ~SaveFiles();
};

