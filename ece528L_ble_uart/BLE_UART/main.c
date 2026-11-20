/**
 * @file main.c
 *
 * @brief Main source code for the BLE_UART program.
 *
 * This file contains the main entry point for the BLE_UART program,
 * which is used to demonstrate the BLE_UART driver.
 *
 * It interfaces with the Adafruit Bluefruit LE UART Friend Bluetooth Low Energy (BLE) module, which uses the UART communication protocol.
 *  - Product Link: https://www.adafruit.com/product/2479
 *
 * @note For more information regarding the Enhanced Universal Serial Communication Interface (eUSCI),
 * refer to the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @author Daniel Khrlobian
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "msp.h"

#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/BLE_UART.h"

#define PWM_NOMINAL 3000
#define FW 0xAA
#define SW 0x55
#define TW 0xFF
#define FTW 0x00

void Process_BLE_UART_Data(char BLE_UART_Buffer[]){
    if(Check_BLE_UART_Data(BLE_UART_Buffer, "RGB LED GREEN")){
        LED2_Output(RGB_LED_GREEN);
    }

    else if (Check_BLE_UART_Data(BLE_UART_Buffer, "RGB LED OFF")){
        LED2_Output(RGB_LED_OFF);
    }

    else{
        printf("BLE UART Command Not Found\n");
    }
}


    void Decode_Controller_Packet(char BLE_UART_Buffer[]) {
        if (BLE_UART_Buffer[0] == 0x21 && BLE_UART_Buffer[1] == 0x42){
            if (BLE_UART_Buffer[2] == 0x35){
                if (BLE_UART_Buffer[3] == 0x31){
                    Motor_Forward(4500, 4500);
                    printf("UP BUTTON\n");
                    }
                else {
                    Motor_Stop();
                    }
            }

            else if (BLE_UART_Buffer[2] == 0x36){
                if (BLE_UART_Buffer[3] == 0x31){
                    Motor_Backward(4500, 4500);
                    printf("DOWN BUTTON\n");
                    }
                else {
                    Motor_Stop();
                    }
            }

            else if (BLE_UART_Buffer[2] == 0x37){
                if (BLE_UART_Buffer[3] == 0x31){
                    Motor_Left(4500, 4500);
                    printf("LEFT BUTTON\n");
                    }
                else {
                    Motor_Stop();
                    }
            }

            else if (BLE_UART_Buffer[2] == 0x38){
                if (BLE_UART_Buffer[3] == 0x31){
                    Motor_Right(4500, 4500);
                    printf("RIGHT BUTTON\n");
                    }
                else {
                    Motor_Stop();
                    }
            }

            else{
                printf("Packet Read Error\n");
            }
        }
    }


int main(void)
{
    DisableInterrupts();

    Clock_Init48MHz();

    LED2_Init();

    EUSCI_A0_UART_Init_Printf();

    BLE_UART_Init();

    Motor_Init();

    // Initialize a buffer that will be used to receive command string from the BLE module
    char BLE_UART_Buffer[BLE_UART_BUFFER_SIZE] = {0};

    EnableInterrupts();

    Clock_Delay1ms(1000);
    BLE_UART_Reset();

    // Send a message to the BLE module to check if the connection is stable
    BLE_UART_OutString("BLE UART Active\r\n");
    Clock_Delay1ms(1000);

    while(1)
    {
       int packet_size = BLE_UART_InPacket(BLE_UART_Buffer, BLE_UART_BUFFER_SIZE);

       if (packet_size == 5) {
           printf("BLE UART Data: ");

           for (int i = 0; i < packet_size; i++) {
               printf("%02X", (unsigned int)(uint8_t)BLE_UART_Buffer[i]);
           }
           printf("\n");

           Decode_Controller_Packet(BLE_UART_Buffer);
       }
    }
}

/*
 * OLD MAIN
 * int string_size = BLE_UART_InPacket(BLE_UART_Buffer, BLE_UART_BUFFER_SIZE);
        printf("BLE UART Data: ");

        for (int i = 0; i < string_size; i++) {
            printf("%c", BLE_UART_Buffer[i]);
        }

        print("\n");

        Decode_Controller_Packet(BLE_UART_Buffer);

 */

/*  Part A)
 *  void Transmit_Byte(){
 *      BLE_UART_OutChar(FW);
 *      Clock_Delay1ms(1000);
 *      BLE_UART_OutChar(SW);
 *      Clock_Delay1ms(1000);
 *      BLE_UART_OutChar(TW);
 *      Clock_Delay1ms(1000);
 *      BLE_UART_OutChar(FTW);
 *      Clock_Delay1ms(1000);
 *  }
 *
 *  Part B)
 *  void Transmit_Byte(){
 *      uint8_t PMOD_SWT_Status = Get_PMOD_SWT_Status(void);
 *      switch (PMOD_SWT_Status) {
 *          case 0x01:
 *              BLE_UART_OutChar(0xAA);
 *              break;
 *          case 0x02:
 *              BLE_UART_OutChar(0x10);
 *              break;
 *          case 0x04:
 *              BLE_UART_OutChar(0xF0);
 *              break;
 *          case 0x08:
 *              BLE_UART_OutChar(0x0F);
 *              break;
 *          default:
 *              BLE_UART_OutChar(0x00);
 *      }
 *  }
 *
 *  Part C)
 *  int BLE_UART_InPacket(char *buffer_pointer, uint16_t buffer_size){
 *      char character;

        if (buffer_pointer == NULL || buffer_size < 6)
        {
            return 0;
        }

        do {
            character = BLE_UART_InChar();
 *      } while (character != 0x21);
 *
 *      buffer_pointer[0] = character;
 *      for (int i = 1; i < 5; i++){
 *          buffer_pointer[i] = BLE_UART_InChar();
 *      }
 *
 *      buffer_pointer[5] = '\0';
 *      return 5;
 *
 */
