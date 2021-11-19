/* 
 * File:   main.c
 * Author: iago rubio
 *
 * Created on November 18, 2021, 8:48 PM
 * 
 * Interrupt click switch and turn on led on
 * Curiosity Nano 4809
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>            
#include <avr/interrupt.h>

volatile uint8_t switchOn;

// PORTF interrupt callback
ISR(PORTF_PORT_vect)
{
    if(PORTF.INTFLAGS & PIN6_bm)
    {
        switchOn = 1;
        PORTF.INTFLAGS &= PIN6_bm; // clear interrupt
    }
}


/* PORTF Pin5 is connected to led*/
void init_led(void)
{
    PORTF.DIR |= PIN5_bm; // set PIN5 as output (using bitmask)
    PORTF.OUT |= PIN5_bm; // set PIN5 low
}

/* 
 * PORTF PIN6 is connected to mechanical switch
 * that's pulled up by a hardware resistor 
 */
void init_button(void)
{
    PORTF_DIR &= ~PIN6_bm; // set PIN6 as input (using bit mask))
    PORTF_PIN6CTRL |=   PORT_ISC_BOTHEDGES_gc; // set interrupt both edges
}


int main(void) {
    init_led();
    init_button();
    
    sei(); // enable interrupt
    
    while (1)
    {        
        if(switchOn)
        {
            if( !(~PORTF.IN & PIN6_bm) )    // if switch is pulled to ground
            {
                PORTF.OUT |= PIN5_bm;       // turn on led PIN (set 6th bit on PORTF output register)
            }                               // PIN5_bm defined as 0x20 in iom4809.h b00100000
            else                            // if is not pulled to ground
            {
                PORTF.OUT &= ~PIN5_bm;      // turn off led PIN (clear 6th bit on PORTF output register)
            }                               // PIN5_bm defined as 0x20 in iom4809.h b00100000
            
            switchOn = 0;
        }
    }
}