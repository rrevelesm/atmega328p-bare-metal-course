#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdbool.h>

#define PWM_TOP 1999U

static volatile uint32_t system_ticks_ms = 0;

ISR(TIMER0_COMPA_vect) {
    system_ticks_ms++;
}

static uint32_t get_system_ticks_ms(void) {
    uint32_t ticks;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ticks = system_ticks_ms;
    }
    return ticks;
}

static void timer0_init_system_tick(void) {
    TCCR0A = (1 << WGM01);
    TCCR0B = 0;
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    TCCR0B |= (1 << CS01) | (1 << CS00);
}

static void timer1_init_pwm(void) {
    /* 1. Configure PB1 / OC1A (DIP-28 Pin 15) as digital output */
    DDRB |= (1 << DDB1);

    /* 2. Configure Timer1 for Fast PWM Mode 14 (TOP = ICR1, WGM13:10 = 1110) */
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    /* 3. Set TOP for 1.0 kHz PWM frequency @ F_CPU = 16 MHz, N = 8 */
    ICR1 = PWM_TOP;
    OCR1A = 0;

    /* 4. Start Timer1 clock with Prescaler = 8 */
    TCCR1B |= (1 << CS11);
}

static void adc_init(void) {
    /* 1. Select AVCC voltage reference with external cap at AREF pin, Channel ADC0 (PC0 / DIP-28 Pin 23) */
    ADMUX = (1 << REFS0);

    /* 2. Disable digital input buffer on ADC0 pin to reduce power & digital noise */
    DIDR0 |= (1 << ADC0D);

    /* 3. Enable ADC and set Prescaler = 128 (16 MHz / 128 = 125 kHz ADC clock) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

static uint16_t adc_read(void) {
    /* Start single conversion */
    ADCSRA |= (1 << ADSC);

    /* Poll ADSC until hardware clears it upon conversion completion */
    while (ADCSRA & (1 << ADSC)) {
        /* Wait for conversion completion */
    }

    /* Read 10-bit ADC result (AVR-libc handles ADCL/ADCH read order) */
    return ADC;
}

static void pwm_set_from_adc(uint16_t adc_val) {
    if (adc_val > 1023U) {
        adc_val = 1023U;
    }
    /* Linear scaling: 0 -> 0% duty (OCR1A=0), 1023 -> 100% duty (OCR1A=PWM_TOP) */
    OCR1A = ((uint32_t)adc_val * (uint32_t)PWM_TOP) / 1023U;
}

int main(void) {
    timer0_init_system_tick();
    timer1_init_pwm();
    adc_init();

    sei();

    uint32_t last_sample_ms = 0;

    while (1) {
        uint32_t current_ms = get_system_ticks_ms();

        /* Sample ADC and update PWM duty cycle every 50 ms */
        if ((current_ms - last_sample_ms) >= 50U) {
            last_sample_ms = current_ms;

            uint16_t raw_adc = adc_read();
            pwm_set_from_adc(raw_adc);
        }
    }

    return 0;
}
