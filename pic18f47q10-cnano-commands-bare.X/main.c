/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/* Disable Watch Dog Timer */
#pragma config WDTE = OFF  
/* Low voltage programming enabled , RE3 pin is MCLR */
#pragma config LVP = ON               

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define PPS_CONFIG_RD0_EUSART2_TX 0x0B
#define PPS_CONFIG_RD1_EUSART2_RX 0x19
#define BAUD_RATE_SYNC_0_BRG16_1_FOSC_1MHZ 0x19
#define MAX_COMMAND_LEN 8

static void CLK_init(void);
static void EUSART2_init(void);
void EUSART2_write(uint8_t txData);
uint8_t EUSART2_read(void);
static void PPS_init(void);
static void PORT_init(void);
void putch(char txData);
char getch(void);

static void CLK_init(void)
{   
    /* Set HFINTOSC as new oscillator source. */
    OSCCON1 = _OSCCON1_NOSC1_MASK | _OSCCON1_NOSC2_MASK;

    /* Set HFFRQ to 1 MHz. */
    OSCFRQ = ~_OSCFREQ_HFFRQ_MASK;
}

static void PORT_init(void)
{
    /* Configure RD0 as output. */
    TRISD &= ~_TRISD_TRISD0_MASK;
    
    /* Configure RD1 as input. */
    TRISD |= _TRISD_TRISD1_MASK;
    
    /* Configure RE0 as output. */
    TRISE &= ~_TRISE_TRISE0_MASK;
    
    /* Enable RD1 digital input buffers.*/
    ANSELD &= ~_ANSELD_ANSELD1_MASK;
}

static void PPS_init(void) 
{
    RD0PPS = PPS_CONFIG_RD0_EUSART2_TX;    
    RX2PPS = PPS_CONFIG_RD1_EUSART2_RX; 
}

void EUSART2_init(void)
{
    /* 16-bit Baud Rate Generator is used */
    BAUD2CON = _BAUD2CON_BRG16_MASK;

    /* Serial Port Enable and Continuous Receive Enable */
    RC2STA = _RC2STA_SPEN_MASK | _RC2STA_CREN_MASK;
            
    /* Transmit Enable and High Baud Rate Select */
    TX2STA = _TX2STA_TXEN_MASK | _TX2STA_BRGH_MASK;

    /* Baud rate 9600 */
    SP2BRGL = BAUD_RATE_SYNC_0_BRG16_1_FOSC_1MHZ;
}

uint8_t EUSART2_read(void)
{
    while(!(PIR3 & _PIR3_RC2IF_MASK))
    {
        ;
    }

    return RC2REG;
}

void EUSART2_write(uint8_t txData)
{
    while(!(PIR3 & _PIR3_TX2IF_MASK))
    {
        ;
    }

    TX2REG = txData;
}

char getch(void)
{
    return EUSART2_read();
}

void putch(char txData)
{
    EUSART2_write(txData);
}

void executeCommand(char *command)
{
    if(strcmp(command, "ON") == 0)
    {
        LATE &= ~_LATE_LATE0_MASK;
        printf("OK, LED ON.\r\n");
    }
    else if (strcmp(command, "OFF") == 0)
    {
        LATE |= _LATE_LATE0_MASK;
        printf("OK, LED OFF.\r\n");
    } 
    else 
    {
        printf("Incorrect command.\r\n");
    }
}

void main(void)
{
    char command[MAX_COMMAND_LEN];
    uint8_t index = 0;
    char c;
    
    CLK_init();
    EUSART2_init();
    PPS_init();
    PORT_init();
    LATE &= ~_LATE_LATE0_MASK;
    
    while(1) 
    {
        c = getch();
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
