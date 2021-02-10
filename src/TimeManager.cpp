

#include "config.hpp"
#include "TimeManager.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"


TimeManager& TimeManager::getInstance()
{
    static TimeManager instance;
    return instance;
}

TimeManager::TimeManager(void) {
    TerminalManager& oTM = TerminalManager::getInstance();
    bool bSuccess = WiFiManager::getInstance().connect();
    oTM.printAutoFlush("Setup Time .");
    configTime(TIME_GMT_OFFSET_SEC, TIME_DAYLIGHT_OFFSET_SEC, TIME_NTP_SERVER, "time.nist.gov");

    if (bSuccess) {
        for (int i = 0; i < 10; ++i) {
            if(!getLocalTime(&m_sTimeInfo)) {
                oTM.printAutoFlush(".");
                delay(500);
                continue;
            }
            bSuccess = true;
            break;
        }
    }

    if (bSuccess) {
        oTM.println(" OK");
    } else {
        oTM.println(" FAIL");
    }
}

void TimeManager::printLocalTime(void)
{
    TerminalManager& oTM = TerminalManager::getInstance();
    if(!getLocalTime(&m_sTimeInfo)) {
        oTM.println("Failed to obtain time");
        return;
    }

    /*
    %A	returns day of week
    %m	returns month of year number
    %B	returns month of year
    %b	returns month of year (shorter)
    %d	returns day of month
    %Y	returns year
    %H	returns hour 24h format
    %I	returns hour 12h format
    %M	returns minutes
    %S	returns seconds
    */
    oTM.println(&m_sTimeInfo, "%Y-%m-%d %H:%M:%S");
}