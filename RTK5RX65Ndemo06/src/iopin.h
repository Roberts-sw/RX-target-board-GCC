#ifndef IOPIN_H_
#define IOPIN_H_
/* iopin.h
   ------------------------------------------ TAB-size 4, code page UTF-8 --
boot modes:
	MD UB = 00|01|1x = SCI-|USB-|FINE-/USER-boot => MD:Rpu UB:Rpd

wijzigingen:
	RvL 17-3-2020	aanmaak
------------------------------------------------------------------------- */
//#include <iodefine.h>	//not used, see #define IO_, shortens build time
#include <stdint.h>

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
#define IO_LO	0b00000000	//data : PODRx='0' for high init
#define IO_HI	0b00000001	//data : PODRx='1' for high init
#define IO_IN	0b00000010	//input : PDRx='0'
#define IO_PE	0b00000100	//peripheral: PMRx='1'
#define IO_ON	0b00001000	//open-Drain N-channel: ODRxy='01'
#define IO_OP	0b00010000	//open-Drain P-channel: ODRxy='10'
#define IO_PU	0b00100000	//Rpull-up: PCRx='1'
#define IO_CA	0b01000000	//Drive Capacity control: DSCRx='1'
#define IO_SP	0b10000000	//Drive Speed control: DSCR2x='1'

//HW 1.5 Pin Assignments
#define RX65x100(X)\
/*	X( 4 ,J3,IO_LO	,-			,0,-					)*/\
	X(11 ,37,IO_LO	,-			,0,XTAL					)\
	X(13 ,36,IO_LO	,-			,0,EXTAL				)\
	X(15 ,35,IO_IN	,-			,0,NMI/UPSEL			)/*0|1=self-/USB-powered*/\
	X(16 ,34,IO_IN	,-			,2,P16_USB_MICRO_VBUS	)\
	X(17 ,33,IO_LO	,-			,2,P33_GROVE_RXD6		)/*RxD6*/\
	X(18 ,32,IO_LO	,PMOD1-RST	,2,P32_GROVE_TXD6		)/*TxD6*/\
	X(19 ,31,IO_LO	,-			,2,P31_IRQ1_SW			)/*IRQ1*/\
	X(20 ,30,IO_IN	,-			,2,P30_PMOD_SSCL1_PIN10	)/*10:SCL1*/\
	X(21 ,27,IO_LO	,-			,0,-					)\
	X(22 ,26,IO_LO	,-			,2,P26_PMOD_SSDA1_PIN9	)/* 9:SDA1*/\
	X(23 ,25,IO_LO	,-			,0,-					)\
	X(24 ,24,IO_LO	,-			,0,-					)\
	X(25 ,23,IO_LO	,-			,2,P23_PMOD_SCI0_CTS_SS	)/* 1:SS0 */\
\
	X(26 ,22,IO_LO	,-			,2,P22_PMOD_SCI0_CLK	)/* 4:SCK0*/\
	X(27 ,21,IO_LO	,-			,2,P21_PMOD_SCI0_RX_MISO)/* 3:RxD0*/\
	X(28 ,20,IO_LO	,-			,2,P20_PMOD_SCI0_TX_MOSI)/* 2:TxD0*/\
	X(29 ,17,IO_LO	,-			,0,-					)\
	X(30 ,16,IO_LO	,-			,0,-					)\
	X(31 ,15,IO_LO	,PMOD1_IRQ	,2,P34_PMOD_IRQ4_GPIO	)/* 7:IRQ4*/\
	X(32 ,14,IO_LO	,-			,0,-					)\
	X(33 ,13,IO_LO	,-			,2,P13_I2C_SDA0			)/*SDA0*/\
	X(34 ,12,IO_LO	,-			,2,P12_I2C_SCL0			)/*SCL0*/\
	/*	IIC-addresses: IC2=ISL29035:0x44 IC3=BME680:0x76 IC4=BMI680:0x68	*/\
/*	X(35 ,H3,IO_IN	,-			,2,VCC_USB				)*/\
/*	X(36 ,H2,IO_IN	,-			,2,PH2_USB_MICRO_DM		)*/\
/*	X(37 ,H1,IO_IN	,-			,2,PH1_USB_MICRO_DP		)*/\
	X(39 ,55,IO_LO	,-			,0,-					)\
	X(40 ,54,IO_LO	,-			,0,-					)\
	X(41 ,53,IO_LO	,-			,0,-					)\
	X(42 ,52,IO_LO	,-			,0,-					)\
	X(43 ,51,IO_LO	,-			,0,-					)\
	X(44 ,50,IO_LO	,-			,2,OB_POWER				)/*low=disable VCC3V3*/\
	X(45 ,C7,IO_HI	,-			,0,-					)/*USB boot*/\
	X(46 ,C6,IO_LO	,-			,0,-					)\
	X(47 ,C5,IO_LO	,-			,0,-					)\
	X(48 ,C4,IO_LO	,-			,0,-					)\
	X(49 ,C3,IO_HI	,PMOD1-IO0	,1,-					)/* 9:IO0*/\
	X(50 ,C2,IO_IN	,PMOD1-IO1	,1,-					)/*10:IO1*/\
\
	X(51 ,C1,IO_LO	,-			,0,-					)\
	X(52 ,C0,IO_LO	,-			,2,PC0_RTS5_USBSERIAL	)/*RTS5*/\
	X(53 ,B7,IO_HI	,-			,2,PB7_GROVE_SSDA9		)/*SDA9*/\
	X(54 ,B6,IO_LO	,-			,2,PB6_GROVE_SSCL9		)/*SCL9*/\
	X(55 ,B5,IO_LO	,-			,0,-					)\
	X(56 ,B4,IO_LO	,-			,0,-					)\
	X(57 ,B3,IO_LO	,-			,0,-					)\
	X(58 ,B2,IO_LO	,-			,0,-					)\
	X(59 ,B1,IO_HI	,TB_nSW1	,2,PB1_LED2				)/*IRQ4*/\
	X(61 ,B0,IO_LO	,-			,2,PB0_LED1				)\
	X(63 ,A7,IO_LO	,PMOD1-MISO	,1,-					)/* 3:MISOA-B */\
	X(64 ,A6,IO_IN	,PMOD1-MOSI	,1,-					)/* 2:MOSIA-B */\
	X(65 ,A5,IO_LO	,PMOD1-SCK	,2,PA5_CTS5_USBSERIAL	)/* 4:RSPCKA-B*/\
	X(66 ,A4,IO_HI	,PMOD1-CS	,2,PA4_TXD5_USBSERIAL	)/* 1:SSLA0-B */\
	X(67 ,A3,IO_IN	,-			,2,PA3_RXD5_USBSERIAL	)\
	X(68 ,A2,IO_LO	,-			,0,-					)\
	X(69 ,A1,IO_LO	,-			,0,-					)\
	X(70 ,A0,IO_LO	,-			,0,-					)\
	X(71 ,E7,IO_LO	,-			,2,PE7_IRQ7_IC4_INT2	)/*BMI680*/\
	X(72 ,E6,IO_LO	,-			,2,PE6_IRQ6_IC4_INT1	)/*BMI680*/\
	X(73 ,E5,IO_LO	,-			,2,PE5_IRQ5_IC2_INT		)/*ISL29035*/\
	X(74 ,E4,IO_LO	,-			,0,-					)\
	X(75 ,E3,IO_LO	,-			,2,PE5_IRQ5_IC2_INT		)\
\
	X(76 ,E2,IO_IN	,-			,0,-					)\
	X(77 ,E1,IO_LO	,-			,0,-					)\
	X(78 ,E0,IO_LO	,-			,0,-					)\
	X(79 ,D7,IO_LO	,TB_nLED1	,3,PD7_QIO01			)/*MX25L3233F*/\
	X(80 ,D6,IO_LO	,TB_nLED0	,3,PD6_QIO00			)/*MX25L3233F*/\
	X(81 ,D5,IO_LO	,-			,3,PD5_QSPCLK			)/*MX25L3233F*/\
	X(82 ,D4,IO_LO	,-			,3,PD4_QSSL				)/*MX25L3233F*/\
	X(83 ,D3,IO_LO	,-			,3,PD3_QIO03			)/*MX25L3233F*/\
	X(84 ,D2,IO_LO	,-			,3,PD2_QIO02			)/*MX25L3233F*/\
	X(85 ,D1,IO_LO	,-			,0,-					)\
	X(86 ,D0,IO_LO	,-			,2,PD0_PMOD_IRQ0_GPIO	)/* 8:IRQ0*/\
	X(87 ,47,IO_LO	,-			,0,-					)\
	X(88 ,46,IO_HI	,-			,0,-					)\
	X(89 ,45,IO_LO	,-			,0,-					)\
	X(90 ,44,IO_LO	,-			,0,-					)\
	X(91 ,43,IO_LO	,-			,0,-					)\
	X(92 ,42,IO_LO	,-			,0,-					)\
	X(93 ,41,IO_LO	,-			,2,P41_GROVE_ADC1		)\
	X(95 ,40,IO_LO	,-			,2,P40_GROVE_ADC0		)\
	X(98 ,07,IO_LO	,-			,0,-					)\
	X(100,05,IO_LO	,-			,0,-					)\
	//ic  io init	assign		 e func-alt
//e:
//   https://reference.digilentinc.com/_media/reference/pmod/pmod-interface-specification-1_2_0.pdf
//1: https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/r12um0038ej0100-cloud.pdf
//2: https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/001/r12um0039eg0100-cloud.pdf
//3: https://www.macronix.com/Lists/Datasheet/Attachments/7426/MX25L3233F,%203V,%2032Mb,%20v1.6.pdf
#define CHIP( X )	RX65x100( X )//RX65x64( X )//RX65x144( X )//RX65x176( X )//

#define ENUM2(a,b,...)		P ## b,
typedef enum en_iopin		{PJ3, CHIP(ENUM2) eIOPINS	} eIOPIN;
#define _isiopin(iopin)		(0<=iopin&&eIOPINS>iopin)	//_isiopin(PJ3) => 1
extern uint8_t const ICPIN[eIOPINS];					//ICPIN[PJ3] => 4

void iopin_dir(eIOPIN iopin, uint8_t asoutput);
void iopin_init(eIOPIN iopin, uint8_t iomode);			//HW 22 + #define IO_??
void iopin_mpcfunc(eIOPIN iopin, uint8_t mpcfunc);		//HW 23, p.830-892
int iopin_read(eIOPIN iopin);
void iopin_toggle(eIOPIN iopin);
void iopin_write(eIOPIN iopin, uint8_t ashigh);


	/* ---------------------------------------------------------
	protected (todo)
	--------------------------------------------------------- */
	//HW 22. I/O Ports
#define IO_ (*(struct\
{	uint8_t _PDR[32],_PODR[32],_PIDR[32],_PMR[32];\
struct {uint8_t _0,_1;} _ODR[32];\
uint8_t _PCR[32],_DSCR[32],_fill[0x28],_DSCR2[32];\
} volatile *const)0x0008C000)

#define iopin_alldirs(	out0,out1,out2,out3,out4,out5,out6,out7,\
					out8,out9,outa,outb,outc,outd,oute,outf,	outg,outj)\
do{	IO_._PDR[ 0]=0b01011111|(~0b00000000 & (out0) );\
	IO_._PDR[ 1]=0b00000000|(~0b00000000 & (out1) );\
	IO_._PDR[ 2]=0b00000000|(~0b00000000 & (out2) );\
	IO_._PDR[ 3]=0b00000000|(~0b00100000 & (out3) );/*P35=NMI*/\
	IO_._PDR[ 4]=0b00000000|(~0b00000000 & (out4) );\
	IO_._PDR[ 5]=0b11000000|(~0b00000000 & (out5) );\
	IO_._PDR[ 6]=0b11111111|(~0b00000000 & (out6) );\
	IO_._PDR[ 7]=0b11111111|(~0b00000000 & (out7) );\
	IO_._PDR[ 8]=0b11111111|(~0b00000000 & (out8) );\
	IO_._PDR[ 9]=0b11111111|(~0b00000000 & (out9) );\
	IO_._PDR[10]=0b00000000|(~0b00000000 & (outa) );\
	IO_._PDR[11]=0b00000000|(~0b00000000 & (outb) );\
	IO_._PDR[12]=0b00000000|(~0b00000000 & (outc) );\
	IO_._PDR[13]=0b00000000|(~0b00000000 & (outd) );\
	IO_._PDR[14]=0b00000000|(~0b00000000 & (oute) );\
	IO_._PDR[15]=0b11111111|(~0b00000000 & (outf) );\
	IO_._PDR[16]=0b11111111|(~0b00000000 & (outg) );\
	/*	no PORTH, skip PORTI-number	*/\
	IO_._PDR[18]=0b11110111|(~0b00000000 & (outj) );/*PJ3*/\
} while(0)

#endif /* IOPIN_H_ */
