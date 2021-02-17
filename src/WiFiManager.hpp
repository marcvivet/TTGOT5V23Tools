#ifndef __WIFI_MANAGER__
#define __WIFI_MANAGER__

#include <WiFi.h>


class WiFiManager {
public:
    static WiFiManager& getInstance(void);
    const bool connect(void) const;
    const bool disconnect(void) const;
    String httpGET(const String& p_oServer, const String& p_oUri, const int p_iPort = 80, const int p_iRetry = 5) const;

private:
    WiFiManager(void);
    WiFiManager(WiFiManager const&);
    void operator=(WiFiManager const&);
};

#endif
