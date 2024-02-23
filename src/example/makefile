

# main.xe: main.o configs.o spi.o steppers.o Stepper.o 
# 	xcc -w -std=c++11 main.o configs.o spi.o steppers.o Stepper.o -o main.xe

# main.o: main.xc
# 	xcc -w -std=c++11 -c main.xc -O2 -target=XU316-1024-QF60A-C24 -o main.o

# configs.o: ../arduino/TMCStepper-master/src/source/CHOPCONF.cpp ../arduino/TMCStepper-master/src/source/COOLCONF.cpp ../arduino/TMCStepper-master/src/source/DRV_CONF.cpp ../arduino/TMCStepper-master/src/source/DRV_STATUS.cpp ../arduino/TMCStepper-master/src/source/DRVCONF.cpp ../arduino/TMCStepper-master/src/source/DRVCTRL.cpp ../arduino/TMCStepper-master/src/source/DRVSTATUS.cpp ../arduino/TMCStepper-master/src/source/ENCMODE.cpp ../arduino/TMCStepper-master/src/source/GCONF.cpp ../arduino/TMCStepper-master/src/source/IHOLD_IRUN.cpp ../arduino/TMCStepper-master/src/source/PWMCONF.cpp ../arduino/TMCStepper-master/src/source/RAMP_STAT.cpp ../arduino/TMCStepper-master/src/source/SGCSCONF.cpp ../arduino/TMCStepper-master/src/source/SHORT_CONF.cpp ../arduino/TMCStepper-master/src/source/SMARTEN.cpp
# 	xcc -w -std=c++11 -c $^ -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o configs.o

# spi.o: ../arduino/TMCStepper-master/src/source/spi_master.c ../arduino/TMCStepper-master/src/source/spi.h
# 	xcc -w ../arduino/TMCStepper-master/src/source/spi_master.c -O2 -target=XU316-1024-QF60A-C24 -o spi.o

# steppers.o: ../arduino/TMCStepper-master/src/source/SW_MODE.cpp ../arduino/TMCStepper-master/src/source/TMC_MACROS.h ../arduino/TMCStepper-master/src/source/TMC2130_bitfields.h ../arduino/TMCStepper-master/src/source/TMC2130Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC2160_bitfields.h ../arduino/TMCStepper-master/src/source/TMC2160Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC5130_bitfields.h ../arduino/TMCStepper-master/src/source/TMC5130Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC5160_bitfields.h ../arduino/TMCStepper-master/src/source/TMC5160Stepper.cpp
# 	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/SW_MODE.cpp ../arduino/TMCStepper-master/src/source/TMC2130Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC2160Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC5130Stepper.cpp ../arduino/TMCStepper-master/src/source/TMC5160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o steppers.o

# Stepper.o: ../arduino/TMCStepper-master/src/source/TMCStepper.cpp ../arduino/TMCStepper-master/src/source/TMCStepper.h
# 	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMCStepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o Stepper.o

main.xe: main.o CHOPCONF.o COOLCONF.o DRV_CONF.o DRV_STATUS.o DRVCONF.o DRVCTRL.o DRVSTATUS.o ENCMODE.o GCONF.o IHOLD_IRUN.o PWMCONF.o RAMP_STAT.o SGCSCONF.o SHORT_CONF.o SMARTEN.o spi.o SW_MODE.o TMC2130Stepper.o TMC2160Stepper.o TMC5130Stepper.o TMC5160Stepper.o Stepper.o
	xcc -w -std=c++11 main.o CHOPCONF.o COOLCONF.o DRV_CONF.o DRV_STATUS.o DRVCONF.o DRVCTRL.o DRVSTATUS.o ENCMODE.o GCONF.o IHOLD_IRUN.o PWMCONF.o RAMP_STAT.o SGCSCONF.o SHORT_CONF.o SMARTEN.o spi.o SW_MODE.o TMC2130Stepper.o TMC2160Stepper.o TMC5130Stepper.o TMC5160Stepper.o Stepper.o -O2 -target=XU316-1024-QF60A-C24 -o main.xe

main.o: main.xc
	xcc -w -std=c++11 -c main.xc -O2 -target=XU316-1024-QF60A-C24 -o main.o

CHOPCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/CHOPCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o CHOPCONF.o

COOLCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/COOLCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o COOLCONF.o

DRV_CONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/DRV_CONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o DRV_CONF.o

DRV_STATUS.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/DRV_STATUS.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o DRV_STATUS.o

DRVCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/DRVCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o DRVCONF.o

DRVCTRL.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/DRVCTRL.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o DRVCTRL.o

DRVSTATUS.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/DRVSTATUS.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o DRVSTATUS.o

ENCMODE.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/ENCMODE.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o ENCMODE.o

GCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/GCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o GCONF.o

IHOLD_IRUN.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/IHOLD_IRUN.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o IHOLD_IRUN.o

PWMCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/PWMCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o PWMCONF.o

RAMP_STAT.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/RAMP_STAT.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o RAMP_STAT.o

SGCSCONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/SGCSCONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o SGCSCONF.o

SHORT_CONF.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/SHORT_CONF.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o SHORT_CONF.o

SMARTEN.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/SMARTEN.cpp -I../arduino/TMCStepper-master/src/ -O2 -target=XU316-1024-QF60A-C24 -o SMARTEN.o

spi.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/spi_master.c -O2 -target=XU316-1024-QF60A-C24 -o spi.o

SW_MODE.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/SW_MODE.cpp -O2 -target=XU316-1024-QF60A-C24 -o SW_MODE.o

TMC2130Stepper.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMC2130Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC2130Stepper.o

TMC2160Stepper.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMC2160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC2160Stepper.o

TMC5130Stepper.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMC5130Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC5130Stepper.o

TMC5160Stepper.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMC5160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC5160Stepper.o

Stepper.o:
	xcc -w -std=c++11 -c ../arduino/TMCStepper-master/src/source/TMCStepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o Stepper.o
