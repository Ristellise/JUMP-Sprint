#include "SaveFiles.h"
#include <vector>
#include <sstream>

SaveFiles::SaveFiles()
{
}

void parseBuffer(std::string line)
{
    std::vector<std::string> seglist;
    std::stringstream ssteam(line);
    std::string segment;
    while (std::getline(ssteam, segment, '='))
    {
        seglist.push_back(segment);
    }
}

SaveFiles::SaveFiles(const std::string filename)
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
}

SaveFiles::~SaveFiles()
{
}
