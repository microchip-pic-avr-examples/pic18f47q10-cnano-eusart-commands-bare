/**
 *  @copyright © 2024 Microchip Technology Inc. and its subsidiaries.
 *
 *  Subject to your compliance with these terms, you may use Microchip software
 *  and any derivatives exclusively with Microchip products. You're responsible
 *  for complying with 3rd party license terms applicable to your use of 3rd
 *  party software (including open source software) that may accompany
 *  Microchip software.
 *
 *  SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 *  APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF
 *  NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 *  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 *  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 *  BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 *  FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 *  RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY, YOU PAID
 *  DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 **/


/* Disable Watch Dog Timer */
#pragma config WDTE = OFF
/* Low voltage programming enabled , RE3 pin is MCLR */
#pragma config LVP = ON


#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#define MAX_COMMAND_LEN    ( 8 )


static void CLK_init(void);
static void PORT_init(void);
static void PPS_init(void);
static void EUSART2_init(void);

void EUSART2_write(uint8_t txData);
uint8_t EUSART2_read(void);

void executeCommand(char const * const command);


void main(void)
{
    char command[MAX_COMMAND_LEN];
    uint8_t index = 0;
    char c;

    CLK_init();
    PORT_init();
    PPS_init();
    EUSART2_init();

    /* LED is ON */
    LATEbits.LATE0 = 0;

    while(1)
    {
        c = (char) getch();
        if(c != '\n' && c != '\r')
        {
            command[index++] = c;
            if(index > MAX_COMMAND_LEN)
            {
                index = 0;
            }
        }

        if(c == '\n')
        {
            command[index] = '\0';
            index = 0;
            executeCommand(command);
        }
    }
}

static void CLK_init(void)
{
    /* Set HFINTOSC as new oscillator source */
    OSCCON1bits.NOSC = 0b110;

    /* Set HFFRQ to 1 MHz */
    OSCFRQbits.HFFRQ = 0;
}

static void PORT_init(void)
{
    /* Configure RD0 as output. */
    TRISDbits.TRISD0 = 0;

    /* Configure RD1 as input. */
    TRISDbits.TRISD1 = 1;

    /* Configure RE0 as output. */
    TRISEbits.TRISE0 = 0;

    /* Enable RD1 digital input buffers.*/
    ANSELDbits.ANSELD1 = 0;
}

static void PPS_init(void)
{
    /* RD0 is TX2 */
    RD0PPS = 0x0B;
    /* RX2 is RD1 */
    RX2PPS = 0b00011001;
}

void EUSART2_init(void)
{
    /* 16-bit Baud Rate Generator is used */
    BAUD2CONbits.BRG16 = 1;

    /* Baud rate 9600 */
    SP2BRGL = 25;
    SP2BRGH = 0;

    /* Transmit Enable */
    TX2STAbits.TXEN = 1;
    /* High Baud Rate Select */
    TX2STAbits.BRGH = 1;

    /* Continuous Receive Enable */
    RC2STAbits.CREN = 1;
    /* Serial Port Enable */
    RC2STAbits.SPEN = 1;
}

void EUSART2_write(uint8_t txData)
{
    while(0 == PIR3bits.TX2IF)
    {
        ;
    }

    TX2REG = txData;
}

uint8_t EUSART2_read(void)
{
    while(0 == PIR3bits.RC2IF)
    {
        ;
    }

    return RC2REG;
}

int getch(void)
{
    return EUSART2_read();
}

void putch(char txData)
{
    EUSART2_write(txData);
}

void executeCommand(char const * const command)
{
    if(strcmp(command, "ON") == 0)
    {
        LATEbits.LATE0 = 0;
        printf("OK, LED ON.\r\n");
    }
    else if (strcmp(command, "OFF") == 0)
    {
        LATEbits.LATE0 = 1;
        printf("OK, LED OFF.\r\n");
    }
    else
    {
        printf("Incorrect command.\r\n");
    }
}
