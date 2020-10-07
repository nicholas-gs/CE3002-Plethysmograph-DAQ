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

#include "AuthKeys.h"
#include "Definitions.h"
#include "SoftwareSerial.h"
#include <Arduino.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <ESP8266_Lib.h>

#if ADC_MODE == ADC_AVERAGE || ADC_MODE == ADC_EWMA
#include <avr/sleep.h>
#endif

#if ADC_MODE == ADC_AVERAGE || ADC_MODE == ADC_EWMA
// Forward declarations
void init_adc_pins();
void init_adc();
void init_adc_sleepmode();
uint16_t oversample_16x();
double convert_adc_val(uint16_t adc_val);
#endif

// Interrupts
EMPTY_INTERRUPT(ADC_vect);

// Object Initialization
#if WIRELESS_MODE == WIFI_PRESENT
SoftwareSerial Esp8266Serial(ESP_SERIAL_RECEIVE_PIN, ESP_SERIAL_TRANSMIT_PIN);
ESP8266 esp8266(&Esp8266Serial);
#endif

void setup() {
    // put your setup code here, to run once:

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.begin(SERIAL_BAUD_RATE);
#if WIRELESS_MODE == WIFI_PRESENT
    Esp8266Serial.begin(ESP8266_BAUD_RATE);
#endif
#if DEBUG_MODE == DEBUG_ON
    Serial.println(F("Serial setup completed"));
#endif

#if ADC_MODE == ADC_AVERAGE
    init_adc_pins();
    init_adc();
    init_adc_sleepmode();
    // Enable global interrupts
    sei();
#elif ADC_MODE == ADC_SIMPLE
    pinMode(A0, INPUT);
#endif
    // Serial.println(esp8266.getStationIp());
#if DEBUG_MODE == DEBUG_ON
    Serial.println(F("Setting up Blynk"));
#endif
#if WIRELESS_MODE == WIFI_PRESENT
    Blynk.begin(auth, esp8266, ssid, pass);
#endif
#if DEBUG_MODE == DEBUG_ON
    Serial.println(F("Blynk started!"));
#endif

#if DEBUG_MODE == DEBUG_ON
    Serial.println(F("End of setup"));
#endif

    delay(50);
}

void loop() {
    // put your main code here, to run repeatedly:
#if WIRELESS_MODE == WIFI_PRESENT
    Blynk.run();
#endif

    uint16_t adc_reading;
#if ADC_MODE == ADC_AVERAGE
    adc_reading = oversample_16x() / 16;
#elif ADC_MODE == ADC_EWMA
    adc_reading = emwa();
#elif ADC_MODE == ADC_SIMPLE
    adc_reading = analogRead(A0);
#endif
    // Send voltage reading to matlab through serial port
    Serial.println(adc_reading);
    delay(10);
}

#if ADC_MODE == ADC_AVERAGE || ADC_MODE == ADC_EWMA

/// Initialise pin A0
void init_adc_pins() {
    // Set PC1 (A1) as input pin
    DDRC &= ~(1 << DDC1);
    // Disable PC1 (A1) pull-up resistor
    PORTC &= ~(1 << PORTC1);
}

/// Initialise ADC
void init_adc() {
    // Set AVcc as ADC reference voltage
    ADMUX &= ~(1 << REFS1);
    ADMUX |= (1 << REFS0);
    // Right shift ADC conversion result
    ADMUX &= ~(1 << ADLAR);

    // Choose ADC1 (A1) channel as ADC input
    ADMUX &= ~((1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    ADMUX |= (1 << MUX0);

    // Disable digital input buffer for ADC0
    DIDR0 |= (1 << ADC1D);

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

#if WIRELESS_MODE == WIFI_PRESENT
// Blynk controlling alert on Arduino
// It turns on the LED and the piezoelectric speaker
BLYNK_WRITE(V0) {
    int pinValue = param.asInt();
    digitalWrite(LED_PIN, pinValue);
    digitalWrite(BUZZER_PIN, pinValue);
}
#endif
