#include "HIIC.h"

#include <avr/io.h>
#include <util/twi.h>

#include "HUART.h"


#define HIIC_BUF_SIZE 18

HIIC iic = HIIC();

uint8_t HIIC::Buf[HIIC_BUF_SIZE];
uint8_t HIIC::BufIdx = 0;
uint8_t HIIC::LastRead = 0;
uint8_t HIIC::BytesAvail = 0;

void HIIC::init(const uint32_t &baud, const uint32_t &scal)
{
    TWBR = (uint8_t)((((F_CPU / baud) / scal) - 16 ) / 2);
}

bool HIIC::base_start()
{
    TWCR = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    loop_until_bit_is_set(TWCR, TWINT);
    if ((TWSR & 0xF8) != TW_START)
    {
        return true;
    }
    return false;
}

bool HIIC::base_write(const uint8_t &data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    loop_until_bit_is_set(TWCR, TWINT);
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
    {
        return true;
    }
    return false;
}

uint8_t HIIC::base_read(const bool &ack)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA);
    loop_until_bit_is_set(TWCR, TWINT);
    return TWDR;
}

void HIIC::base_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void HIIC::beginTransmission(const uint8_t &addr)
{
    BufIdx = 0;
    Buf[BufIdx] = (addr << 1) | 0;
}

bool HIIC::write(const uint8_t &data)
{
    if (BufIdx >= HIIC_BUF_SIZE - 1)
    {
        return false;
    }
    BufIdx++;
    Buf[BufIdx] = data;
}

uint8_t HIIC::endTransmission(const bool &stop = true)
{
    HIIC::base_start();
    for (uint8_t i = 0; i <= BufIdx; i++)
    {
        HIIC::base_write(Buf[i]);
    }
    if (stop)
    {
        HIIC::base_stop();
    }
    return true;
}

uint8_t HIIC::requestFrom(const uint8_t &addr, const uint8_t &bytes)
{
    BufIdx = 0;
    LastRead = 0;
    BytesAvail = bytes;
    Buf[BufIdx] = (addr << 1) | 1;
    HIIC::base_start();
    HIIC::base_write(Buf[0]);
    for (uint8_t i = 1; i < bytes; i++)
    {
        Buf[i] = HIIC::base_read(true);
    }
    Buf[bytes] = HIIC::base_read(false);
    HIIC::base_stop();
    return true;
}

uint8_t HIIC::read()
{
    LastRead++;
    return Buf[LastRead];
}

bool HIIC::available()
{
    return BytesAvail - LastRead;
}
