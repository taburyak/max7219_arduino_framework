#include "max7219.h"
#include <Arduino.h>
#include <SPI.h>

Max7219::Max7219(uint8_t cs, uint8_t numDigit, uint8_t intensivity)
{
    _cs = cs;
    _nd = numDigit;
    _intensivity = intensivity;
}

Max7219::~Max7219()
{

}

void Max7219::Begin(void)
{
    pinMode(_cs, OUTPUT);
    CS_RESET();
    this->TurnOn();
    this->SendData(REG_SCAN_LIMIT, _nd - 1);
    this->SetIntensivity(_intensivity);
    this->Clean();
}

void Max7219::SetIntensivity(uint8_t intensivity)
{
    if (intensivity > 0x0F)
	{
		return;
	}

	this->SendData(REG_INTENSITY, intensivity);
}

void Max7219::Clean(void)
{
	uint8_t clear = 0x00;

	if(_decodeMode == 0xFF)
	{
		clear = BLANK;
	}

	for (int i = 0; i < 8; ++i)
	{
		this->SendData(i + 1, clear);
	}
}

void Max7219::SendData(uint8_t addr, uint8_t data)
{
	CS_SET();
	SPI.transfer(addr);
    SPI.transfer(data);
	CS_RESET();
}

void Max7219::TurnOn(void)
{
    this->SendData(REG_SHUTDOWN, 0x01);
}

void Max7219::TurnOff(void)
{
	this->SendData(REG_SHUTDOWN, 0x00);
}

void Max7219::DecodeOn(void)
{
	_decodeMode = 0xFF;
	this->SendData(REG_DECODE_MODE, _decodeMode);
}

void Max7219::DecodeOff(void)
{
	_decodeMode = 0x00;
	this->SendData(REG_DECODE_MODE, _decodeMode);
}

void Max7219::PrintDigit(uint8_t position, uint8_t numeric, bool point)
{
	if(position > _nd)
	{
		return;
	}

	if(point)
	{
		if(_decodeMode == 0x00)
		{
			this->SendData(position, getSymbol(numeric) | (1 << 7));
		}
		else if(_decodeMode == 0xFF)
		{
			this->SendData(position, numeric | (1 << 7));
		}
	}
	else
	{
		if(_decodeMode == 0x00)
		{
			this->SendData(position, getSymbol(numeric) & (~(1 << 7)));
		}
		else if(_decodeMode == 0xFF)
		{
			this->SendData(position, numeric & (~(1 << 7)));
		}
	}
}

uint8_t Max7219::PrintItos(uint8_t position, int value)
{
	this->SendData(REG_DECODE_MODE, 0xFF);

	int32_t i;
    
	if (value < 0)
	{
		if(position > 0)
		{
			this->SendData(position, MINUS);
			position--;
		}
		value = -value;
	}

	i = 1;

	while ((value / i) > 9)
	{
		i *= 10;
	}

	if(position > 0)
	{
		this->SendData(position, value / i);
		position--;
	}

	i /= 10;

	while (i > 0)
	{
		if(position > 0)
		{
			this->SendData(position, (value % (i * 10)) / i);
			position--;
		}

		i /= 10;
	}

	this->SendData(REG_DECODE_MODE, _decodeMode);

	return position;
}

uint8_t Max7219::PrintNtos(uint8_t position, uint32_t value, uint8_t n)
{
	this->SendData(REG_DECODE_MODE, 0xFF);

	if (n > 0u)
	{
		uint32_t i = lcdPow10(n - 1u);

		while (i > 0u)	/* Display at least one symbol */
		{
			if(position > 0u)
			{
				this->SendData(position, (value / i) % 10u);
				position--;
			}

			i /= 10u;
		}
	}

	this->SendData(REG_DECODE_MODE, _decodeMode);

	return position;
}

uint8_t Max7219::PrintFtos(uint8_t position, float value, uint8_t n)
{
	if(n > 4)
	{
		n = 4;
	}

	this->SendData(REG_DECODE_MODE, 0xFF);

	if (value < 0.0)
	{
		if(position > 0)
		{
			this->SendData(position, MINUS);
			position--;
		}

		value = -value;
	}

	position = this->PrintItos(position, (int32_t) value);

	if (n > 0u)
	{
		this->PrintDigit(position + 1, (uint32_t) value % 10, true);

		position = this->PrintNtos(position, (uint32_t) (value * (float) lcdPow10(n)), n);
	}

	this->SendData(REG_DECODE_MODE, _decodeMode);

	return position;
}

uint16_t Max7219::getSymbol(uint8_t number)
{
	return SYMBOLS[number];
}

uint32_t Max7219::lcdPow10(uint8_t n)
{
	uint32_t retval = 1u;

	while (n > 0u)
	{
		retval *= 10u;
		n--;
	}

	return retval;
}