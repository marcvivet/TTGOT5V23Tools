#ifndef __CONFIG__
#define __CONFIG__

//  WiFi credentials
#define WIFI_SSID "<Your Wifi SSID>"
#define WIFI_PASSWORD "<Your Wifi Password>"

// Use your own API key by signing up for a free developer account at https://openweathermap.org/
#define OPEN_WEATHER_KEY "<Your Open Weather Key>"


#define OPEN_WEATHER_SERVER "api.openweathermap.org"
//Set your location according to OWM locations
#define OPEN_WEATHER_CITY "Barcelona"
// Could it be "metric" or "imperial"
#define OPEN_WEATHER_UNITS "metric"
// Your _ISO-3166-1_two-letter_country_code country code, on OWM find your nearest city and the country code is displayed
#define OPEN_WEATHER_COUNTRY "ES"
// https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
#define OPEN_WEATHER_LANGUAGE "EN"
// "north" or "south"
#define OPEN_WEATHER_HEMISPHERE "north"
// Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define OPEN_WEATHER_TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// Or, choose a time server close to you, but in most cases it's best to use pool.ntp.org to find an NTP server
// then the NTP system decides e.g. 0.pool.tp.org, 1.pool.ntp.org as the NTP syem ries to find  the closest available servers
// EU "0.europe.pool.ntp.org"
// US "0.north-america.pool.ntp.org"
// See: https://www.ntppool.org/en/
#define TIME_NTP_SERVER "pool.ntp.org"

// UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
#define TIME_GMT_OFFSET_SEC 3600
// In the UK DST is +1hr or 3600-secs, other countries may use 2hrs 7200 or 30-mins 1800 or 5.5hrs 19800 Ahead of GMT use + offset behind - offset
#define TIME_DAYLIGHT_OFFSET_SEC 3600 * 2

#endif
