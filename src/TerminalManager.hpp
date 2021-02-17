#ifndef __TERMINAL_MANAGER__
#define __TERMINAL_MANAGER__

#include <Print.h>
#include <Adafruit_I2CDevice.h>
#include <U8g2_for_Adafruit_GFX.h>


class TerminalManager : public Print {
public:
    struct Line {
        uint16_t id;
        Line* next;
        Line* prev;
        uint8_t* buffer;
        uint16_t width;

        Line(void);
        ~Line(void);

        void alloc(const size_t size);
    };

    virtual ~TerminalManager(void);
    static TerminalManager& getInstance(void);

    virtual size_t write(uint8_t);

    template<class T>
    size_t printAutoFlush(const T& p_oString);

    template<class T>
    size_t printAutoFlush(const T& p_oString, int value);

    void flush(void);
    void flushOnAuto(void);

    void setAutoFush(const bool p_bValue);

private:
    bool m_bAutoFlush;
    U8G2_FOR_ADAFRUIT_GFX m_oFont;

    Line* m_pLines;
    Line* m_pCurrLine;
    uint16_t m_uiMaxLines;
    uint16_t m_uiMaxCharacters;
    uint16_t m_uiBufferPos;
 
    uint16_t m_uiCharWidth;
    uint16_t m_uiCharHeight;

    uint16_t m_uiScreenWidth;
    uint16_t m_uiScreenHeight;

    TerminalManager(void);
    TerminalManager(TerminalManager const&);
    void operator=(TerminalManager const&);
    void setCursor(void);
    
};


template<class T>
size_t TerminalManager::printAutoFlush(const T& p_oString) {
    size_t uiSize = print(p_oString);
    flushOnAuto();

    return uiSize;
}

template<class T>
size_t TerminalManager::printAutoFlush(const T& p_oString, int value) {
    size_t uiSize = print(p_oString, value);
    flushOnAuto();

    return uiSize;
}
/*
template<class T>
size_t TerminalManager::println(const T& p_oString) {
    size_t uiSize = m_oFont.println(p_oString);
    update();

    return uiSize;
}

template<class T>
size_t TerminalManager::println(const T& p_oString, int value) {
    size_t uiSize = m_oFont.println(p_oString, value);
    update();

    return uiSize;
}
*/

#endif
