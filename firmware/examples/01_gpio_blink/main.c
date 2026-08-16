#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /* Configurar PB5 (Pin 13 en placa Arduino UNO, PB5 en ATmega328P) como salida */
    DDRB |= (1 << DDB5);

    while (1) {
        /* Alternar estado logico de PB5 */
        PORTB ^= (1 << PORTB5);
        _delay_ms(500);
    }

    return 0;
}
