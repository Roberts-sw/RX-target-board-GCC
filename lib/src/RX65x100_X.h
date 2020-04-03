#define CHIP( X )\
/*Pin Number 100-Pin LFQFP	I/O Port	init	MPC function	option board	A/D D/A*/\
/*X( 4 ,J3,IO_LO,  0,-						,)*/\
X( 11,37,IO_LO,  0,-						,)\
X( 13,36,IO_LO,  0,-						,)\
X( 15,35,IO_IN,  0,-						,)\
X( 16,34,IO_IN,  0,-						,)\
X( 17,33,IO_IN, 10,P33_GROVE_RXD6			,)\
X( 18,32,IO_HI, 10,P32_GROVE_TXD6			,)\
X( 19,31,IO_IN, 64,P31_IRQ1_SW				,)\
X( 20,30,IO_IN, 10,P30_PMOD_SSCL1_PIN10		,)\
X( 21,27,IO_LO,  0,-						,)\
X( 22,26,IO_IN, 10,P26_PMOD_SSDA1_PIN9		,)\
X( 23,25,IO_LO,  0,-						,ADTRG0n)\
X( 24,24,IO_LO,  0,-						,)\
X( 25,23,IO_IN, 11,P23_PMOD_SCI0_CTS_SS		,)\
/* ISL29035:0x44 BME680:0x76 BMI680:0x68 */\
X( 26,22,IO_HI, 10,P22_PMOD_SCI0_CLK		,)\
X( 27,21,IO_IN, 10,P21_PMOD_SCI0_RX_MISO	,)\
X( 28,20,IO_HI, 10,P20_PMOD_SCI0_TX_MOSI	,)\
X( 29,17,IO_LO,  0,-						,ADTRG1n)\
X( 30,16,IO_IN, 17,P16_USB_MICRO_VBUS		,ADTRG0n)\
X( 31,15,IO_IN, 64,P34_PMOD_IRQ4_GPIO_IRQ5	,)\
X( 32,14,IO_LO,  0,-						,)\
X( 33,13,IO_IN, 10,P13_I2C_SDA0				,ADTRG1n)\
X( 34,12,IO_IN, 10,P12_I2C_SCL0				,)\
X( 39,55,IO_LO,  0,-						,)\
X( 40,54,IO_LO,  0,-						,)\
X( 41,53,IO_LO,  0,-						,)\
X( 42,52,IO_LO,  0,-						,)\
X( 43,51,IO_LO,  0,-						,)\
X( 44,50,IO_LO,  0,OB_POWER					,)\
X( 45,C7,IO_HI,  0,-						,)\
X( 46,C6,IO_LO,  0,-						,)\
X( 47,C5,IO_LO,  0,-						,)\
X( 48,C4,IO_LO,  0,-						,)\
X( 49,C3,IO_LO,  0,-						,)\
X( 50,C2,IO_LO,  0,-						,)\
/*		*/\
X( 51,C1,IO_LO,  0,-						,)\
X( 52,C0,IO_LO, 11,PC0_RTS5_USBSERIAL		,)\
X( 53,B7,IO_IN, 10,PB7_GROVE_SSDA9			,)\
X( 54,B6,IO_IN, 10,PB6_GROVE_SSCL9			,)\
X( 55,B5,IO_LO,  0,-						,)\
X( 56,B4,IO_LO,  0,-						,)\
X( 57,B3,IO_LO,  0,-						,)\
X( 58,B2,IO_LO,  0,-						,)\
X( 59,B1,IO_HI,  3,PB1_LED2					,)\
X( 61,B0,IO_LO,  3,PB0_LED1					,)\
X( 63,A7,IO_IN,  0,-						,)\
X( 64,A6,IO_HI,  0,-						,)\
X( 65,A5,IO_HI,  3,PA5_CTS5_USBSERIAL		,)\
X( 66,A4,IO_HI, 10,PA4_TXD5_USBSERIAL		,)\
X( 67,A3,IO_IN, 10,PA3_RXD5_USBSERIAL		,)\
X( 68,A2,IO_LO,  0,-						,)\
X( 69,A1,IO_LO,  0,-						,)\
X( 70,A0,IO_LO,  0,-						,)\
X( 71,E7,IO_IN, 64,PE7_IRQ7_IC4_INT2		,AN105)\
X( 72,E6,IO_IN, 64,PE6_IRQ6_IC4_INT1		,AN104)\
X( 73,E5,IO_IN, 64,PE5_IRQ5_IC2_INT			,AN103)\
X( 74,E4,IO_LO,  0,-						,AN102)\
X( 75,E3,IO_LO,  0,-						,AN101)\
/*		*/\
X( 76,E2,IO_IN,  0,-						,AN100)\
X( 77,E1,IO_LO,  0,-						,ANEX1)\
X( 78,E0,IO_LO,  0,-						,ANEX0)\
X( 79,D7,IO_LO, 27,PD7_QIO01				,AN107)\
X( 80,D6,IO_LO, 27,PD6_QIO00				,AN106)\
X( 81,D5,IO_LO, 27,PD5_QSPCLK				,AN113)\
X( 82,D4,IO_LO, 27,PD4_QSSL					,AN112)\
X( 83,D3,IO_LO, 27,PD3_QIO03				,AN111)\
X( 84,D2,IO_LO, 27,PD2_QIO02				,AN110)\
X( 85,D1,IO_LO,  0,-						,AN109)\
X( 86,D0,IO_LO, 64,PD0_PMOD_IRQ0_GPIO		,AN108)\
X( 87,47,IO_LO,  0,-						,AN007)\
X( 88,46,IO_LO,  0,-						,AN006)\
X( 89,45,IO_LO,  0,-						,AN005)\
X( 90,44,IO_LO,  0,-						,AN004)\
X( 91,43,IO_LO,  0,-						,AN003)\
X( 92,42,IO_LO,  0,-						,AN002)\
X( 93,41,IO_IN,128,P41_GROVE_ADC1			,AN001)\
X( 95,40,IO_IN,128,P40_GROVE_ADC0			,AN000)\
X( 98,07,IO_LO,  0,-						,ADTRG0n)\
X(100,05,IO_LO,  0,-						,DA1)\
/*	79	*/
