#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

/* Debounce stability threshold in milliseconds */
#define DEBOUNCE_MS 20

/* Button Finite State Machine States */
typedef enum {
    STATE_RELEASED,
    STATE_POSSIBLE_PRESS,
    STATE_PRESSED,
    STATE_POSSIBLE_RELEASE
} button_state_t;

/* 
 * Non-blocking button state machine tick processor.
 * Polled every 1ms. Returns true ONCE when a stable press event is confirmed.
 */
static bool process_button_debounce(void) {
    static button_state_t current_state = STATE_RELEASED;
    static uint8_t timer_ms = 0;
    bool event_trigger = false;

    /* Read raw pin state: Active-Low logic (0 = pressed to GND, 1 = released/pull-up) */
    bool raw_pressed = !(PIND & (1 << PIND2));

    switch (current_state) {
        case STATE_RELEASED:
            if (raw_pressed) {
                current_state = STATE_POSSIBLE_PRESS;
                timer_ms = 0;
            }
            break;

        case STATE_POSSIBLE_PRESS:
            if (raw_pressed) {
                timer_ms++;
                if (timer_ms >= DEBOUNCE_MS) {
                    current_state = STATE_PRESSED;
                    event_trigger = true; /* Confirmed stable press event */
                }
            } else {
                current_state = STATE_RELEASED; /* Spurious glitch ignored */
            }
            break;

        case STATE_PRESSED:
            if (!raw_pressed) {
                current_state = STATE_POSSIBLE_RELEASE;
                timer_ms = 0;
            }
            break;

        case STATE_POSSIBLE_RELEASE:
            if (!raw_pressed) {
                timer_ms++;
                if (timer_ms >= DEBOUNCE_MS) {
                    current_state = STATE_RELEASED; /* Confirmed stable release */
                }
            } else {
                current_state = STATE_PRESSED; /* Spurious bounce ignored */
            }
            break;
    }

    return event_trigger;
}

int main(void) {
    /* 1. Configure PD2 (DIP-28 Pin 4) as digital input */
    DDRD &= ~(1 << DDD2);

    /* 2. Enable internal pull-up resistor on PD2 */
    PORTD |= (1 << PORTD2);

    /* 3. Configure PB5 (DIP-28 Pin 19) as digital output */
    DDRB |= (1 << DDB5);

    while (1) {
        /*
         * Process non-blocking debounce state machine tick.
         * Note: _delay_ms(1) provides a simple 1ms timebase tick.
         * Hardware Timers (introduced in PR-05) will eliminate software polling delays.
         */
        if (process_button_debounce()) {
            /* Toggle LED output on confirmed button press event */
            PORTB ^= (1 << PORTB5);
        }

        _delay_ms(1);
    }

    return 0;
}
