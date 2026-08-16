#include <avr/io.h>

int main(void) {
    /* 1. Configure PD2 (DIP-28 Pin 4) as digital input */
    DDRD &= ~(1 << DDD2);

    /* 2. Enable internal pull-up resistor on PD2 */
    PORTD |= (1 << PORTD2);

    /* 3. Configure PB5 (DIP-28 Pin 19) as digital output */
    DDRB |= (1 << DDB5);

    while (1) {
        /*
         * Active-Low Logic Reading:
         * When the tactile push button connected between PD2 and GND is pressed,
         * PD2 is pulled to 0V (LOW). Bitwise expression (PIND & (1 << PIND2)) evaluates to 0.
         * When released, internal pull-up pulls PD2 to 5V (HIGH). Expression evaluates to non-zero.
         */
        if (!(PIND & (1 << PIND2))) {
            /* Button Pressed (LOW) -> Turn LED ON (PORTB5 HIGH) */
            PORTB |= (1 << PORTB5);
        } else {
            /* Button Released (HIGH) -> Turn LED OFF (PORTB5 LOW) */
            PORTB &= ~(1 << PORTB5);
        }
    }

    return 0;
}
