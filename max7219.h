#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>



class Max7219
{
private:
    uint8_t _cs;
    uint8_t _nd;
    uint8_t _intensivity;
    uint8_t _decodeMode = 0x00;

    uint16_t getSymbol(uint8_t number);
    uint32_t lcdPow10(uint8_t n);

    #define CS_SET() digitalWrite(_cs, LOW)
    #define CS_RESET() digitalWrite(_cs, HIGH)

    const uint8_t SYMBOLS[16] = {
		0x7E,	// numeric 0
		0x30,	// numeric 1
		0x6D,	// numeric 2
		0x79,	// numeric 3
		0x33,	// numeric 4
		0x5B,	// numeric 5
		0x5F,	// numeric 6
		0x70,	// numeric 7
		0x7F,	// numeric 8
		0x7B,	// numeric 9
		0x01,	// minus
		0x4F,	// letter E
		0x37,	// letter H
		0x0E,	// letter L
		0x67,	// letter P
		0x00 };	// blank

public:
    typedef enum 
    {
        REG_NO_OP 			= 0x00,
        REG_DIGIT_0 		= 0x01,
        REG_DIGIT_1 		= 0x02,
        REG_DIGIT_2 		= 0x03,
        REG_DIGIT_3 		= 0x04,
        REG_DIGIT_4 		= 0x05,
        REG_DIGIT_5 		= 0x06,
        REG_DIGIT_6 		= 0x07,
        REG_DIGIT_7 		= 0x08,
        REG_DECODE_MODE 	= 0x09,
        REG_INTENSITY 		= 0x0A,
        REG_SCAN_LIMIT 		= 0x0B,
        REG_SHUTDOWN 		= 0x0C,
        REG_DISPLAY_TEST 	= 0x0F,
    } RegistersTD;
    
    typedef enum 
    {
        DIGIT_1 = 1, DIGIT_2 = 2, DIGIT_3 = 3, DIGIT_4 = 4,
        DIGIT_5 = 5, DIGIT_6 = 6, DIGIT_7 = 7, DIGIT_8 = 8
    } DigitsTD;

    typedef enum 
    {
        NUM_0		= 0x00,
        NUM_1		= 0x01,
        NUM_2		= 0x02,
        NUM_3		= 0x03,
        NUM_4		= 0x04,
        NUM_5		= 0x05,
        NUM_6		= 0x06,
        NUM_7		= 0x07,
        NUM_8		= 0x08,
        NUM_9		= 0x09,
        MINUS		= 0x0A,
        LETTER_E	= 0x0B,
        LETTER_H	= 0x0C,
        LETTER_L	= 0x0D,
        LETTER_P	= 0x0E,
        BLANK		= 0x0F
    }NumericTD;
    
    Max7219(uint8_t cs, uint8_t numDigit, uint8_t intensity);
    ~Max7219();
    
    void Begin(void);
    void SetIntensivity(uint8_t intensivity);
    void Clean(void);
    void SendData(uint8_t addr, uint8_t data);
    void TurnOn(void);
    void TurnOff(void);
    void DecodeOn(void);
    void DecodeOff(void);
    void PrintDigit(uint8_t position, uint8_t numeric, bool point);
    uint8_t PrintItos(uint8_t position, int value);
    uint8_t PrintNtos(uint8_t position, uint32_t value, uint8_t n);
    uint8_t PrintFtos(uint8_t position, float value, uint8_t n);
};

#endif