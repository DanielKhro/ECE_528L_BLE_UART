/**
 * @file BLE_UART.c
 *
 * @brief Source code for the BLE_UART driver.
 *
 * This file contains the function definitions for the BLE_UART driver.
 *
 * It interfaces with the Adafruit Bluefruit LE UART Friend Bluetooth Low Energy (BLE) module, which uses the UART communication protocol.
 *  - Product Link: https://www.adafruit.com/product/2479
 *
 * The following connections must be made:
 *  - BLE UART MOD  (Pin 1)     <-->  MSP432 LaunchPad Pin P1.6
 *  - BLE UART CTS  (Pin 2)     <-->  MSP432 LaunchPad GND
 *  - BLE UART TXO  (Pin 3)     <-->  MSP432 LaunchPad Pin P9.6 (PM_UCA3RXD)
 *  - BLE UART RXI  (Pin 4)     <-->  MSP432 LaunchPad Pin P9.7 (PM_UCA3TXD)
 *  - BLE UART VIN  (Pin 5)     <-->  MSP432 LaunchPad VCC (3.3V)
 *  - BLE UART RTS  (Pin 6)     <-->  Not Connected
 *  - BLE UART GND  (Pin 7)     <-->  MSP432 LaunchPad GND
 *  - BLE UART DFU  (Pin 8)     <-->  Not Connected
 *
 * @note For more information regarding the Enhanced Universal Serial Communication Interface (eUSCI),
 * refer to the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @author
 *
 */

#include "../inc/BLE_UART.h"

void BLE_UART_Init()
{
    P9SEL0 |= 0xC0;
    P9SEL1 &= ~0xC0;
    P1SEL0 &= ~0x40;
    P1SEL1 &= ~0x40;
    P1DIR  |= 0x40;
    // Reset Held for editing
    EUSCI_A3->CTLW0 |= 0x01;
    // Clear modulation control
    EUSCI_A3->MCTLW &= ~0xFF;
    // Disable Parity
    EUSCI_A3->CTLW0 &= ~0x8000;
    // Select odd parity
    EUSCI_A3->CTLW0 &= ~0x4000;
    // Select LSB
    EUSCI_A3->CTLW0 &= ~0x2000;
    // 8 bit character length
    EUSCI_A3->CTLW0 &= ~0x1000;
    // One stop bit
    EUSCI_A3->CTLW0 &= ~0x0800;
    // Enable UART
    EUSCI_A3->CTLW0 &= ~0x0600;
    // Disable Synchronous Mode
    EUSCI_A3->CTLW0 &= ~0x0100;
    // Configure EUSCI_A3 to use SMCLK
    EUSCI_A3->CTLW0 |= 0x00C0;
    // Set the baud rate by writing to UCBRx
    // N = (Clock Frequency) / (Baud Rate) = (12,000,000 / 9600) = 1250
    EUSCI_A3->BRW   =  1250;
    // Disable the following interrupts
    // Transmit Complete Interrupt and Start bit interrupt
    EUSCI_A3->IE    &= ~0x0C;
    // Enable the following interrupts
    // Transmit Interrupt and Receive Interrupt
    EUSCI_A3->IE    |= 0x03;
    // Release the EUSCI_A3 module from the reset state
    EUSCI_A3->CTLW0 &= ~0x01;
}

uint8_t BLE_UART_InChar()
{
    // Check receive interrupt flag (UCRXIFG, Bit 0)
    // in the IFG register and wait if the flag is not set
    // If the UCRXIFG is set, then the Receive Buffer (UCAxRXBUF)
    // has received a complete character
    while((EUSCI_A3->IFG & 0x01) == 0);

    //Return the data from the Receive Buffer (UCAxRXBUF)
    // Reading the UCAxRXBUF will reset the UCRXIFG flag

    return EUSCI_A3->RXBUF;
}

void BLE_UART_OutChar(uint8_t data)
{
    //Check the transmit interrupt flag (UCTXIFG, Bit 1)
    // in the IFG register and wait if the flag is not set
    // If the UCTXIFG is set, then the Transmit Buffer (UCAxTXBUF) is empty
    while((EUSCI_A3->IFG & 0x02) == 0);

    // Write the data to the Transmit Buffer (UCAxTXBUF)
    // Writing to the UCAxTXBUF will clear the UCTXIFG flag
    EUSCI_A3->TXBUF = data;
}

int BLE_UART_InString(char *buffer_pointer, uint16_t buffer_size)
{
    int length = 0;
    int string_size = 0;

    // Read last received data from UART receive buffer
    char character = BLE_UART_InChar();

    // Check if the received character is a carriage return.
    // Otherwise if it is a valid character, then store it in
    // BLE_UART_Buffer in main.
    while(character != LF){
        //Remove the character from the buffer if the received character is a backspace character
        if (character == BS)
        {
            if (length){
                buffer_pointer--;
                length--;
                BLE_UART_OutChar(BS);
            }
        }

        // Otherwise, if there are more characters to be read, store them in the buffer
        else if (length < buffer_size){
            *buffer_pointer = character;
            buffer_pointer++;
            length++;
            string_size++;
        }
        character = BLE_UART_InChar();
    }
    *buffer_pointer = 0;
    return string_size;
}

int BLE_UART_InPacket(char *buffer_pointer, uint16_t buffer_size){
       char character;

       if (buffer_pointer == NULL || buffer_size < 6)
       {
           return 0;
           }

       do {
           character = BLE_UART_InChar();
       } while (character != 0x21);

       buffer_pointer[0] = character;
       for (int i = 1; i < 5; i++){
           buffer_pointer[i] = BLE_UART_InChar();
           }

       buffer_pointer[5] = '\0';
       return 5;
}

void BLE_UART_OutString(char *pt)
{
    while(*pt){
        BLE_UART_OutChar(*pt);
        pt++;
    }
}

uint8_t Check_BLE_UART_Data(char BLE_UART_Data_Buffer[], char *data_string)
{
    if (strstr(BLE_UART_Data_Buffer, data_string) != NULL){
        return 0x01;
    }
    else{
        return 0x00;
    }
}

void BLE_UART_Reset()
{
    // Switch to CMD mode by setting MOD pin (P1.6) to 1
    P1->OUT |= 0x40;
    Clock_Delay1ms(1000);
    // Send the system reset command by sending the "ATZ" string
    // to the BLE UART module
    BLE_UART_OutString("ATZ\r\n");
    Clock_Delay1ms(3000);

    // Switch back to DATA mode by clearing the MOD pin (P1.6) to 0
    P1->OUT &= ~0x40;
}


/*
 *
 * int BLE_UART_InPacket(char *buffer_pointer, uint16_t buffer_size){
 *  char character;

    if (buffer_pointer == NULL) {
        return 0;
    }

    do {
        character = BLE_UART_InChar();
    } while (character != 0x21);
    buffer_pointer[0] = character;

    for (int i = 1; i < 5; i++){
        buffer_pointer[i] = BLE_UART_InChar();
    }

    buffer_pointer[5] = 0;
    return 5;
 * }
 */
