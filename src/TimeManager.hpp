#ifndef __TIME_MANAGER__
#define __TIME_MANAGER__

#include "time.h"

class TimeManager {
public:
    static TimeManager& getInstance(void);
    void printLocalTime(void);

private:
    struct tm m_sTimeInfo;
    TimeManager(void);
};

#endif
