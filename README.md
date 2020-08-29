#  CE3002-Plethysmograph-DAQ

## Arduino Uno ADC

 * The ADC in the ATMega328P is a 10-bit ADC. Oversampling is implemented to obtain a 12-bit ADC conversion result. To achieve n extra bits of resolution, 2^2^n samples is divided by 2^n. Hence 16 consecutive samples are obtained and divided by 4 to obtain a 12-bit ADC reading.

 * ADC noise reduction mode of the ATMega328P is also used to improve the noise environment for the ADC, enabling higher resolution measurements.