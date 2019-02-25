#pragma once
#include <map>
#include <fstream>
#include <string>
#include <vector>
class INIFile
{
    std::string name;
    bool loaded;
    std::map<std::string, std::string> data;
    void parseBuffer(std::string line);
public:
    bool saveFile(const std::string filename);
    bool loadFile(const std::string filename);
    std::string getValueString(const std::string value);
    bool getValueBool(const std::string value, bool defaults = false);
    double getValueDouble(const std::string key, double defaults = 0.0);
    int getValueint(const std::string key, int defaults = 0);
    bool setValue(const std::string key, const std::string value);
    std::map<std::string, std::string> getValuesAll();
    std::vector<std::string> getValueMulti(std::string key, char delimiter);
    INIFile();
    INIFile(const std::string filename);
    ~INIFile();
};


