[English](README.en.md) | [日本語](README.md)

# MSD700 Arduino Code Management

<div align="center">
<img src="./photo/100x.png" width="300" />
</div>

Files starting with the numbers 1000 and so on indicate the unit numbers of machines that have been completed and shipped for demonstration. <br />


The files in the tunnel folder are used for tunnel inspections developed in collaboration with Nishimatsu Construction Co. <br />
Unit 1004 is not included in the numbering because Unit 1004 is the model for this tunnel.


In the test folder, you will find the code to test pcb.


The old programs are stored locally on the Nakayama server and only can be accessed through the office's local network. They are excluded from git management.


## Installation
You can download it by `Download ZIP` in an arbitrary directory or by `git clone`.
Open `.ino` with the Arduino IDE; the installation of the Arduino IDE is not described here.
```
git clone https://github.com/nakayama-software/arduino_programs.git
```


## Usage

### Writing a program

Select `Arduino Mega or Mega2560` in Arduino IDE and write the program by specifying the port.