EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_UNO_R3 A?
U 1 1 5F7344F5
P 5300 2750
F 0 "A?" H 5300 3931 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 5300 3850 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 5300 2750 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 5300 2750 50  0001 C CNN
	1    5300 2750
	1    0    0    -1  
$EndComp
$Comp
L components:ESp8266 U?
U 1 1 5F7388BA
P 7250 2400
F 0 "U?" H 7275 2775 50  0000 C CNN
F 1 "ESp8266" H 7275 2684 50  0000 C CNN
F 2 "" H 7350 2300 50  0001 C CNN
F 3 "" H 7350 2300 50  0001 C CNN
	1    7250 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2250 6250 2250
Wire Wire Line
	6250 2250 6250 1500
Wire Wire Line
	6250 1500 5400 1500
Wire Wire Line
	5400 1500 5400 1750
Wire Wire Line
	7700 2550 7700 3000
Wire Wire Line
	7700 4000 6150 4000
Wire Wire Line
	5400 4000 5400 3850
Wire Wire Line
	6850 2450 6250 2450
Wire Wire Line
	6250 2450 6250 2250
Connection ~ 6250 2250
Text GLabel 6850 2550 0    50   Input ~ 0
D5
Text GLabel 4800 2650 0    50   Input ~ 0
D5
$Comp
L Switch:SW_Push SW?
U 1 1 5F73E769
P 6350 3250
F 0 "SW?" H 6350 3535 50  0000 C CNN
F 1 "SW_Push" H 6350 3444 50  0000 C CNN
F 2 "" H 6350 3450 50  0001 C CNN
F 3 "~" H 6350 3450 50  0001 C CNN
	1    6350 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3250 6150 4000
Connection ~ 6150 4000
Wire Wire Line
	6150 4000 5400 4000
Wire Wire Line
	6550 3250 6550 2350
Wire Wire Line
	6550 2350 6850 2350
$Comp
L Device:R R?
U 1 1 5F7441DD
P 8000 3000
F 0 "R?" V 7793 3000 50  0000 C CNN
F 1 "R" V 7884 3000 50  0000 C CNN
F 2 "" V 7930 3000 50  0001 C CNN
F 3 "~" H 8000 3000 50  0001 C CNN
	1    8000 3000
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5F744D35
P 8550 3000
F 0 "R?" V 8343 3000 50  0000 C CNN
F 1 "R" V 8434 3000 50  0000 C CNN
F 2 "" V 8480 3000 50  0001 C CNN
F 3 "~" H 8550 3000 50  0001 C CNN
	1    8550 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 3000 8250 3000
Wire Wire Line
	7850 3000 7700 3000
Connection ~ 7700 3000
Wire Wire Line
	7700 3000 7700 4000
Wire Wire Line
	7700 2250 8250 2250
Wire Wire Line
	8250 2250 8250 3000
Connection ~ 8250 3000
Wire Wire Line
	8250 3000 8400 3000
Text GLabel 8700 3000 2    50   Input ~ 0
D6
Text GLabel 4800 2750 0    50   Input ~ 0
D6
Text Label 7950 3050 0    50   ~ 0
2K
Text Label 8500 3050 0    50   ~ 0
1K
$EndSCHEMATC
