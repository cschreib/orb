#ifndef LOG_H
#define LOG_H

#include "utils.h"

void Log(const std::string& sMessage, bool bTimeStamps, uint_t uiOffset);

template<class T> void Log(const T& mObject, bool bTimeStamps = true, uint_t uiOffset = 0)
{
    Log(ToString(mObject), bTimeStamps, uiOffset);
}

void Error(const std::string& sClass, const std::string& sMessage);
void Warning(const std::string& sClass, const std::string& sMessage);

template<class T, class ... Args>
void Error(const std::string& sClass, const std::string& sMessage, const T& mObject, const Args& ... args)
{
    Error(sClass, sMessage+ToString(mObject), args...);
}

template<class T, class ... Args>
void Warning(const std::string& sClass, const std::string& sMessage, const T& mObject, const Args& ... args)
{
    Warning(sClass, sMessage+ToString(mObject), args...);
}

#endif
