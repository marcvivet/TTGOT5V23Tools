#ifndef __WIFI_MANAGER__
#define __WIFI_MANAGER__

#include <WiFi.h>


class WiFiManager {
public:
    static WiFiManager& getInstance(void);
    const bool connect(void) const;
    const bool disconnect(void) const;

private:
    WiFiManager(void);
};

#endif
