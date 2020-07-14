/*
main.c

MIT License

Copyright (c) 2020 Frank Manu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

void print_string(char * string);
void print_char(char c);
char read_char(void);

#include "inc/tm4c123gh6pm.h"
#define UART0_ENABLE (1<<0)
#define GPIO_PORTA_ENABLE (1<<0)
#define GPIO_PORTA_ENABLE_RX (1<<0)
#define GPIO_PORTA_ENABLE_TX (1<<1)
#define UART_ENABLE (1<<0)
#define UART_PARAMETERS (3<<5)      //8 BIT, NO PARITY, 1 STOP BIT
#define SYSTEM_CLOCK (0x0)

int main(void)
{
    char c;
//    1. Enable the UART module using the RCGCUART register
    SYSCTL_RCGCUART_R |= UART0_ENABLE;

//    2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register
    SYSCTL_RCGCGPIO_R |= GPIO_PORTA_ENABLE;

//    3. Set the GPIO AFSEL bits for the appropriate pins
    GPIO_PORTA_AFSEL_R |= GPIO_PORTA_ENABLE_RX;
    GPIO_PORTA_AFSEL_R |= GPIO_PORTA_ENABLE_TX;

//    4. Configure the GPIO current level and/or slew rate as specified for the mode selected

//    5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
//       pins
    GPIO_PORTA_PCTL_R |= GPIO_PORTA_ENABLE_RX;
    GPIO_PORTA_PCTL_R |= GPIO_PORTA_ENABLE_TX;

//    1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
    UART0_CTL_R &= ~UART_ENABLE;

//    2. Write the integer portion of the BRD to the UARTIBRD register.
    UART0_IBRD_R = 104;

//    3. Write the fractional portion of the BRD to the UARTFBRD register.
    UART0_FBRD_R = 11;

//    4. Write the desired serial parameters to the UARTLCRH register
    UART0_LCRH_R |= UART_PARAMETERS;

//    5. Configure the UART clock source by writing to the UARTCC register.
    UART0_CC_R = SYSTEM_CLOCK;

//    6. Enable the UART by setting the UARTEN bit in the UARTCTL register.
    UART0_CTL_R |= UART_ENABLE;

    while(1)
    {
        print_string("Enter rgb \n\r");

        c = read_char();
        print_char(c);
        print_string("\n\r");
        switch(c)
        {
            case 'r':
            break;
        }
    }
}

char read_char()
{
    char c;
    while((UART0_FR_R & (1<<4)) != 0);
    c = UART0_DR_R;
    return c;
}

void print_char(char c)
{
    while (UART0_FR_R & (1<<5) != 0);
    UART0_DR_R = c;
}

void print_string(char * string)
{
    while(*string)
    {
        print_char(*(string++));
    }
}
