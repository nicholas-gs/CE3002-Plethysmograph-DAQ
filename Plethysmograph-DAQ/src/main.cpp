/**
 * Platform: ATMega328P (Arduino Uno)
 * 
 * Get ADC reading from ADC0 channel (Pin A0) of Arduino Uno.
 * 
 * The ADC in the ATMega328P is a 10-bit ADC. Oversampling is implemented
 * to obtain a 12-bit ADC conversion result. To achieve n extra bits of resolution, 2^2^n samples is divided by 2^n.
 * 
 * ADC noise reduction mode is also used to improve the noise environment for the ADC, enabling higher resolution measurements.
**/

#include <Arduino.h>

// Definitions
#define ADC_SIMPLE     0
#define ADC_OVERSAMPLE 1
#define DEBUG_ON       1
#define DEBUG_OFF      0

#define SERIAL_BAUD_RATE 9600
#define ADC_REF          5
#define ADC_STEPS        4096

// Configurations
#define ADC_MODE   ADC_OVERSAMPLE
#define DEBUG_MODE DEBUG_ON

#if ADC_MODE == ADC_OVERSAMPLE
#include <avr/sleep.h>
#endif

#if ADC_MODE == ADC_OVERSAMPLE
// Forward declarations
void init_adc_pins();
void init_adc();
void init_adc_sleepmode();
uint16_t oversample_16x();
double convert_adc_val(uint16_t adc_val);
#endif

// Interrupts
EMPTY_INTERRUPT(ADC_vect);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(SERIAL_BAUD_RATE);

#if ADC_MODE == ADC_OVERSAMPLE
    init_adc_pins();
    init_adc();
    init_adc_sleepmode();
    // Enable global interrupts
    sei();
#elif ADC_MODE == ADC_SIMPLE
    pinMode(A0, INPUT);
#endif

#if DEBUG_MODE == DEBUG_ON
    Serial.println("End of setup");
#endif

    delay(50);
}

void loop() {
    // put your main code here, to run repeatedly:
    double voltage_reading;
#if ADC_MODE == ADC_OVERSAMPLE
    voltage_reading = convert_adc_val(oversample_16x());
#elif ADC_MODE == ADC_SIMPLE
    uint16_t adc_reading = analogRead(A0);
    voltage_reading = ((double)adc_reading / (double)1024) * ((double)ADC_REF);
#endif
    Serial.print(voltage_reading);
    Serial.println(" V");
    delay(200);
}

#if ADC_MODE == ADC_OVERSAMPLE

/// Initialise pin A0
void init_adc_pins() {
    // Set PC0 (A0) as input pin
    DDRC &= ~(1 << DDC0);
    // Disable PC0 (A0) pull-up resistor
    PORTC &= ~(1 << PORTC0);
}

/// Initialise ADC
void init_adc() {
    // Set AVcc as ADC reference voltage
    ADMUX &= ~(1 << REFS1);
    ADMUX |= (1 << REFS0);
    // Right shift ADC conversion result
    ADMUX &= ~(1 << ADLAR);
    // Choose ADC0 (A0) channel as ADC input
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));

    // Disable digital input buffer for ADC0
    DIDR0 |= (1 << ADC0D);

    // Enable ADC
    ADCSRA |= (1 << ADEN);
    // Disable ADC auto-trigger
    ADCSRA &= ~(1 << ADATE);
    // Enable ADC conversion complete interrupt.
    ADCSRA |= (1 << ADIE);
    // Set ADC clock prescalar to 32
    ADCSRA |= ((1 << ADPS2) | (1 << ADPS0));
    ADCSRA &= ~(1 << ADPS1);
}

/// Initialise ADC Noise Reduction Mode
void init_adc_sleepmode() {
    SMCR |= (1 << SM0);
    SMCR &= ~((1 << SM1) | (1 << SM2));
}

/// Convert ADC reading into voltage reading.
double convert_adc_val(uint16_t adc_val) {
    uint16_t adc_12_bit_val = adc_val >> 2;
    return (((double)(adc_12_bit_val) / (double)(ADC_STEPS))) * (double)(ADC_REF);
}

/// Take 16 consecutive readings from the ADC
uint16_t oversample_16x() {
    uint16_t oversampledValue = 0;
    for (uint8_t i = 0; i < 16; i++) {
        sleep_mode();
        oversampledValue += ADC;
    }
    return oversampledValue;
}

#endif
