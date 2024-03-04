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
port_t p_ss = XS1_PORT_1A; //Active Low
port_t p_sclk = XS1_PORT_1C;
port_t p_mosi = XS1_PORT_1D;
xclock_t cb = XS1_CLKBLK_1;
port_t led = XS1_PORT_4F;

//"XS1_PORT_1D"  Name="PORT_SPI_MOSI"/>
//<Port Location="XS1_PORT_1P"  Name="PORT_SPI_MISO"/>
//
//
void setup_driver(TMC5160Stepper* driver, int current) {
    driver->begin();
    std::cout << "Setup check 1" << std::endl;
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

    // spi_master_device_init(&spi_dev_0, &spi_ctx,
    //     0,
    //     0, 0,
    //     spi_master_source_clock_xcore,
    //     75,
    //     spi_master_sample_delay_0,
    //     0, 0 ,0 ,0 );

    // spi_master_start_transaction(&spi_dev_0);
    // spi_master_start_transaction(&spi_dev_0);

    // spi_master_delay_before_next_transfer(&spi_dev_0, 0);

    // spi_master_transfer(&spi_dev_0, (uint8_t*)0x05, (uint8_t*)0x00, 1);

    TMC5160Stepper* driver = new TMC5160Stepper(&spi_dev_0, &spi_ctx,
        0,
        0, 0,  //cpol, cpha
        spi_master_source_clock_xcore,
        75,
        spi_master_sample_delay_0,
        0, 0 ,0 ,0);

    setup_driver(driver, WRIST_ROTATION_CURRENT);
    
    port_enable(led);

    while(1) {
        auto xactual = driver->XACTUAL();
        auto xtarget = driver->XTARGET();

        std::cout<<"ioin="<<driver->IOIN()<<" xactual="<<xactual<<"\n";

        if (xactual == xtarget) 
            {
            driver->XTARGET(-xactual);
            }
        port_out(led, 0b1100);
        delay_milliseconds(100);
        port_out(led, 0b0000);
        delay_milliseconds(100);
    }

    ////////////////////////////////////////
    //// WORKING BASIC I/O TEST         ////
    ////////////////////////////////////////

    // port_enable(p_miso);
    // port_enable(led);

    // while(1) {
    //     port_out(p_miso, 1);
    //     port_out(led, 0b1100);
    //     delay_milliseconds(1000);
    //     port_out(p_miso, 0);
    //     port_out(led, 0b0000);
    //     delay_milliseconds(1000);
    // }

    // port_disable(p_miso);
    // port_disable(led);

}

