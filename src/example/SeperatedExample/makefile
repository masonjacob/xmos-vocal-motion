

# main.xe: main.o configs.o spi.o steppers.o Stepper.o 
# 	xcc -w -std=c++11 main.o configs.o spi.o steppers.o Stepper.o -o main.xe

# main.o: main.xc
# 	xcc -w -std=c++11 -c main.xc -O2 -target=XU316-1024-QF60A-C24 -o main.o

# configs.o: CHOPCONF.cpp COOLCONF.cpp DRV_CONF.cpp DRV_STATUS.cpp DRVCONF.cpp DRVCTRL.cpp DRVSTATUS.cpp ENCMODE.cpp GCONF.cpp IHOLD_IRUN.cpp PWMCONF.cpp RAMP_STAT.cpp SGCSCONF.cpp SHORT_CONF.cpp SMARTEN.cpp
# 	xcc -w -std=c++11 -c $^  -O2 -target=XU316-1024-QF60A-C24 -o configs.o

# spi.o: spi_master.c spi.h
# 	xcc -w spi_master.c -O2 -target=XU316-1024-QF60A-C24 -o spi.o

# steppers.o: SW_MODE.cpp TMC_MACROS.h TMC2130_bitfields.h TMC2130Stepper.cpp TMC2160_bitfields.h TMC2160Stepper.cpp TMC5130_bitfields.h TMC5130Stepper.cpp TMC5160_bitfields.h TMC5160Stepper.cpp
# 	xcc -w -std=c++11 -c SW_MODE.cpp TMC2130Stepper.cpp TMC2160Stepper.cpp TMC5130Stepper.cpp TMC5160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o steppers.o

# Stepper.o: TMCStepper.cpp TMCStepper.h
# 	xcc -w -std=c++11 -c TMCStepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o Stepper.o

main.xe: main.o CHOPCONF.o COOLCONF.o DRV_CONF.o DRV_STATUS.o DRVCONF.o DRVCTRL.o DRVSTATUS.o ENCMODE.o GCONF.o IHOLD_IRUN.o PWMCONF.o RAMP_STAT.o SGCSCONF.o SHORT_CONF.o SMARTEN.o spi.o SW_MODE.o TMC2130Stepper.o TMC2160Stepper.o TMC5130Stepper.o TMC5160Stepper.o Stepper.o
	xcc -w -std=c++11 main.o CHOPCONF.o COOLCONF.o DRV_CONF.o DRV_STATUS.o DRVCONF.o DRVCTRL.o DRVSTATUS.o ENCMODE.o GCONF.o IHOLD_IRUN.o PWMCONF.o RAMP_STAT.o SGCSCONF.o SHORT_CONF.o SMARTEN.o spi.o SW_MODE.o TMC2130Stepper.o TMC2160Stepper.o TMC5130Stepper.o TMC5160Stepper.o Stepper.o -O2 -target=XU316-1024-QF60A-C24 -o main.xe

main.o: main.cpp
	xcc -w -std=c++11 -c main.cpp -O2 -target=XU316-1024-QF60A-C24 -o main.o

CHOPCONF.o:
	xcc -w -std=c++11 -c CHOPCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o CHOPCONF.o

COOLCONF.o:
	xcc -w -std=c++11 -c COOLCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o COOLCONF.o

DRV_CONF.o:
	xcc -w -std=c++11 -c DRV_CONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o DRV_CONF.o

DRV_STATUS.o:
	xcc -w -std=c++11 -c DRV_STATUS.cpp  -O2 -target=XU316-1024-QF60A-C24 -o DRV_STATUS.o

DRVCONF.o:
	xcc -w -std=c++11 -c DRVCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o DRVCONF.o

DRVCTRL.o:
	xcc -w -std=c++11 -c DRVCTRL.cpp  -O2 -target=XU316-1024-QF60A-C24 -o DRVCTRL.o

DRVSTATUS.o:
	xcc -w -std=c++11 -c DRVSTATUS.cpp  -O2 -target=XU316-1024-QF60A-C24 -o DRVSTATUS.o

ENCMODE.o:
	xcc -w -std=c++11 -c ENCMODE.cpp  -O2 -target=XU316-1024-QF60A-C24 -o ENCMODE.o

GCONF.o:
	xcc -w -std=c++11 -c GCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o GCONF.o

IHOLD_IRUN.o:
	xcc -w -std=c++11 -c IHOLD_IRUN.cpp  -O2 -target=XU316-1024-QF60A-C24 -o IHOLD_IRUN.o

PWMCONF.o:
	xcc -w -std=c++11 -c PWMCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o PWMCONF.o

RAMP_STAT.o:
	xcc -w -std=c++11 -c RAMP_STAT.cpp  -O2 -target=XU316-1024-QF60A-C24 -o RAMP_STAT.o

SGCSCONF.o:
	xcc -w -std=c++11 -c SGCSCONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o SGCSCONF.o

SHORT_CONF.o:
	xcc -w -std=c++11 -c SHORT_CONF.cpp  -O2 -target=XU316-1024-QF60A-C24 -o SHORT_CONF.o

SMARTEN.o:
	xcc -w -std=c++11 -c SMARTEN.cpp  -O2 -target=XU316-1024-QF60A-C24 -o SMARTEN.o

spi.o: 
	xcc -w -c spi_master.c -O2 -target=XU316-1024-QF60A-C24 -o spi.o

# spi.o: 
# 	gcc -c spi_master.c -o spi.o


SW_MODE.o:
	xcc -w -std=c++11 -c SW_MODE.cpp -O2 -target=XU316-1024-QF60A-C24 -o SW_MODE.o

TMC2130Stepper.o:
	xcc -w -std=c++11 -c TMC2130Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC2130Stepper.o

TMC2160Stepper.o:
	xcc -w -std=c++11 -c TMC2160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC2160Stepper.o

TMC5130Stepper.o:
	xcc -w -std=c++11 -c TMC5130Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC5130Stepper.o

TMC5160Stepper.o:
	xcc -w -std=c++11 -c TMC5160Stepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o TMC5160Stepper.o

Stepper.o:
	xcc -w -std=c++11 -c TMCStepper.cpp -O2 -target=XU316-1024-QF60A-C24 -o Stepper.o
