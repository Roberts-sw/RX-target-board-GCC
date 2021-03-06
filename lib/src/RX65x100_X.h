#define CHIP(X)\
/*	X(  0,W7,IO_IN,NO    ,0  ,-         ,-                      )*/\
	/*Pin,IO,init ,MSTP  ,MPC,target    ,option*/\
	X(  4,J3,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 11,37,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 13,36,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 15,35,IO_IN,NO    ,0  ,-         ,-                      )\
	X( 16,34,IO_IN,NO    ,0  ,-         ,-                      )\
	X( 17,33,IO_IN,SCI6  ,10 ,-         ,P33_GROVE_RXD6         )\
	X( 18,32,IO_HI,SCI6  ,10 ,PMOD1-RST ,P32_GROVE_TXD6         )\
	X( 19,31,IO_IN,NO    ,64 ,-         ,P31_IRQ1_SW            )\
	X( 20,30,IO_IN,SCI1  ,10 ,-         ,P30_PMOD_SSCL1_PIN10   )\
	X( 21,27,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 22,26,IO_IN,SCI1  ,10 ,-         ,P26_PMOD_SSDA1_PIN9    )\
	X( 23,25,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 24,24,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 25,23,IO_IN,SCI0  ,11 ,-         ,P23_PMOD_SCI0_CTS_SS   )\
	/* ISL29035:0x44 BME680:0x76 BMI680:0x68,,*/\
	X( 26,22,IO_HI,SCI0  ,10 ,-         ,P22_PMOD_SCI0_CLK      )\
	X( 27,21,IO_IN,SCI0  ,10 ,-         ,P21_PMOD_SCI0_RX_MISO  )\
	X( 28,20,IO_HI,SCI0  ,10 ,-         ,P20_PMOD_SCI0_TX_MOSI  )\
	X( 29,17,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 30,16,IO_IN,USB0  ,17 ,-         ,P16_USB_MICRO_VBUS     )\
	X( 31,15,IO_IN,NO    ,64 ,PMOD1_IRQ ,P34_PMOD_IRQ4_GPIO_IRQ5)\
	X( 32,14,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 33,13,IO_IN,RIIC0 ,15 ,-         ,P13_I2C_SDA0           )\
	X( 34,12,IO_IN,RIIC0 ,15 ,-         ,P12_I2C_SCL0           )\
	X( 39,55,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 40,54,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 41,53,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 42,52,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 43,51,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 44,50,IO_LO,NO    ,0  ,-         ,OB_POWER               )\
	X( 45,C7,IO_HI,NO    ,0  ,-         ,-                      )\
	X( 46,C6,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 47,C5,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 48,C4,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 49,C3,IO_LO,NO    ,0  ,PMOD1-IO0 ,-                      )\
	X( 50,C2,IO_LO,NO    ,0  ,PMOD1-IO1 ,-                      )\
	/*   ,  ,*/\
	X( 51,C1,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 52,C0,IO_LO,SCI5  ,11 ,-         ,PC0_RTS5_USBSERIAL     )\
	X( 53,B7,IO_IN,SCI9  ,10 ,-         ,PB7_GROVE_SSDA9        )\
	X( 54,B6,IO_IN,SCI9  ,10 ,-         ,PB6_GROVE_SSCL9        )\
	X( 55,B5,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 56,B4,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 57,B3,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 58,B2,IO_LO,TPU   ,3  ,-         ,PB2_LED2               )\
	X( 59,B1,IO_HI,NO    ,0  ,TB_nSW1   ,-                      )\
	X( 61,B0,IO_LO,TPU   ,3  ,-         ,PB0_LED1               )\
	X( 63,A7,IO_IN,NO    ,0  ,PMOD1-MISO,-                      )\
	X( 64,A6,IO_HI,NO    ,0  ,PMOD1-MOSI,-                      )\
	X( 65,A5,IO_HI,NO    ,3  ,PMOD1-SCK ,PA5_CTS5_USBSERIAL     )\
	X( 66,A4,IO_HI,SCI5  ,10 ,PMOD1-CS  ,PA4_TXD5_USBSERIAL     )\
	X( 67,A3,IO_IN,SCI5  ,10 ,-         ,PA3_RXD5_USBSERIAL     )\
	X( 68,A2,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 69,A1,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 70,A0,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 71,E7,IO_IN,NO    ,64 ,-         ,PE7_IRQ7_IC4_INT2      )\
	X( 72,E6,IO_IN,NO    ,64 ,-         ,PE6_IRQ6_IC4_INT1      )\
	X( 73,E5,IO_IN,NO    ,64 ,-         ,PE5_IRQ5_IC2_INT       )\
	X( 74,E4,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 75,E3,IO_LO,NO    ,0  ,-         ,-                      )\
	/*,,*/\
	X( 76,E2,IO_IN,NO    ,0  ,-         ,-                      )\
	X( 77,E1,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 78,E0,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 79,D7,IO_LO,QSPI  ,27 ,TB_nLED1  ,PD7_QIO01              )\
	X( 80,D6,IO_LO,QSPI  ,27 ,TB_nLED0  ,PD6_QIO00              )\
	X( 81,D5,IO_LO,QSPI  ,27 ,-         ,PD5_QSPCLK             )\
	X( 82,D4,IO_LO,QSPI  ,27 ,-         ,PD4_QSSL               )\
	X( 83,D3,IO_LO,QSPI  ,27 ,-         ,PD3_QIO03              )\
	X( 84,D2,IO_LO,QSPI  ,27 ,-         ,PD2_QIO02              )\
	X( 85,D1,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 86,D0,IO_LO,NO    ,64 ,-         ,PD0_PMOD_IRQ0_GPIO     )\
	X( 87,47,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 88,46,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 89,45,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 90,44,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 91,43,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 92,42,IO_LO,NO    ,0  ,-         ,-                      )\
	X( 93,41,IO_IN,S12AD0,128,-         ,P41_GROVE_ADC1         )\
	X( 95,40,IO_IN,S12AD0,128,-         ,P40_GROVE_ADC0         )\
	X( 98,07,IO_LO,NO    ,0  ,-         ,-                      )\
	X(100,05,IO_LO,NO    ,0  ,-         ,-                      )\
	/*   ,79,*/

#define iopin_alldirs(\
	out0,out1,out2,out3,out4,out5,out6,out7,\
	out8,out9,outa,outb,outc,outd,oute,outf,\
	outg,outj)\
do{	IO_._PDR[CH2P_('0')]=0b01011111|(~0b00000000 & (out0) );\
	IO_._PDR[CH2P_('1')]=0b00000011|(~0b00000000 & (out1) );\
	IO_._PDR[CH2P_('2')]=0b00000000|(~0b00000000 & (out2) );\
	IO_._PDR[CH2P_('3')]=0b00000000|(~0b00100000 & (out3) );/*P35=NMI*/\
	IO_._PDR[CH2P_('4')]=0b00000000|(~0b00000000 & (out4) );\
	IO_._PDR[CH2P_('5')]=0b11000000|(~0b00000000 & (out5) );\
	IO_._PDR[CH2P_('6')]=0b11111111|(~0b00000000 & (out6) );\
	IO_._PDR[CH2P_('7')]=0b11111111|(~0b00000000 & (out7) );\
	IO_._PDR[CH2P_('8')]=0b11111111|(~0b00000000 & (out8) );\
	IO_._PDR[CH2P_('9')]=0b11111111|(~0b00000000 & (out9) );\
	IO_._PDR[CH2P_('A')]=0b00000000|(~0b00000000 & (outa) );\
	IO_._PDR[CH2P_('B')]=0b00000000|(~0b00000000 & (outb) );\
	IO_._PDR[CH2P_('C')]=0b00000000|(~0b00000000 & (outc) );\
	IO_._PDR[CH2P_('D')]=0b00000000|(~0b00000000 & (outd) );\
	IO_._PDR[CH2P_('E')]=0b00000000|(~0b00000000 & (oute) );\
	IO_._PDR[CH2P_('F')]=0b11111111|(~0b00000000 & (outf) );\
	IO_._PDR[CH2P_('G')]=0b11111111|(~0b00000000 & (outg) );\
	IO_._PDR[CH2P_('J')]=0b11110111|(~0b00000000 & (outj) );/*PJ3*/\
	/*100-pin:           IO-pins:79 input-only:1*/\
} while(0)
