

/*------------------------------------------------
*   Authors = Dixit Gurung, Jacob VanVanderveen
*   EGR 326 LAB09
*   PART I
*---------------------------------------------------*/

/*
#include "msp.h"
#include <ST7735.h>
#include <stdio.h>
#include <stdint.h>


void PORT1_init(void);
void Clock_Init48MHz(void);
int rotation = 0, count = 0;
int part1_flag = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer



     PORT1_init();
    Clock_Init48MHz();

    //Initialize Pins
    P2->SEL0    =    0x00;    //make P2 GPIO
    P2->SEL1    =    0x00;

    P2->DIR        |=BIT0;    //P2.1is output
    P2->OUT      &= ~BIT0;    //Start with P2.1 off

    P2->DIR        |=BIT1;    //P2.1is output
    P2->OUT      &= ~BIT1;    //Start with P2.1 off

    P2->DIR        |=BIT2;    //P2.1is output
    P2->OUT      &= ~BIT2;    //Start with P2.1 off

    NVIC_EnableIRQ(PORT1_IRQn);
    __enable_irq();

    //Black background color black for LCD
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
       while(1)
        {

    //Since the interrupt edge is togelled in the port handler when the count is odd the hall effect transistor is closed
    //and when even the transistor is open
           if(rotation %2 == 0){
               ST7735_DrawString(8, 8, "OPEN", ST7735_YELLOW);
           }
           else
               ST7735_DrawString(8, 8, "CLOSED", ST7735_YELLOW);

}
}


void PORT1_init(void){
    P1->SEL0 &= ~ BIT5;
    P1->SEL1 &= ~ BIT5;
    P1->DIR &= ~ BIT5;
    P1->REN |= BIT5;
    P1->OUT |= BIT5;
    P1->IES |=BIT5;//falling edge interrupt
    P1->IE  |= BIT5;//enable interrupt
    P1->IFG = 0;//clear IFG flag
}


void PORT1_IRQHandler()
{


    if (P1->IFG & BIT5)
    {

        rotation++;
        P2->OUT ^= BIT0;
        P1->IES &=~BIT5;//Rising edge interrupt
    }

    P1->IFG = 0x00;                     //Clear interrupt flags
}


void SysTick_Init_interrupt(void)
{            //initialization of systic timer
    SysTick->CTRL = 0;                         // disable SysTick During step
    SysTick->LOAD = 3000;          //  reload value for 1ms interrupts
    SysTick->VAL = 0;                         // any write to current clears it
    SysTick->CTRL = 0x00000007;            // enable systic, 3MHz,   Interrupts
}

void SysTick_Handler(void)
{

    count++;

}



void Clock_Init48MHz(void)
{
    // Configure Flash wait-state to 1 for both banks 0 & 1
       FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK0_RDCTL_WAIT_1;
       FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK1_RDCTL_WAIT_1;

    //Configure HFXT to use 48MHz crystal, source to MCLK & HSMCLK*
       PJ->SEL0 |= BIT2 | BIT3;                     // Configure PJ.2/3 for HFXT function
       PJ->SEL1 &= ~(BIT2 | BIT3);
       CS->KEY = CS_KEY_VAL ;                       // Unlock CS module for register access
       CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
          while(CS->IFG & CS_IFG_HFXTIFG)
                    CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

     Select MCLK & HSMCLK = HFXT, no divider
      CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK     |
                              CS_CTL1_DIVM_MASK     |
                              CS_CTL1_SELS_MASK     |
                              CS_CTL1_DIVHS_MASK)   |
                              CS_CTL1_SELM__HFXTCLK |
                              CS_CTL1_SELS__HFXTCLK;

      CS->CTL1 = CS->CTL1 |CS_CTL1_DIVS_2;    // change the SMCLK clock speed to 12 MHz.

      CS->KEY = 0;                            // Lock CS module from unintended accesses
}



*/






/*------------------------------------------------
*   Authors = Dixit Gurung, Jacob VanVanderveen
*   EGR 326 LAB09
*   PART II
*---------------------------------------------------*/

#include "msp.h"
#include <ST7735.h>
#include <stdio.h>
#include <stdint.h>


void PORT1_init(void);
void Clock_Init48MHz(void);
void SysTick_Init_interrupt(void);
void int_to_string(int value);


char string[3]= {'0','0','0'};
int rotation = 0, count = 0, rate_prev = 0, rate_new = 0;
int part1_flag = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer



     PORT1_init();
    Clock_Init48MHz();

    //Initialize Pins
    P2->SEL0    =    0x00;    //make P2 GPIO
    P2->SEL1    =    0x00;

    P2->DIR        |=BIT0;    //P2.1is output
    P2->OUT      &= ~BIT0;    //Start with P2.1 off

    P2->DIR        |=BIT1;    //P2.1is output
    P2->OUT      &= ~BIT1;    //Start with P2.1 off

    P2->DIR        |=BIT2;    //P2.1is output
    P2->OUT      &= ~BIT2;    //Start with P2.1 off

    SysTick_Init_interrupt();
    NVIC_EnableIRQ(PORT1_IRQn);
    __enable_irq();

    //Black background color black for LCD
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    count = 0;

       while(1)
        {

           //count = 4 for 1 seconds
         if (count == 4)
        {
            rate_new = 60*rotation; //saving the rotation in rate variable and times 60 to change from RPS to RPM
            rotation =0; //Clear rotation variable so that it can start recounting in port handler
            int_to_string(rate_new); //This function breaks the integer and saves the broken value into the string
            ST7735_DrawString(5, 8, string, ST7735_YELLOW);
            ST7735_DrawString(8, 8, "RPM", ST7735_YELLOW);
            count = 0;//clearing count
            P2->OUT ^= BIT1;//for testing purposes
        }

}
}


void PORT1_init(void){
    P1->SEL0 &= ~ BIT5;
    P1->SEL1 &= ~ BIT5;
    P1->DIR &= ~ BIT5;
    P1->REN |= BIT5;
    P1->OUT |= BIT5;
    P1->IES |=BIT5;//falling edge interrupt
    P1->IE  |= BIT5;//enable interrupt
    P1->IFG = 0;//clear IFG flag
}


void PORT1_IRQHandler()
{


    if (P1->IFG & BIT5)
    {
        rotation++;
        P2->OUT ^= BIT0;
    }

    P1->IFG = 0x00;                     //Clear interrupt flags
}


void SysTick_Init_interrupt(void)
{            //initialization of systic timer
    SysTick->CTRL = 0;                         // disable SysTick During step
    SysTick->LOAD = 16000000;          //  reload value for 1ms interrupts
    SysTick->VAL = 0;                         // any write to current clears it
    SysTick->CTRL = 0x00000007;            // enable systic, 3MHz,   Interrupts
}

void SysTick_Handler(void)
{

    count++;

}

//Breakdown the int value and then store into a string array
void int_to_string(int value){

    int i = 2;


    while (value > 0) {
     int digit = value % 10;
     // do something with digit
      string[i]= digit + 48;
      i--;
     value /= 10;
    }
}


void Clock_Init48MHz(void)
{
    // Configure Flash wait-state to 1 for both banks 0 & 1
       FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK0_RDCTL_WAIT_1;
       FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK1_RDCTL_WAIT_1;

    //Configure HFXT to use 48MHz crystal, source to MCLK & HSMCLK*
       PJ->SEL0 |= BIT2 | BIT3;                     // Configure PJ.2/3 for HFXT function
       PJ->SEL1 &= ~(BIT2 | BIT3);
       CS->KEY = CS_KEY_VAL ;                       // Unlock CS module for register access
       CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
          while(CS->IFG & CS_IFG_HFXTIFG)
                    CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

    /* Select MCLK & HSMCLK = HFXT, no divider */
      CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK     |
                              CS_CTL1_DIVM_MASK     |
                              CS_CTL1_SELS_MASK     |
                              CS_CTL1_DIVHS_MASK)   |
                              CS_CTL1_SELM__HFXTCLK |
                              CS_CTL1_SELS__HFXTCLK;

      //CS->CTL1 = CS->CTL1 |CS_CTL1_DIVS_2;    // change the SMCLK clock speed to 12 MHz.

      CS->KEY = 0;                            // Lock CS module from unintended accesses
}
