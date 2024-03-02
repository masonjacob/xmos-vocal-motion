# URI ELECOMP 2023-2024 - Team XMOS: Project Vocal Motion
----------------------------------------------------------------
## Getting Started

Wake Word:
    - XMOS Board
    
Command Set:
    - Move Right
    - Move Left
    - Move Up
    - Move Down
    - Blink Once
    - Blink Five Times
    - SOS

## Docker Dev Enviroment
This project is configured with a Docker multi-container environment to facilitate cross platform development. The environment is comprised of the following services:
- **xmos-dev**: contains XTC tools and the sln_voice repositroy, located in `/opt/XMOS`, as well as a bind mount to the src folder and a shared volume `/shared`, located in `/home`.
- **gtkwave**: automatically runs the gtkwave application used to debug simulated signal outputs. the `/shared` volume is mounted in the top-level directory to make it easy to tranfer output files to the application.

### Prerequisites

The following must be installed before you can run the dev environment:
- Docker
- VS-Code & Dev Containers Extension
- WSL2 (Windows Only)
- winget (Windows Only, Windows Package Manager)
- USBIPD-WIN (Windows Only, installed with `winget install --interactive --exact dorssel.usbipd-win` in Powershell)

### Using the Dev Environment

The dev enviroment can be build and run with the following command:
```
docker-compose up -d
```

After all containers are built and deployed, open VS-Code and do the following:
- open the command palette using `CTRL+SHIFT+P`. 
- Search for and select `Dev Containers: Attach to Running Container...`. 
- select the running `xmos-dev` container from the dropdown list

VS-Code will now open a new window attached to the running xmos-dev container. Select the "Open Folder" to open the `/src` bind mount, located under `/home`. 

Congrats! You have a running dev enviroment and are ready to program and flash the XCORE platform.

### Flashing a Dev Board with XTAG

#### Linux:
The xmos-dev container bind mounts `/dev/bus/usb`, so USB devices should be accessible in the xmos-dev container automatically. Run `lsusb` in the container to verify.

#### Windows:
To flash a dev board using the XTAG on a Windows host machine, the USB device must be tunneled into the container. 

1. Install USBIPD-WIN by running the following in Powershell:
```
winget install --interactive --exact dorssel.usbipd-win
```

2. Open a new WSL2 Terminal window and run the following:
```
sudo apt install linux-tools-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip $(command -v ls /usr/lib/linux-tools/*/usbip | tail -n1) 20
```

3. Connect the XTAG to the computer using the provided USB cable. 

4. Open Powershell as administrator, and run `usbipd wsl list` to list all USB devices. 

5. Select the bus ID of the XTAG and attach it to WSL2 with the following command:
```
usbipd wsl attach --busid <busid>
```

6. The XTAG is now connected to WSL2. To list the attached devices, use `lsusb` in a WSL2 terminal. The docker-compose for this dev enviroment bind mounts the `/dev/bus/usb` directory to the `xmos-dev` container, so once the USB device is mounted in WSl2 it is also mounted in the xmos-dev container.
