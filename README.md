# !!!WARNING!!!
**!!!DO NOT USE THIS FIRMWARE IF USING THE M4-V3 OR OLDER HOTEND!!!**
## ADDED FEATURES: 
1. Added PETG to Preheat Menu (Requires modified Language.JPG file in TFT firmware)
2. Changed Mixing Max Temp to 260. 

## ZONESTAR 3D Printer Firmware Source Code
###  Configured machines list   
|     Config     |         Model       |    Control board    |  
|----------------|---------------------|---------------------|  
|  	Z9V5   	  |    Z9V5(Pro)-MK1(2) |       ZM3E4         |  
|  Z9V5_MK3      |    Z9V5Pro-MK3      |       ZM3E4         |
|  Z9V5_MK4      |    Z9V5Pro-MK4      |       ZM3E4         |


---
## How to build firmware
### 1. Prepare the compilation environment
1. Install "Visual Studio Code: Visit the [***Setting up Visual Studio Code***](https://code.visualstudio.com/docs/setup/setup-overview) page to download and install the latest Visual Studio Code for your particular platform.  
2. Install the PlatformIO IDE: Head over to the [***Get PlatformIO IDE***](https://platformio.org/install/ide?install=vscode) page to learn how to install PlatformIO IDE in Visual Studio Code.  
**More detail, please refer to** [***Install marlin compilation environment***](https://marlinfw.org/docs/basics/install_platformio_vscode.html)  

### 2. Prepare firmware source code
1. Download firmware source code to your PC and unzip it.  
![](./config/download.jpg)    
2. Choose and replace config files  
   - Find the configuration files of your machine from the "config" directory  
   - Copy and replace configuration.h and configuration_adv.h to "marlin" directory  
   - Copy and replace platformio.ini to root directory of the source code  
   - Modify "configuration.h" and "configuration_adv.h" according to the setting of your machine if need.  
3. Open the project in Visual Stuido Code  
Use the "Open Folder…" command in the Visual Studio Code "File" menu to open the source code directory which you stored, e.g. "main".  

### 3. Compile firmware
1. Run the "Run build task" command in the Visual Studio Code "Terminal" menu, and then choose "PlatformIO:build" task to build the firmware bin (or Hex) file.
   - **32bits control board(ZM3E2/ZM3E4)**: firmware.bin file.
   - **8bits control board(ZMIB/ZRIB)**: firmware.hex file.
2. Wait for the build finished.
3. Find the *firmware.bin (32-bits control board)* or *firmware.HEX (8-bits control board)* file in the ".pio/build/{board}" of the source code directory

---
## Upload firmware to your 3d printer
1. For 32 bits control board, you can copy the *firmware.bin* file to SD card and upload the firmware from the SD card.
2. For 8bits control board, you need to upload firmwre to the control board by PC (from USB cable), about detail, please refer to [**upload firmware**](https://github.com/ZONESTAR3D/firmware/readme.md)

---
## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
