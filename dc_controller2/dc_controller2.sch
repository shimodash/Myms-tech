EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp "Myms tech"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push_DPDT SW1
U 1 1 5ED327CB
P 3050 3650
F 0 "SW1" H 3050 4135 50  0000 C CNN
F 1 "SW_Push_DPDT" H 3050 4044 50  0000 C CNN
F 2 "miyama_dpdt:toggle_sw_THT" H 3050 3850 50  0001 C CNN
F 3 "~" H 3050 3850 50  0001 C CNN
	1    3050 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR0101
U 1 1 5ED330BA
P 3550 3000
F 0 "#PWR0101" H 3550 2850 50  0001 C CNN
F 1 "+3V0" H 3565 3173 50  0000 C CNN
F 2 "" H 3550 3000 50  0001 C CNN
F 3 "" H 3550 3000 50  0001 C CNN
	1    3550 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5ED334FE
P 3600 4250
F 0 "#PWR0102" H 3600 4000 50  0001 C CNN
F 1 "GND" H 3605 4077 50  0000 C CNN
F 2 "" H 3600 4250 50  0001 C CNN
F 3 "" H 3600 4250 50  0001 C CNN
	1    3600 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 3000 3550 3100
Wire Wire Line
	3550 3350 3250 3350
Wire Wire Line
	3550 3350 3550 3950
Wire Wire Line
	3550 3950 3250 3950
Connection ~ 3550 3350
Wire Wire Line
	3600 4250 3600 4200
Wire Wire Line
	3600 3750 3250 3750
Wire Wire Line
	3250 3550 3600 3550
Wire Wire Line
	3600 3550 3600 3750
Connection ~ 3600 3750
$Comp
L Motor:Motor_DC M1
U 1 1 5ED34125
P 2050 3600
F 0 "M1" H 2208 3596 50  0000 L CNN
F 1 "Motor_DC" H 2208 3505 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 2050 3510 50  0001 C CNN
F 3 "~" H 2050 3510 50  0001 C CNN
	1    2050 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3400 2550 3450
Wire Wire Line
	2550 3450 2850 3450
Wire Wire Line
	2050 3400 2550 3400
Wire Wire Line
	2050 3900 2550 3900
Wire Wire Line
	2550 3900 2550 3850
Wire Wire Line
	2550 3850 2850 3850
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5ED34FD1
P 3800 3000
F 0 "#FLG0101" H 3800 3075 50  0001 C CNN
F 1 "PWR_FLAG" H 3800 3173 50  0000 C CNN
F 2 "" H 3800 3000 50  0001 C CNN
F 3 "~" H 3800 3000 50  0001 C CNN
	1    3800 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 3000 3800 3100
Wire Wire Line
	3800 3100 3550 3100
Connection ~ 3550 3100
Wire Wire Line
	3550 3100 3550 3350
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5ED35BB0
P 3800 4100
F 0 "#FLG0102" H 3800 4175 50  0001 C CNN
F 1 "PWR_FLAG" H 3800 4273 50  0000 C CNN
F 2 "" H 3800 4100 50  0001 C CNN
F 3 "~" H 3800 4100 50  0001 C CNN
	1    3800 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4100 3800 4200
Wire Wire Line
	3800 4200 3600 4200
Connection ~ 3600 4200
Wire Wire Line
	3600 4200 3600 3750
$Comp
L Switch:SW_Push_DPDT SW2
U 1 1 5ED37C2A
P 5600 3600
F 0 "SW2" H 5600 4085 50  0000 C CNN
F 1 "SW_Push_DPDT" H 5600 3994 50  0000 C CNN
F 2 "miyama_dpdt:toggle_sw_THT" H 5600 3800 50  0001 C CNN
F 3 "~" H 5600 3800 50  0001 C CNN
	1    5600 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3300 5800 3300
Wire Wire Line
	6100 3300 6100 3900
Wire Wire Line
	6100 3900 5800 3900
Connection ~ 6100 3300
Wire Wire Line
	6150 3700 5800 3700
Wire Wire Line
	5800 3500 6150 3500
Wire Wire Line
	6150 3500 6150 3700
Connection ~ 6150 3700
$Comp
L Motor:Motor_DC M2
U 1 1 5ED37C46
P 4600 3550
F 0 "M2" H 4758 3546 50  0000 L CNN
F 1 "Motor_DC" H 4758 3455 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 4600 3460 50  0001 C CNN
F 3 "~" H 4600 3460 50  0001 C CNN
	1    4600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3350 5100 3400
Wire Wire Line
	5100 3400 5400 3400
Wire Wire Line
	4600 3350 5100 3350
Wire Wire Line
	4600 3850 5100 3850
Wire Wire Line
	5100 3850 5100 3800
Wire Wire Line
	5100 3800 5400 3800
Connection ~ 3800 3100
Wire Wire Line
	6100 3100 6100 3300
Wire Wire Line
	3800 4200 6150 4200
Connection ~ 3800 4200
Wire Wire Line
	6150 3700 6150 4200
$Comp
L Device:Battery BT1
U 1 1 5ED3A3F2
P 1200 3650
F 0 "BT1" H 1308 3696 50  0000 L CNN
F 1 "Battery" H 1308 3605 50  0000 L CNN
F 2 "footprint_mod:BatteryHolder_myfoot_2xAA" V 1200 3710 50  0001 C CNN
F 3 "~" V 1200 3710 50  0001 C CNN
	1    1200 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 3450 1200 3100
Wire Wire Line
	1200 3100 3550 3100
Wire Wire Line
	1200 3850 1200 4200
Wire Wire Line
	1200 4200 3600 4200
$Comp
L Graphic:Logo_Open_Hardware_Large #LOGO1
U 1 1 5ED3828A
P 6400 6950
F 0 "#LOGO1" H 6400 7450 50  0001 C CNN
F 1 "Logo_Open_Hardware_Large" H 6400 6550 50  0001 C CNN
F 2 "" H 6400 6950 50  0001 C CNN
F 3 "~" H 6400 6950 50  0001 C CNN
	1    6400 6950
	1    0    0    -1  
$EndComp
$Comp
L lib_microbit_connector:microbit_edge_connector J1
U 1 1 5ED3C6A2
P 4000 1600
F 0 "J1" V 3319 1454 50  0000 C CNN
F 1 "microbit_edge_connector" V 3410 1454 50  0000 C CNN
F 2 "Connectors_4UCON:4UCON_17909_02x401.27mm_Vertical" H 3900 1500 50  0001 C CNN
F 3 "https://www.microbit.co.uk/device/pins" H 3900 1500 50  0001 C CNN
	1    4000 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 2100 5500 2100
Wire Wire Line
	5500 2100 5600 2100
Connection ~ 5500 2100
Wire Wire Line
	5600 2100 5700 2100
Connection ~ 5600 2100
Wire Wire Line
	5700 2100 5800 2100
Connection ~ 5700 2100
Wire Wire Line
	5300 2100 5400 2100
Connection ~ 5400 2100
Wire Wire Line
	5000 2100 4900 2100
Wire Wire Line
	4900 2100 4800 2100
Connection ~ 4900 2100
Wire Wire Line
	4800 2100 4700 2100
Connection ~ 4800 2100
Connection ~ 4700 2100
Wire Wire Line
	2000 2100 2100 2100
Wire Wire Line
	2100 2100 2200 2100
Connection ~ 2100 2100
Wire Wire Line
	2300 2100 2200 2100
Connection ~ 2200 2100
Wire Wire Line
	2800 2100 2900 2100
Wire Wire Line
	2900 2100 3000 2100
Connection ~ 2900 2100
Wire Wire Line
	3000 2100 3100 2100
Connection ~ 3000 2100
Wire Wire Line
	3700 2100 3800 2100
Wire Wire Line
	3800 2100 3900 2100
Connection ~ 3800 2100
Wire Wire Line
	3900 2100 4000 2100
Connection ~ 3900 2100
Wire Wire Line
	4500 2100 4600 2100
Connection ~ 4600 2100
Wire Wire Line
	4600 2100 4700 2100
Connection ~ 4500 2100
Wire Wire Line
	5800 2100 6350 2100
Wire Wire Line
	6350 4200 6150 4200
Connection ~ 5800 2100
Connection ~ 6150 4200
$Comp
L Connector_Generic:Conn_02x04_Row_Letter_First J2
U 1 1 5ED5C96B
P 7050 2400
F 0 "J2" H 7100 2717 50  0000 C CNN
F 1 "Conn_02x04_Top_Bottom" H 7100 2626 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x04_P2.54mm_Vertical" H 7050 2400 50  0001 C CNN
F 3 "~" H 7050 2400 50  0001 C CNN
	1    7050 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2100 2300 2300
Wire Wire Line
	2300 2300 6850 2300
Connection ~ 2300 2100
Wire Wire Line
	3100 2100 3100 2400
Connection ~ 3100 2100
Wire Wire Line
	4000 2100 4000 2500
Wire Wire Line
	4000 2500 6850 2500
Connection ~ 4000 2100
Wire Wire Line
	3200 2100 3200 2600
Wire Wire Line
	3200 2600 6850 2600
Wire Wire Line
	6350 2100 6350 4200
Wire Wire Line
	3800 3100 4500 3100
Connection ~ 4500 3100
Wire Wire Line
	4500 3100 6100 3100
Wire Wire Line
	4500 2100 4500 3100
Wire Wire Line
	3100 2400 6850 2400
Wire Wire Line
	6100 3100 7350 3100
Connection ~ 6100 3100
Wire Wire Line
	7350 3100 7350 2600
Wire Wire Line
	7350 2300 7750 2300
Wire Wire Line
	7750 2300 7750 4200
Wire Wire Line
	7750 4200 6350 4200
Connection ~ 6350 4200
Wire Wire Line
	5100 2100 5100 2900
Wire Wire Line
	5100 2900 7450 2900
Wire Wire Line
	7450 2900 7450 2500
Wire Wire Line
	7450 2500 7350 2500
Wire Wire Line
	7350 2400 7550 2400
Wire Wire Line
	7550 2400 7550 3000
Wire Wire Line
	7550 3000 5200 3000
Wire Wire Line
	5200 3000 5200 2100
$EndSCHEMATC
