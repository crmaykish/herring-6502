EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "Herring SBC"
Date "2022-10-17"
Rev "1.0a"
Comp "Herring Computers"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Colin Maykish"
$EndDescr
$Comp
L herring:W65C02 U?
U 1 1 634E5D35
P 5750 3800
F 0 "U?" H 5400 5150 50  0000 C CNN
F 1 "W65C02" H 6000 2450 50  0000 C CNN
F 2 "" H 5750 3800 50  0001 C CNN
F 3 "" H 5750 3800 50  0001 C CNN
	1    5750 3800
	1    0    0    -1  
$EndComp
$Comp
L Oscillator:CXO_DIP14 X?
U 1 1 634EC20A
P 5300 1300
F 0 "X?" H 5100 1550 50  0000 L CNN
F 1 "CXO_DIP14" H 5350 1050 50  0000 L CNN
F 2 "Oscillator:Oscillator_DIP-14" H 5750 950 50  0001 C CNN
F 3 "http://cdn-reichelt.de/documents/datenblatt/B400/OSZI.pdf" H 5200 1300 50  0001 C CNN
	1    5300 1300
	1    0    0    -1  
$EndComp
NoConn ~ 5250 5000
$Comp
L power:VCC #PWR?
U 1 1 634F0394
P 5750 2300
F 0 "#PWR?" H 5750 2150 50  0001 C CNN
F 1 "VCC" H 5765 2473 50  0000 C CNN
F 2 "" H 5750 2300 50  0001 C CNN
F 3 "" H 5750 2300 50  0001 C CNN
	1    5750 2300
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 634F1077
P 5300 900
F 0 "#PWR?" H 5300 750 50  0001 C CNN
F 1 "VCC" H 5315 1073 50  0000 C CNN
F 2 "" H 5300 900 50  0001 C CNN
F 3 "" H 5300 900 50  0001 C CNN
	1    5300 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 634F25BA
P 5300 1700
F 0 "#PWR?" H 5300 1450 50  0001 C CNN
F 1 "GND" H 5305 1527 50  0000 C CNN
F 2 "" H 5300 1700 50  0001 C CNN
F 3 "" H 5300 1700 50  0001 C CNN
	1    5300 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 634F3310
P 5750 5300
F 0 "#PWR?" H 5750 5050 50  0001 C CNN
F 1 "GND" H 5600 5200 50  0000 C CNN
F 2 "" H 5750 5300 50  0001 C CNN
F 3 "" H 5750 5300 50  0001 C CNN
	1    5750 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1600 5300 1700
Wire Wire Line
	5300 900  5300 1000
Wire Wire Line
	5750 2300 5750 2400
Wire Wire Line
	5750 5200 5750 5300
NoConn ~ 5000 1300
Text GLabel 5600 1300 2    50   Input ~ 0
CLK_OSC
Text GLabel 5250 2600 0    50   Input ~ 0
CLK_OSC
$EndSCHEMATC
