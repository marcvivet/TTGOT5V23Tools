
#include "config.hpp"
#include "WiFiManager.hpp"
#include "TerminalManager.hpp"

#include <HTTPClient.h>


WiFiManager& WiFiManager::getInstance()
{
    static WiFiManager instance;
    return instance;
}

WiFiManager::WiFiManager(void) {

}

const bool WiFiManager::connect(void) const {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }

    TerminalManager& oTM = TerminalManager::getInstance();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    oTM.printAutoFlush("Connecting .");

    for (int i = 0; i < 10; ++i) {
        if (WiFi.status() != WL_CONNECTED) {
            oTM.printAutoFlush(".");
            delay(500);
            continue;
        }
        break;
    }

    if (WiFi.status() == WL_CONNECTED) {
        oTM.println(" OK");
        oTM.print("IP: ");
        oTM.println(WiFi.localIP());

        int8_t dBm = WiFi.RSSI();
        oTM.print("Strength: ");
        oTM.print(dBm);
        oTM.print("dBm ");
        // -30 dBm	Amazing
        // -67 dBm	Very Good
        // -70 dBm	Okay
        // -80 dBm	Not Good
        // -90 dBm	Unusable
        if (dBm >= -30) {
            oTM.println("Amazing");
        } else {
            if (dBm >= -67) {
                oTM.println("Very Good");
            } else {
                if (dBm >= -70) {
                    oTM.println("Okay");
                } else {
                    if (dBm >= -80) {
                        oTM.println("Not Good");
                    } else {
                        oTM.println("Unusable");
                    }
                }
            }
        }
        return true;
    }
    
    oTM.println(" FAIL");
    return false;
}

const bool WiFiManager::disconnect(void) const {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    return true;
}

String WiFiManager::httpGET(const String& p_oServer, const String& p_oUri, const int p_iPort, const int p_iRetry) const {
    TerminalManager &oTM = TerminalManager::getInstance();
    WiFiClient client;
    HTTPClient http;
    client.stop();

    http.begin(client, p_oServer, p_iPort, p_oUri);
    for (int i = 0; i < p_iRetry; ++i) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK)
        {
            oTM.println("Request succesfuly!");
            String output = http.getString();
            
            http.end();
            return output;
        }
        
        oTM.printf("Request [%d] failed, error: %s", i, http.errorToString(httpCode).c_str());
        delay(500);
    }
    
    client.stop();
    http.end();
    return String();
}