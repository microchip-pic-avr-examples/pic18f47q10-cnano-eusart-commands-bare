<div id="readme" class="Box-body readme blob js-code-block-container">
<article class="markdown-body entry-content p-3 p-md-6" itemprop="text"><p><a href="https://www.microchip.com" rel="nofollow"><img src="images/MicrochipLogo.png" alt="MCHP" style="max-width:100%;"></a></p>

# PIC18F47Q10 EUSART Receive Control Commands

## Objective:

This example shows how to implement a command line interface. This way, the microcontroller can receive
control commands via the EUSART. In this use case an LED is controlled using commands sent from MPLAB® X
Data Visualizer.

## Resources:

- Technical Brief Link [(linkTBD)](http://www.microchip.com/)
- MPLAB® X IDE 5.30 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC8 2.10 or newer compiler [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
- PIC18F47Q10 Curiosity Nano [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM182029)
- [PIC18F47Q10 datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40002043D.pdf) for more information or specifications.

## Hardware Configuration:

The PIC18F47Q10 Curiosity Nano Development Board [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM182029) is used as the test platform. It has an onboard debugger that acts as an USART to USB convertor so no further hardware is needed. It also has the LED that is going to be used.

The following configurations must be made for this project:

- RD0 pin - Configured as digital output
- RD1 pin - Configured as digital input
- RE0 pin - Configured as digital output

## Demo:

Run the code, start MPLAB® X Data Visualizer selecting the COM port of the board, and send commands by writing them in the input box at the bottom.

<img src="images/demo.png" alt="Demo in MPLABX Data Visualizer"/>
