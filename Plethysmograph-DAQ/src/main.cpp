/**
 * Platform: ATMega328P (Arduino Uno)
 * 
 * Get ADC reading from ADC1 channel (Pin A1) of Arduino Uno.
 * 
 * The ADC in the ATMega328P is a 10-bit ADC. Multiple consecutive ADC readings are taken, and the average is used so
 * smooth out noise
 * 
 * ADC noise reduction mode is also used to improve the noise environment for the ADC, enabling higher precision measurements.
**/

#include "AuthKeys.h"
#include "Definitions.h"
#include "SoftwareSerial.h"
#include <Arduino.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <ESP8266_Lib.h>

#if ADC_MODE == ADC_AVERAGE
#include <avr/sleep.h>
#endif

#if ADC_MODE == ADC_AVERAGE
// Forward declarations
void init_adc_pins();
void init_adc();
void init_adc_sleepmode();
uint16_t oversample_4x();
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
    pinMode(A1, INPUT);
#endif

#if WIRELESS_MODE == WIFI_PRESENT
    #if DEBUG_MODE == DEBUG_ON
        Serial.println(F("Setting up Blynk"));
    #endif
    Blynk.begin(auth, esp8266, ssid, pass);
    #if DEBUG_MODE == DEBUG_ON
        Serial.println(F("Blynk started!"));
    #endif
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
    adc_reading = oversample_4x();
#elif ADC_MODE == ADC_SIMPLE
    adc_reading = analogRead(A1);
#endif
    // Send voltage reading to matlab through serial port
    Serial.println(adc_reading);
    delay(50);
}

#if ADC_MODE == ADC_AVERAGE

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

/// Convert ADC reading into voltage reading. Currently not used
double convert_adc_val(uint16_t adc_val) {
    uint16_t adc_12_bit_val = adc_val >> 2;
    return (((double)(adc_12_bit_val) / (double)(ADC_STEPS))) * (double)(ADC_REF);
}

/// Take 16 consecutive readings from the ADC and returns the average
uint16_t oversample_4x() {
    uint16_t oversampledValue = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sleep_mode();
        oversampledValue += ADC;
    }
    return oversampledValue / 4;
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
