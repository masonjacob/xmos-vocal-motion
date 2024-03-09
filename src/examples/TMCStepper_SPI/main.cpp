#include <stdio.h>
#include <stdint.h>
#include <xs1.h>
#include <syscall.h>
#include <timer.h>
#include <print.h>
#include <platform.h>
#include <xclib.h>
#include <iostream>

float Rsense = 0.075;

//#include "../arduino/TMCStepper-master/src/source/TMCStepper.h"
//#include "../arduino/TMCStepper-master/src/source/spi.h"

#include "TMCStepper.h"


#define WRIST_ROTATION_CURRENT 2000


port_t p_miso  = XS1_PORT_1P;
port_t p_ss = XS1_PORT_1A; //Active Low CHIP SELECT
port_t p_sclk = XS1_PORT_1C; //SPI CLK
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
	driver->XTARGET(2000);
    // XTARGET = -51200 (Move one rotation left (200*256 microsteps) 
    driver->shaft(true);

    //Snapshot from Teemuatlut for positioning mode. Explicitly says step and dir and enable are wired to gnd
    // driver.toff(3);
	// driver.rms_current(800);
	// driver.en_pwm_mode(true);

	// driver.A1(1000);
	// driver.V1(50000);
	// driver.AMAX(500);
	// driver.VMAX(200000);
	// driver.DMAX(700);
	// driver.D1(1400);
	// driver.VSTOP(10);
	// driver.RAMPMODE(0);
	// driver.XTARGET(-51200);



    //Snapshot of initial commands for xtarget in positioning mode from datasheet
    //SPI send: 0xEC000100C3; // CHOPCONF: TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (SpreadCycle)
    // SPI send: 0x9000061F0A; // IHOLD_IRUN: IHOLD=10, IRUN=31 (max. current), IHOLDDELAY=6
    // SPI send: 0x910000000A; // TPOWERDOWN=10: Delay before power down in stand still
    // SPI send: 0x8000000004; // EN_PWM_MODE=1 enables StealthChop (with default PWMCONF)
    // SPI send: 0x93000001F4; // TPWM_THRS=500 yields a switching velocity about 35000 = ca. 30RPM
    // SPI sample sequence to enable and initialize the motion controller and move one rotation (51200
    // microsteps) using the ramp generator. A read access querying the actual position is also shown.
    // SPI send: 0xA4000003E8; // A1 = 1 000 First acceleration
    // SPI send: 0xA50000C350; // V1 = 50 000 Acceleration threshold velocity V1
    // SPI send: 0xA6000001F4; // AMAX = 500 Acceleration above V1
    // SPI send: 0xA700030D40; // VMAX = 200 000
    // SPI send: 0xA8000002BC; // DMAX = 700 Deceleration above V1
    // SPI send: 0xAA00000578; // D1 = 1400 Deceleration below V1
    // SPI send: 0xAB0000000A; // VSTOP = 10 Stop velocity (Near to zero)
    // SPI send: 0xA000000000; // RAMPMODE = 0 (Target position move)
    // // Ready to move!
    // SPI send: 0xADFFFF3800; // XTARGET = -51200 (Move one rotation left (200*256 microsteps)
    // // Now motor 1 starts rotating
    // SPI send: 0x2100000000; // Query XACTUAL – The next read access delivers XACTUAL
    // SPI read; // Read XACTUAL

}


int main() {
    spi_master_t spi_ctx;

    spi_master_init(&spi_ctx, cb, p_ss, p_sclk, p_mosi, p_miso);

    spi_master_device_t spi_dev_0;

    TMC5160Stepper* driver = new TMC5160Stepper(Rsense, &spi_dev_0, &spi_ctx,
        0,
        0, 0,  //cpol, cpha
        spi_master_source_clock_xcore,
        75, //Was 75 600Mhz/(4 * 75) = 2MHz
        spi_master_sample_delay_0,
        0, 0 ,0 ,0);

    setup_driver(driver, WRIST_ROTATION_CURRENT);
    
    port_enable(led);
    // port_enable(p_sclk);

    while(1) {
        auto xactual = driver->XACTUAL();
        auto xtarget = driver->XTARGET();

        // //std::cout<<"ioin="<<driver->IOIN()<<" xactual="<<xactual<<"\n";
        std::cout << "xtarget" << xtarget << std::endl;
        std::cout << "xactual " << xactual << std::endl;
    
        // if (xactual == xtarget) 
        //     {
        //     driver->XTARGET(-xactual);
        //     }

        // port_out(led, 0b1100);
        // delay_milliseconds(100);
        // port_out(led, 0b0000);
        // delay_milliseconds(100);

        // port_out(p_sclk, 0);
        // delay_microseconds(10);
        // port_out(p_sclk, 1);
        // delay_microseconds(10);

    }

    // port_disable(p_sclk);

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