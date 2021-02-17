#ifndef __OPEN_WEATHER_MANAGER__
#define __OPEN_WEATHER_MANAGER__

#include <WiFi.h>
#include <ArduinoJson.h>


class OpenWeatherManager {
public:
    static const uint8_t FORECAST_DAYS = 5;
 
    enum class RequestType {
        WEATHER = 0,
        FORECAST = 1
    };

    struct Weather
    { // For current Day and Day 1, 2, 3, etc
        int Dt;
        String Period;
        String Icon;
        String IconName;
        String Trend;
        String Main0;
        String Forecast0;
        String Forecast1;
        String Forecast2;
        String Description;
        String Time;
        String Country;
        float lat;
        float lon;
        float Temperature;
        float Feelslike;
        float Humidity;
        float High;
        float Low;
        float Winddir;
        float Windspeed;
        float Rainfall;
        float Snowfall;
        float Pop;
        float Pressure;
        int Cloudcover;
        int Visibility;
        String Sunrise;
        String Sunset;
        int Timezone;

        void setValuesFromJSON(const JsonObject& p_oInput, const bool p_bVervose = false);
        String convertUnixTime(const int unix_time) const;
        String convertIconToName(const String& IconName) const;
        void show(void) const;
    };

    typedef Weather Forecast[FORECAST_DAYS];

    static OpenWeatherManager& getInstance(void);
    const Weather& getWeather(void) const;
    const Forecast& getForecast(void) const;
    bool update(const bool p_bVervose = false);
    bool update(const RequestType p_eRequestType, const bool p_bVervose = false);
    
private:
    Weather m_oCurrentWather;
    Forecast m_oForecastWeather;

    OpenWeatherManager(void);
    OpenWeatherManager(OpenWeatherManager const&);
    void operator=(OpenWeatherManager const&);

    bool parseWeather(const String& p_oResponse, const RequestType p_eRequestType, const bool p_bVervose = false);
   
};

#endif
