#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <utility>
#include <limits>
#include <sstream>

using uint_t = std::size_t;
using uchar_t = unsigned char;

static const uint_t npos = uint_t(-1);

template<typename T>
bool IsInRange(const T& value, const T& min, const T& max) {
    return min <= value && value <= max;
}

template<typename T>
T Clamp(const T& value, const T& min, const T& max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<typename T>
std::string ToString(const T& mObject) {
    std::ostringstream ss;
    ss << mObject;
    return ss.str();
}

bool FileExists(const std::string& sFilename);
uint_t HexToUInt(const std::string& sStr);
uint_t CountOccurrences(const std::string& sStr, const std::string& sPattern);
std::string Replace(std::string sStr, const std::string& sPattern, const std::string& sReplacement);
std::vector<std::string> SplitEach(const std::string& sStr, const std::string& sPattern);

#endif
