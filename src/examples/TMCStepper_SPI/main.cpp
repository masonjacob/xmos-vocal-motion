#include <stdio.h>
#include <stdint.h>
#include <xs1.h>
#include <syscall.h>
#include <timer.h>
#include <print.h>
#include <platform.h>
#include <xclib.h>
#include <iostream>



//#include "../arduino/TMCStepper-master/src/source/TMCStepper.h"
//#include "../arduino/TMCStepper-master/src/source/spi.h"

#include "TMCStepper.h"


#define WRIST_ROTATION_CURRENT 2000


port_t p_miso  = XS1_PORT_1P;
port_t p_ss = XS1_PORT_1A;
port_t p_sclk = XS1_PORT_1C;
port_t p_mosi = XS1_PORT_1D;
xclock_t cb = XS1_CLKBLK_1;

//"XS1_PORT_1D"  Name="PORT_SPI_MOSI"/>
//<Port Location="XS1_PORT_1P"  Name="PORT_SPI_MISO"/>
//
//
void setup_driver(TMC5160Stepper* driver, int current) {
    driver->begin();
    driver->toff(4); //off time
    driver->microsteps(16); //16 microsteps
    driver->rms_current(current); //400mA RMS
    driver->en_pwm_mode(true);
    driver->pwm_autoscale(true);
	driver->AMAX(500);
    driver->VSTART(10);
	driver->VMAX(2000);
	driver->DMAX(700);
	driver->VSTOP(10);
	driver->RAMPMODE(1);
	driver->XTARGET(-51200);
    driver->shaft(true);
}


int main() {
    spi_master_t spi_ctx;

    spi_master_init(&spi_ctx, cb, p_ss, p_sclk, p_mosi, p_miso);

    spi_master_device_t spi_dev_0;

    spi_master_device_init(&spi_dev_0, &spi_ctx,
        0,
        0, 0,
        spi_master_source_clock_xcore,
        0,
        spi_master_sample_delay_0,
        0, 0 ,0 ,0 );

    spi_master_start_transaction(&spi_dev_0);

    spi_master_delay_before_next_transfer(&spi_dev_0, 0);

    TMC5160Stepper* driver = new TMC5160Stepper(&spi_dev_0, &spi_ctx,
        0,
        0, 0,  //cpol, cpha
        spi_master_source_clock_xcore,
        0,
        spi_master_sample_delay_0,
        0, 0 ,0 ,0);

    setup_driver(driver, WRIST_ROTATION_CURRENT);

    auto xactual = driver->XACTUAL();
    auto xtarget = driver->XTARGET();

    while(1)
    {
    delay_microseconds(1000);
    
    std::cout << "xactual" << xactual << std::endl;
    std::cout << "xtarget" << xtarget << std::endl;
    }



}

