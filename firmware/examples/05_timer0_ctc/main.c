#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Shared 32-bit millisecond system timebase.
 * Must be declared volatile because it is updated inside the Timer0 ISR.
 */
static volatile uint32_t system_ticks_ms = 0;

/*
 * Timer0 Output Compare Match A Interrupt Service Routine.
 * Executes periodically every 1 ms when F_CPU = 16 MHz, Prescaler = 64, OCR0A = 249.
 */
ISR(TIMER0_COMPA_vect) {
    system_ticks_ms++;
}

/*
 * Safely reads the 32-bit system tick counter using atomic protection.
 * On 8-bit AVR architecture, multi-byte reads can be interrupted mid-byte by ISR.
 */
static uint32_t get_system_ticks_ms(void) {
    uint32_t ticks;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ticks = system_ticks_ms;
    }
    return ticks;
}

int main(void) {
    /* 1. Configure PB5 (DIP-28 Pin 19) as digital LED output */
    DDRB |= (1 << DDB5);

    /* 2. Configure Timer0 in CTC Mode (WGM02:0 = 010) */
    TCCR0A = (1 << WGM01);
    TCCR0B = 0;

    /* 3. Set Compare Register A for 1 ms tick (250 counts: 0 to 249) */
    OCR0A = 249;

    /* 4. Enable Timer0 Output Compare Match A Interrupt */
    TIMSK0 = (1 << OCIE0A);

    /* 5. Set Prescaler = 64 (CS02:0 = 011) to start Timer0 clock */
    TCCR0B |= (1 << CS01) | (1 << CS00);

    /* 6. Enable global interrupts (sets I-bit in SREG) */
    sei();

    uint32_t last_toggle_ms = 0;

    /* 7. Non-blocking main super-loop */
    while (1) {
        uint32_t current_ms = get_system_ticks_ms();

        /* Wraparound-safe elapsed time calculation for 500 ms LED toggle */
        if ((current_ms - last_toggle_ms) >= 500U) {
            last_toggle_ms = current_ms;
            PORTB ^= (1 << PORTB5); /* Toggle LED output */
        }
    }

    return 0;
}
