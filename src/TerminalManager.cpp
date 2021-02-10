#include "TerminalManager.hpp"
#include "DisplayManager.hpp"


TerminalManager::Line::Line(void) {
    next = nullptr;
    prev = nullptr;
    buffer = nullptr;
    width = 0;
}    

TerminalManager::Line::~Line(void) {
    if (buffer)
        delete[] buffer;
}

void TerminalManager::Line::alloc(const size_t size) {
    buffer = new uint8_t[size];
    buffer[0] = '\0';
}

TerminalManager& TerminalManager::getInstance()
{
    static TerminalManager instance;
    return instance;
}

TerminalManager::TerminalManager(void) {
    Serial.begin(115200);

    m_oFont.begin(DisplayManager::getInstance().getDisplay());
    m_oFont.setFontMode(1);                           // use u8g2 transparent mode (this is default)
    m_oFont.setForegroundColor(GxEPD_BLACK);          // apply Adafruit GFX color
    m_oFont.setBackgroundColor(GxEPD_WHITE);          // apply Adafruit GFX color
    m_oFont.setFont(u8g2_font_courB10_tf); 

    m_uiBufferPos = 0;

    DisplayManager& oDM = DisplayManager::getInstance();
    m_uiScreenWidth = oDM.getWidth();
    m_uiScreenHeight = oDM.getHeight();

    m_uiCharWidth = (m_oFont.getUTF8Width("B ") + 1) / 2;
    m_uiCharHeight = m_oFont.getFontAscent() + 4;

    m_uiMaxLines = (m_uiScreenHeight + m_uiCharHeight) / m_uiCharHeight;
    m_uiMaxCharacters = m_uiScreenWidth / m_uiCharWidth;

    m_pLines = new Line[m_uiMaxLines];
    m_pCurrLine = m_pLines;
    Line* pCurrLine = m_pCurrLine;

    pCurrLine->id = 0;
    pCurrLine->alloc(m_uiMaxCharacters + 1);     
    pCurrLine->next = m_pCurrLine + 1;
    pCurrLine->prev = m_pCurrLine + (m_uiMaxLines - 1);
    pCurrLine++;

    for (uint16_t i = 1; i < m_uiMaxLines; ++i, ++pCurrLine) {
        pCurrLine->id = i;
        pCurrLine->alloc(m_uiMaxCharacters + 1);
        
        pCurrLine->next = m_pCurrLine + ((i + 1) % m_uiMaxLines);
        pCurrLine->prev = m_pCurrLine + (i - 1);
    }

    m_bAutoFlush = false;
}

TerminalManager::~TerminalManager(void) {
    delete[] m_pLines;
}

void TerminalManager::setCursor(void) {
    m_oFont.setCursor(50, 50);
}

void TerminalManager::flush(void) {
    DisplayManager& oDM = DisplayManager::getInstance();
    Line* pCurrLine = m_pCurrLine;
    m_pCurrLine->buffer[m_uiBufferPos] = '\0';

    oDM.clear();
    for (uint16_t i = 0; i < m_uiMaxLines; ++i) {
        m_oFont.setCursor(0, m_uiScreenHeight + m_uiCharHeight - m_uiCharHeight * i);
        m_oFont.print((char*)pCurrLine->buffer);
        pCurrLine = pCurrLine->prev;
    }
    oDM.updateWindow();
}

void TerminalManager::setAutoFush(const bool p_bValue) {
    m_bAutoFlush = p_bValue;
}

void TerminalManager::flushOnAuto(void) {
    if (m_bAutoFlush)
        m_pCurrLine->buffer[m_uiBufferPos] = '\0';
        m_pCurrLine = m_pCurrLine->next;
        flush();
        m_pCurrLine = m_pCurrLine->prev;
}


size_t TerminalManager::write(uint8_t p_uiValue) {
    if (p_uiValue == '\n') {
        m_pCurrLine->buffer[m_uiBufferPos] = '\0';
        Serial.println((char*)m_pCurrLine->buffer);
        m_uiBufferPos = 0;
        m_pCurrLine = m_pCurrLine->next;
        if (m_bAutoFlush)
            flush();
        return 0;
    }

    if (m_uiBufferPos == m_uiMaxCharacters) {
        m_pCurrLine->buffer[m_uiBufferPos] = '\0';
        Serial.println((char*)m_pCurrLine->buffer);
        m_uiBufferPos = 0;
        m_pCurrLine = m_pCurrLine->next;
        if (m_bAutoFlush)
            flush();
    }

    m_pCurrLine->buffer[m_uiBufferPos++] = p_uiValue;
    return 1;
}




