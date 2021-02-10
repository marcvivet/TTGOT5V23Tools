#ifndef __OPEN_WEATHER_MANAGER__
#define __OPEN_WEATHER_MANAGER__


class OpenWeatherManager {
public:
    static OpenWeatherManager& getInstance(void);

private:
    OpenWeatherManager(void);
};

#endif
