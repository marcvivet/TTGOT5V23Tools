#ifndef __OPEN_WEATHER_MANAGER__
#define __OPEN_WEATHER_MANAGER__

#include <WiFi.h>


class OpenWeatherManager {
public:
    static OpenWeatherManager& getInstance(void);
    bool getData(void);
    bool parseWeather(WiFiClient& json, const String& Type);
    String convertUnixTime(const int unix_time) const;

private:
    OpenWeatherManager(void);
};

#endif
