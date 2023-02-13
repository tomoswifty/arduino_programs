[English](README.en.md) | [日本語](README.md)

# MSD700 Arduino Code Management

<div align="center">
<img src="./photo/100x.png" width="300" />
</div>


Each number before "\_msd700" represents an individual machine unit number. <br />
The code for 1000 in particular has been rewritten as a template numbering for the machine unit number.

Files starting with the numbers 1000 and so on indicate the unit numbers of machines that have been completed and shipped for demonstration. <br />
Files starting with tunnel_ are used for tunnel inspections unit (1004) and developed in collaboration with Nishimatsu Construction Co.

Unit 1004 is not included in the numbering because it is the unit number of the tunnel model. <br />
The old programs are stored locally on the Nakayama server and only can be accessed through the office's local network. They are excluded from git management.

## Installation
Download the software by `git clone` in an arbitrary directory.
Open `.ino` with Arduino IDE.

## Writing a program
Select `Arduino Mega or Mega2560` in Arduino IDE and write the program with specified port.
