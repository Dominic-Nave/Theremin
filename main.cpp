#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#define baseFreq 10000
#define prescale 1024
#define theta (F_CPU / prescale) / baseFreq
#define outBit DDD3
#define abs(n) n*(((n>0)<<1)-1)
#define quant(x) x^3/10
#define mode 2
#include <avr/io.h>
#include "FreqCounter/FreqCounter.h"
volatile int overflow = 0;

// this ISR is fired whenever a match occurs
// hence, toggle led here itself..
ISR (TIMER1_COMPA_vect)
{

    PORTD ^= (1 << PD3);

}

ISR (TIMER0_OVF_vect)
{
    overflow++;
}

// initialize timer, interrupt and variable
void setupTimers()
{
    cli();
/* Timer1 setup for waveform generation */
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  //OCR1A = 1000; //Compare value

  TCCR1B |= (1 << WGM12)| (1 << CS10); // Prescaler
  TIMSK1 |= (1 << OCIE1A); //Enable timer compare interrupt

/* Timer0 setup for frequency counting */
    TCCR0B |= (1 << CS02) | (1 << CS01); //Setup external clocking
    TIMSK1 |= (1 << TOIE0); //Enable overflow interrupt
    TCNT0 = 0; //Reset counter register

    // enable global interrupts
    sei();


}


#ifdef hello
int main()
{


    // connect led to pin PC0
    DDRD |= (1 << 3);
    uint16_t freq;
    // initialize timer
    setupTimers();

    // loop forever
    while(1)
    {
        TCNT0 = 0;
        _delay_ms(9);
        freq = (TCNT0 + (255 * overflow)) * 128;
        OCR1A = 12000;
        OCR1A = freq;

    }
}
#endif


int main()
{
    // connect led to pin PC0
    DDRD |= (1 << 3);
    uint16_t freq;
    // initialize timer
    setupTimers();

    // loop forever
    while(1)
    {
        TCNT0 = 0;
        _delay_ms(10);
        //freq = (TCNT0 + (255 * overflow)) * 257;
        OCR1A =  TCNT0 * 600;

    }
}


