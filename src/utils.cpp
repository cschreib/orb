#include "utils.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

bool FileExists(const std::string& sFilename)
{
    std::ifstream mFile(sFilename);
    return mFile.is_open();
}

uint_t HexToUInt(const std::string& sStr)
{
    uint_t i;
    std::istringstream ss(sStr);
    ss >> std::hex >> i;
    return i;
}

uint_t CountOccurrences(const std::string& sStr, const std::string& sPattern)
{
    uint_t uiCount = 0;
    uint_t uiPos = sStr.find(sPattern);
    while (uiPos != sStr.npos)
    {
        ++uiCount;
        ++uiPos;
        uiPos = sStr.find(sPattern, uiPos);
    }

    return uiCount;
}

std::string Replace(std::string sStr, const std::string& sPattern, const std::string& sReplacement)
{
    uint_t uiPos = sStr.find(sPattern);

    while (uiPos != sStr.npos)
    {
        sStr.replace(uiPos, sPattern.size(), sReplacement);
        uiPos = sStr.find(sPattern, uiPos + sReplacement.size());
    }

    return sStr;
}

std::vector<std::string> SplitEach(const std::string& sStr, const std::string& sDelim)
{
    std::vector<std::string> lPieces;
    uint_t uiPos = sStr.find(sDelim);
    uint_t uiLastPos = 0;
    uint_t uiCurSize = 0;
    while (uiPos != sStr.npos)
    {
        std::cout << uiPos << ", " << uiLastPos << std::endl;
        uiCurSize = uiPos - uiLastPos;
        lPieces.push_back(sStr.substr(uiLastPos, uiCurSize));
        uiLastPos = uiPos + sDelim.size();
        uiPos = sStr.find(sDelim, uiLastPos);
    }

    lPieces.push_back(sStr.substr(uiLastPos));

    return lPieces;
}
