# ECE 528/L - Lab 4 Wireless Control
**CSU Northridge**

**Department of Electrical and Computer Engineering**

# Overview

This lab introduces the use of Universal Asynchronous Receiver/Transmitter or UART. This in combination with a Bluetooth Low Energy module will be used to operate a robot via mobile application. Configuring eUSCI on the MSP432 microcontroller to support UART communication through selecting the correct operation from the pins initialization will help us get there. Then from here, developing a driver that communicates between the Bluefruit LE module to allow sending and receiving of data wirelessly to then ultimately control motor movements of the robot.

# Components Used

- MSP432 LaunchPad
- USB-A to Micro-USB Cable
- TI-RSLK MAX Chassis
- Adafruit BLE UART Module
- PMOD SWT
- Oscilloscope Probes

# Results and Analysis

The MSP432 correctly sent bytes of data to the BLE module which was verified through oscilloscope probing, screenshots of such packets shown below. After verification, testing of the packet reads to then move the robot were done in which the MSP432 read correctly. This in turn allowed for simple motor control via a control pad on the app.

**Insert Screenshots

# Known Issues or Limitations

The device had a very low duty cycle which I incorrectly implemented so the robot moved at very low speeds. Adjusting this in code allowed for proper movement and speed.

# References

- MSP432P4xx SimpleLink Microcontrollers Technical Reference Manual

- Adafruit Bluefruit LE UART Friend Module

- Adafruit Bluefruit LE UART Friend Module Tutorial

- Bluefruit LE Connect User Guide

- Lab 4 PDF

# Pre-Lab

1. What is Bluetooth Low Energy (BLE)? How does it differ from regular Bluetooth in terms of power consumption and range?

Bluetooth Low Energy is a module that brings Bluetooth communication into embedded systems with a iOS or Android phone. It sends the signals received from Bluetooth over a UART interface. This module is also configurable to the users request.

Adafruit states that the range of their BLEs are up to 100 meters but optimal working ranges are 10 meters and below, however regular Bluetooth can handle over 100m with less challenge. In terms of power consumption the BLE module uses much less power compared to the regular Bluetooth module that can reach up to 100mW. Depending on the application, the BLE tends to use only a fraction of that.

2. What does GAP stand for and what is its purpose? What is the difference between Peripheral and Central devices in the context of BLE?

GAP stands for Generic Access Profile, it is used for controlling connections and advertising Bluetooth. In a sense it makes your Bluetooth device known. 

Peripheral Devices: Small, low power device that is able to connect to a much more power central device. These are more specific in functionality, for example some devices are a heart rate monitor and BLE enabled proximity tag.

Central Devices: These are the devices we use everyday like a phone, tablet, or even a computer/laptop that possesses the processing power and memory far greater than a peripheral.


3. What is GATT in BLE, and how does it structure data exchange between connected devices? Provide definitions for the following: Profile, Service, and Characteristic.

GATT stands for Generic ATTribute, its structure for data exchange between two devices involves using Services and Characteristics through a Attribute Protocol (ATT) through a lookup table. This occurs only when a connection is made between the devices. 

Profiles: A pre-defined collection of services that has been compiled by either the Bluetooth SIG or the designers of the peripheral. 

Services: These will begin to break up data into logical entities that hold chunks of data that represent characteristics. Each service though has a specific ID that separates itself from other services which allows them to remain unique.

Characteristics: The data of the specific device that has its own UUID to distinguish itself. It carries out measured data for the peripherals, for example it provides heart rate data that matches the selected data format which is also provided by the characteristic.


4. What is an advertising packet in BLE and what information can it contain?

Advertising packets are part of the advertising process for a central device to see information regarding the peripheral which in this case is using the BLE. It ultimately lets a central device know that it is in range. The central device can also see this packet and choose to request a scan of the device.

The data you can expect to see involves device name, model, brand, etc., any information that a developer wants to provide through the communication.  


5. What is a UUID used for in BLE and why are 128-bit UUIDs used? You can use this page as a reference.

UUID is a unique numeric ID used to define information in computer systems. Each UUID holds characteristics from those systems like in the hear rate monitor, the UUID contains heart rate measurement, body sensor location, and heart rate control point. Not all the information needs to be used. A 128-bit is used for custom services but can also be represented in many different ways while also making it nearly impossible to duplicate.



6. What bit from which register determines if the UART data is received or transmitted MSB or LSB first? Refer to the eUSCI_A UART Registers section (24.4) of the MSP432P4xx Technical Reference Manual (SLAU356I).

The UCMSB bit from UCAxCTLW0 register is the bit that will define whether the data received or transmitted is MSB or LSB first.


7. Provide the mnemonic and primary module function for each pin listed. Refer to Table 6-45 (Default Mapping) on page 126 of the MSP432P401R Datasheet

P2.2: Mnemonic = PM_UCA1RXD/PM_UCA1SOMI	

Input Pin Primary Functions = eUSCI_A1 UART RXD (direction controlled by eUSCI–Input) eUSCI_A1 SPI slave out master in (direction controlled by eUSCI)

Output Pin Primary Functions: eUSCI_A1 UART RXD (direction controlled by eUSCI–Input) eUSCI_A1 SPI slave out master in (direction controlled by eUSCI)

P2.3: Mnemonic = PM_UCA1TXD/PM_UCA1SIMO

Input Pin Primary Functions = eUSCI_A1 UART TXD (direction controlled by eUSCI– output)/eUSCI_A1 SPI slave in master out (direction controlled by eUSCI)

Output Pin Primary Functions: eUSCI_A1 UART TXD (direction controlled by eUSCI– output)/eUSCI_A1 SPI slave in master out (direction controlled by eUSCI)

P3.2: Mnemonic = PM_UCA2RXD/PM_UCA2SOMI

Input Pin Primary Functions:eUSCI_A2 UART RXD (direction controlled by eUSCI– input)/eUSCI_A2 SPI slave out master in (direction controlled by eUSCI)

Output Pin Primary Functions:eUSCI_A2 UART RXD (direction controlled by eUSCI– input)/eUSCI_A2 SPI slave out master in (direction controlled by eUSCI)

P3.3: Mnemonic = PM_UCA2TXD/PM_UCA2SIMO

Input Pin Primary Functions: eUSCI_A2 UART TXD (direction controlled by eUSCI– output)/eUSCI_A2 SPI slave in master out (direction controlled by eUSCI)

Output Pin Primary Functions: eUSCI_A2 UART TXD (direction controlled by eUSCI– output)/eUSCI_A2 SPI slave in master out (direction controlled by eUSCI)


void P2_2_and_P2_3_Init(void)

{

	P2->SEL0 |= 0x0C;
	P2->SEL1 &= ~0x0C;

}

References:
https://learn.adafruit.com/alltheiot-transports/bluetooth-btle

https://developerhelp.microchip.com/xwiki/bin/view/applications/ble/introduction/

https://en.wikipedia.org/wiki/Universally_unique_identifier