#include "SW_SPI.h"
#include <cstdint>

SW_SPIClass::SW_SPIClass(uint16_t mosi, uint16_t miso, uint16_t sck) :
  mosi_pin(mosi),
  miso_pin(miso),
  sck_pin(sck)
  {}

//Pin initialization

void SW_SPIClass::init() {
    /* Setup the clock block */
    spi->clock_block = clock_block;
    clock_enable(spi->clock_block);

    /* Setup the chip select port */
    spi->cs_port = cs_port;
    port_enable(spi->cs_port);
    port_set_clock(spi->cs_port, XS1_CLKBLK_REF);
    port_out(spi->cs_port, 0xFFFFFFFF);
    port_sync(spi->cs_port);
    spi->current_device = 0xFFFFFFFF;

    /* Setup the SCLK port */
    spi->sclk_port = sclk_port;
    port_start_buffered(spi->sclk_port, 32);
    port_set_clock(spi->sclk_port, spi->clock_block);

    /* Setup the MOSI port */
    spi->mosi_port = mosi_port;
    if (mosi_port != 0) {
        port_start_buffered(spi->mosi_port, 32);
        port_set_clock(spi->mosi_port, spi->clock_block);
        port_clear_buffer(spi->mosi_port);
    }

    /* Setup the MISO port */
    spi->miso_port = miso_port;
    if (miso_port != 0) {
        port_start_buffered(spi->miso_port, 32);
        port_set_clock(spi->miso_port, spi->clock_block);
        port_clear_buffer(spi->miso_port);
    }





  pinMode(mosi_pin, OUTPUT);
  pinMode(sck_pin, OUTPUT);
  pinMode(miso_pin, INPUT_PULLUP);
    mosi_register = portOutputRegister(getPort(mosi_pin));
    miso_register = portInputRegister(getPort(miso_pin));
    sck_register = portOutputRegister(getPort(sck_pin));
    mosi_bm = digitalPinToBitMask(mosi_pin);
    miso_bm = digitalPinToBitMask(miso_pin);
    sck_bm = digitalPinToBitMask(sck_pin);
 
  writeSCK_H;
}

uint8_t SW_SPIClass::transfer(uint8_t ulVal) {
  uint8_t value = 0;
  writeSCK_L;

  for (uint8_t i=7 ; i>=1 ; i--) {
    // Write bit
    !!(ulVal & (1 << i)) ? writeMOSI_H : writeMOSI_L;
    // Start clock pulse
    writeSCK_H;
    // Read bit
    value |= ( readMISO ? 1 : 0) << i;
    // Stop clock pulse
    writeSCK_L;
  }

  !!(ulVal & (1 << 0)) ? writeMOSI_H : writeMOSI_L;
  writeSCK_H;
  value |= ( readMISO ? 1 : 0) << 0;

  return value;
}

uint16_t SW_SPIClass::transfer16(uint16_t data) {
  uint16_t returnVal = 0x0000;
  returnVal |= transfer((data>>8)&0xFF) << 8;
  returnVal |= transfer(data&0xFF) & 0xFF;
  return returnVal;
}
