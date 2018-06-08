#pragma once

#include <inttypes.h>

class HIIC
{
  private:
    static uint8_t Buf[];
    static uint8_t BufIdx;
    static uint8_t LastRead;
    static uint8_t BytesAvail;

  private:
    bool base_start();
    bool base_write(const uint8_t &data);
    uint8_t base_read(const bool &ack);
    void base_stop();

  public:
    void init(const uint32_t &baud = 100000UL, const uint32_t &scal = 1);
    void beginTransmission(const uint8_t &addr);
    bool write(const uint8_t &data);
    uint8_t endTransmission(const bool &stop);
    uint8_t requestFrom(const uint8_t &addr, const uint8_t &byte);
    uint8_t read();
    bool available();
};

extern HIIC iic;
