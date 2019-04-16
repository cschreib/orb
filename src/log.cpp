#include "log.h"

#include <iostream>
#include <fstream>

#include <SFML/System/Clock.hpp>

void Log( const std::string& sMessage, bool bTimeStamps, uint_t uiOffset )
{
    static std::ofstream mLog("Orb.log");
    static sf::Clock mClock;

    std::string sNewMessage;
    if (bTimeStamps)
    {
        if (sMessage[0] == '|' && sMessage[1] == 't')
        {
            sNewMessage = sMessage;
            sNewMessage = Replace(sNewMessage, "\n", "\n"+std::string(uiOffset, ' '));
            sNewMessage = sNewMessage.substr(2);
        }
        else
        {
            float fTime = mClock.getElapsedTime().asSeconds();

            uint_t uiHours(std::floor(fTime/3600.0));
            fTime -= float(uiHours)*3600.0;
            uint_t uiMinutes(std::floor(fTime/60.0));
            fTime -= float(uiMinutes)*60.0;
            uint_t uiSeconds(std::floor(fTime));
            fTime -= float(uiSeconds);
            uint_t uiMilliseconds(std::floor(fTime*1000));

            std::string sTime = "[";
            sTime += ToString(uiHours) + ":";
            sTime += ToString(uiMinutes) + ":";
            sTime += ToString(uiSeconds) + ":";
            sTime += ToString(uiMilliseconds) + "] : ";

            sNewMessage = sTime+sMessage;
            sNewMessage = Replace(sNewMessage, "\n", "\n"+sTime+std::string(uiOffset, ' '));
        }
    }
    else
    {
        sNewMessage = sMessage;
        sNewMessage = Replace(sNewMessage, "\n", "\n"+std::string(uiOffset, ' '));
    }

    std::cout << sNewMessage << std::endl;
    mLog << sNewMessage << std::endl;
}

void Error( const std::string& sClass, const std::string& sMessage )
{
    std::string sHeader = "# Error # : ";
    if (sClass != "")
        sHeader += sClass + " : ";

    Log(sHeader + sMessage, true, sHeader.size());
}

void Warning( const std::string& sClass, const std::string& sMessage )
{
    std::string sHeader = "# Warning # : ";
    if (sClass != "")
        sHeader += sClass + " : ";

    Log(sHeader + sMessage, true, sHeader.size());
}
