#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/*
 * Shared event flag communicated between ISR and main loop.
 * Must be declared volatile to prevent compiler register optimization/caching.
 */
static volatile bool button_event = false;

/*
 * Interrupt Service Routine for External Interrupt INT0 (PD2 / DIP-28 Pin 4).
 * Triggered asynchronously on falling edge (button press to GND).
 * Keeps execution minimal: sets flag and returns immediately.
 */
ISR(INT0_vect) {
    button_event = true;
}

int main(void) {
    /* 1. Configure PD2 / INT0 (DIP-28 Pin 4) as digital input */
    DDRD &= ~(1 << DDD2);

    /* 2. Enable internal pull-up resistor on PD2 */
    PORTD |= (1 << PORTD2);

    /* 3. Configure PB5 (DIP-28 Pin 19) as digital output */
    DDRB |= (1 << DDB5);

    /* 4. Configure INT0 trigger sense control: Falling edge (ISC01 = 1, ISC00 = 0) */
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    /* 5. Enable INT0 external interrupt request in mask register */
    EIMSK |= (1 << INT0);

    /* 6. Enable global interrupts (sets I-bit in Status Register SREG) */
    sei();

    /* 7. Application super-loop */
    while (1) {
        if (button_event) {
            button_event = false;    /* Clear event flag */
            PORTB ^= (1 << PORTB5); /* Toggle LED output */
        }
    }

    return 0;
}
