#ifndef RX65x_H_
#define RX65x_H_
/* rx65x.h 
   ------------------------------------------ TAB-size 4, code page UTF-8 --
hardware manual (HW):
	https://www.renesas.com/eu/en/doc/products/mpumcu/doc/rx_family/001/r01uh0590ej0230-rx651.pdf

Wijzigingen:
	RvL  3-4-2020	+ bitstructs + Endian + module stop bits
	RvL 29-3-2020	opname in lib/src
	RvL 23-3-2020	aanmaak, zie ../../doc/nl/header.pdf
------------------------------------------------------------------------- */
#define NOP()	__asm("nop")//asm not optimized out?

	//get-macro:
#define bf_xtr_(c,p,sz) 	((c) >> (p) & bit_(sz) - 1 )
#define bit_(c)     		(1ULL<< (c) )
#define bitget_(c,b)		get_((c),bit_(b) )	//8 == bitget_(~0,3)
#define bitval_(c,b)		((c) >> (b) & 1)	//1 == bitval_(~0,3)
#define elements_(arr)		(sizeof(arr)/sizeof(*arr) )
#define get_(c,m)   		((c) &  (m) )
#define lsbget_(c)  		((c) & -(c) )		//-c == ~c+1
#define mask_(p,sz) 		(bit_(sz) - 1 << (p) )
#define maxindex_(arr)		(elements_(arr)-1)

	//set-macro:
#define bf_ins_(v,p,sz,n)	insert_((v), mask_((p),(sz) ),(n)<<(p) )
#define bitclr_(v,b)		clr_((v),bit_(b) )
#define bitinv_(v,b)		inv_((v),bit_(b) )
#define bitset_(v,b)		set_((v),bit_(b) )
#define clr_(v,m)   		((v)&=~(m) )
#define clrset_(v,m,n)		((v) = (v) & ~(m) | (n) )
#define inv_(v,m)   		((v)^= (m) )
#define set_(v,m)   		((v)|= (m) )
#define insert_(v,m,n)		inv_((v),(m)&((v)^(n) ) )
#define lsbclr_(v)  		((v) &= (v)-1)
#define xmax_(x,M)   		if(x>(M) )	x=(M)
#define xmin_(x,m)   		if(x<(m) )	x=(m)
#define xlim_(x,m,M) 		do{xmin_(x,m); else xmax_(x,M);} while(0)

#define v(pos, sz, ...)     u08 _ ## pos[0x ## sz]	//void (positioned) array

	//fixed-width types (3 characters to define signedness and width) GCC for RX
typedef signed char s08;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u08;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

	//HW 2. CPU
	//HW 2.5 Endian
#ifndef __RX_LITTLE_ENDIAN__
	//Big-Endian: byte == b7..b0, prefix access type, R=reversed-endian
#	define  U32_BIT(x) (*(BITS31_0 *)(&(x) ) )
#	define  U16_BIT(x) (*(BITS15_0 *)(&(x) ) )
#	define  U08_BIT(x) (*(BITS7_0  *)(&(x) ) )
#	define RU32_BIT(x) (*(BITS0_31 *)(&(x) ) )
#	define RU16_BIT(x) (*(BITS0_15 *)(&(x) ) )
#	define RU08_BIT(x) (*(BITS0_7  *)(&(x) ) )
#else
	//Little-Endian: byte == b0..b7, prefix access type, R=reversed-endian
#	define  U08_BIT(x) (*(BITS0_7  *)(&(x) ) )
#	define  U16_BIT(x) (*(BITS0_15 *)(&(x) ) )
#	define  U32_BIT(x) (*(BITS0_31 *)(&(x) ) )
#	define RU08_BIT(x) (*(BITS7_0  *)(&(x) ) )
#	define RU16_BIT(x) (*(BITS15_0 *)(&(x) ) )
#	define RU32_BIT(x) (*(BITS31_0 *)(&(x) ) )
#endif


	//HW 2.6.1 Exception Vector Table
	//HW 2.6.2 Interrupt Vector Table

	//HW 5. I/O-Registers, p. 187
	//  (2) Notes on writing to I/O registers:
#define IOREG_WRITE(r,v)	do{(r)=(v); if((r)!=(v)) NOP();} while(0)
#define WRITE_CHK(r,v)		do (r)=(v); while( (r)!=(v) )

	//HW 7. Option-Setting Memory (OFSM)
	
	/* ---------------------------------------------------------
	system
		HW 3. Operating Modes
		HW 6. Resets
		HW 8. Voltage Detection Circuit (LVDA)
		HW 9. Clock Generation Circuit
		HW 11. Low Power Consumption
		HW 13. Register Write Protection Function
	--------------------------------------------------------- */
#define SYSTEM_ (*(volatile struct {\
/*0000*/u16 MDMONR,_0002,_0004,SYSCR0,SYSCR1,_000a,SBYCR,_000e;\
/*0010*/u32 MSTPCRA,MSTPCRB,MSPCRC,MSTPCRD;\
/*0020*/u32 SCKCR; u16 SCKCR2,SCKCR3,PLLCR; u08 PLLCR2; v(002b,5);\
/*0030*/u08 BCKCR,_0031,MOSCCR,SOSCCR,LOCOCR_,ILOCOCR,HOCOCR,HOCOCR2;\
    v(0038,4); u08 OSCOVFSR,_003d,_003e,_003f;\
/*0040*/u08 OSTDCR,OSTDSR; v(0042,5e);\
/*00a0*/u08 OPCCR,RSTCKCR,MOSCWTCR,SOSCWTCR; v(00a4,1c);\
/*00c0*/u08 RSTSR2,_00c1; u16 SWRR; v(00c4,1c);\
/*00e0*/u08 LVD1CR1,LVD1SR,LVD2CR1,LVD2SR; v(00e4,31a); u16 PRCR; v(0400,c1c);\
/*101c*/u08 ROMWT; v(101d,b263);\
/*c280*/u08 DPSBYCR,_c281,DPSIER0,DPSIER1,DPSIER2,DPSIER3,DPSIFR0,DPSIFR1,\
    DPSIFR2,DPSIFR3,DPSIEGR0,DPSIEGR1,DPSIEGR2,DPSIEGR3,_c28e,_c28f;\
/*c290*/u08 RSTSR0,RSTSR1,_c292,MOFCR,HOCOPCR,_c295,_c296,LVCMPCR,\
    LVDLVLR,_c299,LVD1CR0,LVD2CR0; v(c29c,4);\
/*c2a0*/u08 DPSBKR[32];\
} *const)0x00080000)

enum en_ck_div {CK_1, CK_2, CK_4, CK_8, CK_16, CK_32, CK_64};	//0..6
enum en_sck {CKS_LOCO,CKS_HOCO,CKS_MAIN,CKS_SUB,CKS_PLL};		//0..4
enum en_oscovfsr {b00_MOOVF, b01_SOOVF, b02_PLOVF, b03_HCOVF, b04_ILCOVF};

#define MSTP_EXDMAC_  U32_BIT(SYSTEM_.MSTPCRA).B29
#define MSTP_DMADTC_  U32_BIT(SYSTEM_.MSTPCRA).B28
#define MSTP_DA_      U32_BIT(SYSTEM_.MSTPCRA).B19
#define MSTP_S12AD_   U32_BIT(SYSTEM_.MSTPCRA).B17
#define MSTP_S12AD1_  U32_BIT(SYSTEM_.MSTPCRA).B16
#define MSTP_CMT01_   U32_BIT(SYSTEM_.MSTPCRA).B15
#define MSTP_CMT23_   U32_BIT(SYSTEM_.MSTPCRA).B14
#define MSTP_TPU_     U32_BIT(SYSTEM_.MSTPCRA).B13
#define MSTP_PPG0_    U32_BIT(SYSTEM_.MSTPCRA).B11
#define MSTP_PPG1_    U32_BIT(SYSTEM_.MSTPCRA).B10
#define MSTP_MTU_     U32_BIT(SYSTEM_.MSTPCRA).B9
#define MSTP_TMR01_   U32_BIT(SYSTEM_.MSTPCRA).B5
#define MSTP_TMR23_   U32_BIT(SYSTEM_.MSTPCRA).B4
#define MSTP_CMTW0_   U32_BIT(SYSTEM_.MSTPCRA).B1
#define MSTP_CMTW1_   U32_BIT(SYSTEM_.MSTPCRA).B0

#define MSTP_SCI0_    U32_BIT(SYSTEM_.MSTPCRB).B31
#define MSTP_SCI1_    U32_BIT(SYSTEM_.MSTPCRB).B30
#define MSTP_SCI2_    U32_BIT(SYSTEM_.MSTPCRB).B29
#define MSTP_SCI3_    U32_BIT(SYSTEM_.MSTPCRB).B28
#define MSTP_SCI4_    U32_BIT(SYSTEM_.MSTPCRB).B27
#define MSTP_SCI5_    U32_BIT(SYSTEM_.MSTPCRB).B26
#define MSTP_SCI6_    U32_BIT(SYSTEM_.MSTPCRB).B25
#define MSTP_SCI7_    U32_BIT(SYSTEM_.MSTPCRB).B24
#define MSTP_CRC_     U32_BIT(SYSTEM_.MSTPCRB).B23
#define MSTP_PDC_     U32_BIT(SYSTEM_.MSTPCRB).B22
#define MSTP_RIIC0_   U32_BIT(SYSTEM_.MSTPCRB).B21
#define MSTP_RIIC1_   U32_BIT(SYSTEM_.MSTPCRB).B20
#define MSTP_USB0_    U32_BIT(SYSTEM_.MSTPCRB).B19
#define MSTP_RSPI0_   U32_BIT(SYSTEM_.MSTPCRB).B17
#define MSTP_RSPI1_   U32_BIT(SYSTEM_.MSTPCRB).B16
#define MSTP_ETHERC0_ U32_BIT(SYSTEM_.MSTPCRB).B15
#define MSTP_ELC_     U32_BIT(SYSTEM_.MSTPCRB).B9
#define MSTP_TEMPS_   U32_BIT(SYSTEM_.MSTPCRB).B8
#define MSTP_DOC_     U32_BIT(SYSTEM_.MSTPCRB).B6
#define MSTP_SCI12_   U32_BIT(SYSTEM_.MSTPCRB).B4
#define MSTP_CAN1_    U32_BIT(SYSTEM_.MSTPCRB).B1
#define MSTP_CAN0_    U32_BIT(SYSTEM_.MSTPCRB).B0

#define MSTP_GLCDC_   U32_BIT(SYSTEM_.MSTPCRC).B29
#define MSTP_DRW2D_   U32_BIT(SYSTEM_.MSTPCRC).B28
#define MSTP_SCI8_    U32_BIT(SYSTEM_.MSTPCRC).B27
#define MSTP_SCI9_    U32_BIT(SYSTEM_.MSTPCRC).B26
#define MSTP_SCI10_   U32_BIT(SYSTEM_.MSTPCRC).B25
#define MSTP_SCI11_   U32_BIT(SYSTEM_.MSTPCRC).B24
#define MSTP_QSPI_    U32_BIT(SYSTEM_.MSTPCRC).B23
#define MSTP_RSPI2_   U32_BIT(SYSTEM_.MSTPCRC).B22
#define MSTP_CAC_     U32_BIT(SYSTEM_.MSTPCRC).B19
#define MSTP_RIIC2_   U32_BIT(SYSTEM_.MSTPCRC).B17
#define MSTP_STBYRAM_ U32_BIT(SYSTEM_.MSTPCRC).B7
#define MSTP_RAM2_    U32_BIT(SYSTEM_.MSTPCRC).B2
#define MSTP_RAM0_    U32_BIT(SYSTEM_.MSTPCRC).B0

#define MSTP_TSIP_    U32_BIT(SYSTEM_.MSTPCRD).B27
#define MSTP_MMCIF_   U32_BIT(SYSTEM_.MSTPCRD).B21
#define MSTP_SDHI_    U32_BIT(SYSTEM_.MSTPCRD).B19
#define MSTP_SDSI_    U32_BIT(SYSTEM_.MSTPCRD).B13

	/* ---------------------------------------------------------
	icu
		HW 15. Interrupt Controller (ICUB)
	--------------------------------------------------------- */
#define ICU_ (*(volatile struct {\
/*7000*/u08 IR[256];	/*16..255*/\
/*7100*/u08 DTCER[256];	/*26..255*/\
/*7200*/u08 IER[32];	/*2..31*/ v(7220,c0);\
/*72e0*/u08 SWINTR,SWINT2R; v(7222,e);\
/*72f0*/u16 FIR; v(72f2,e);\
/*7300*/u08 IPR[256];\
/*7400*/u08 DMRSR0;v(7401,3); u08 DMRSR1;v(7405,3);\
/*7408*/u08 DMRSR2;v(7409,3); u08 DMRSR3;v(740d,3);\
/*7410*/u08 DMRSR4;v(7411,3); u08 DMRSR5;v(7415,3);\
/*7418*/u08 DMRSR6;v(7419,3); u08 DMRSR7;v(741d,e3);\
/*7500*/u08 IRQCR[16]; v(7510,10);\
/*7520*/u08 IRQFLTE0,IRQFLTE1; v(7522,6);\
/*7528*/u16 IRQFLTC0,IRQFLTC1; v(752c,54);\
/*7580*/u08 NMISR,NMIER,NMICLR,NMICR; v(7584,c);\
/*7590*/u08 NMIFLTE; v(7591,3); u08 NMIFLTC; v(7595,6b);\
/*7600*/u32 GRPBE0; v(7604,2c);\
/*7630*/u32 GRPBL0,GRPBL1,GRPBL2; v(763c,4);\
/*7640*/u32 GENBE0; v(7644,2c);\
/*7670*/u32 GENBL0,GENBL1,GENBL2; v(767c,4);\
/*7680*/u32 GCRBE0; v(7684,7c);\
/*7700*/u08 PIBR0,PIBR1,PIBR2,PIBR3,PIBR4,PIBR5,PIBR6,PIBR7,\
    PIBR8,PIBR9,PIBRA,PIBRB; v(770c,74);\
/*7780*/u08 SLIBXR128,SLIBXR129,SLIBXR130,SLIBXR131,\
        SLIBXR132,SLIBXR133,SLIBXR134,SLIBXR135,\
        SLIBXR136,SLIBXR137,SLIBXR138,SLIBXR139,\
        SLIBXR140,SLIBXR141,SLIBXR142,SLIBXR143,\
    SLIBR144,SLIBR145,SLIBR146,SLIBR147,SLIBR148,SLIBR149,SLIBR150,SLIBR151,\
    SLIBR152,SLIBR153,SLIBR154,SLIBR155,SLIBR156,SLIBR157,SLIBR158,SLIBR159,\
    SLIBR160,SLIBR161,SLIBR162,SLIBR163,SLIBR164,SLIBR165,SLIBR166,SLIBR167,\
    SLIBR168,SLIBR169,SLIBR170,SLIBR171,SLIBR172,SLIBR173,SLIBR174,SLIBR175,\
    SLIBR176,SLIBR177,SLIBR178,SLIBR179,SLIBR180,SLIBR181,SLIBR182,SLIBR183,\
    SLIBR184,SLIBR185,SLIBR186,SLIBR187,SLIBR188,SLIBR189,SLIBR190,SLIBR191,\
    SLIBR192,SLIBR193,SLIBR194,SLIBR195,SLIBR196,SLIBR197,SLIBR198,SLIBR199,\
    SLIBR200,SLIBR201,SLIBR202,SLIBR203,SLIBR204,SLIBR205,SLIBR206,SLIBR207;\
v(77d0,60);\
/*7830*/u32 GRPAL0,GRPAL1; v(7838,38);\
/*7870*/u32 GENAL0,GENAL1; v(7878,88);\
/*7900*/u08 PIAR0,PIAR1,PIAR2,PIAR3,PIAR4,PIAR5;v(7906,5);u08 PIARB;v(790c,c4);\
/*79d0*/u08\
    SLIAR208,SLIAR209,SLIAR210,SLIAR211,SLIAR212,SLIAR213,SLIAR214,SLIAR215,\
	SLIAR216,SLIAR217,SLIAR218,SLIAR219,SLIAR220,SLIAR221,SLIAR222,SLIAR223,\
	SLIAR224,SLIAR225,SLIAR226,SLIAR227,SLIAR228,SLIAR229,SLIAR230,SLIAR231,\
	SLIAR232,SLIAR233,SLIAR234,SLIAR235,SLIAR236,SLIAR237,SLIAR238,SLIAR239,\
	SLIAR240,SLIAR241,SLIAR242,SLIAR243,SLIAR244,SLIAR245,SLIAR246,SLIAR247,\
	SLIAR248,SLIAR249,SLIAR250,SLIAR251,SLIAR252,SLIAR253,SLIAR254,SLIAR255,\
    SLIPRCR, SELEXDR;\
} *const)0x00087000)


	//HW 17. Memory-Protection Unit (MPU)


	//HW 18. DMA Controller (DMACAa)
	
	
	//HW 19. EXDMA Controller (EXDMACa)
	
	
	//HW 20. Data Transfer Controller (DTCb)
	
	
	//HW 21. Event Link Controller (ELC)


	//HW 22. I/O Ports
//#define IO_ (*(volatile struct\
{   u08 _PDR[32],_PODR[32],_PIDR[32],_PMR[32];\
    u08 _ODR[32][2],_PCR[32],_DSCR[32],v(c100,28),_DSCR2[32];\
} *const)0x0008C000)
#define IO_ (*(volatile struct\
{	u08 _PDR[32],_PODR[32],_PIDR[32],_PMR[32];\
	struct {u08 _0,_1;} _ODR[32];\
	u08 _PCR[32],_DSCR[32],_fill[0x28],_DSCR2[32];\
} *const)0x0008C000)
#define iopin_alldirs(\
	out0,out1,out2,out3,out4,out5,out6,out7,\
	out8,out9,outa,outb,outc,outd,oute,outf,\
	outg,outj)\
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


	//HW 23. Multi-Function Pin Controller (MPC)
#define MPC_ (*(volatile struct/*PmnPFS[]: no PORTH, skip PORTI-number*/\
{	u08 PFCSE,_01,PFCSS0,PFCSS1,PFAOE0,PFAOE1,PFBCR0,PFBCR1,PFBCR2,PFBCR3,\
	_0a[4],PFENET,_0f[0x10],PWPR,_20[0x20],PFS[19*8];\
} *const)0x0008C100)
#define PWPR_ (*(volatile u08 *const)0x0008c11f)
	/*PmnPFS(port,pin): no PORTH, skip PORTI-number*/
#define PmnPFS(port,pin) ((volatile u08 *const)0x0008C140)[8*(port)+(pin)]
#define PFS_unlock() do{PWPR_&=~0x80;PWPR_|=0x40;}while(0)
#define PFS_relock() do{PWPR_&=~0x40;PWPR_|=0x80;}while(0)


	//HW 24. Multi-Function Timer Pulse Unit 3 (MTU3a)

	/* ---------------------------------------------------------
	mtu general
	--------------------------------------------------------- */
#define MTU_ (*(volatile struct {\
/*1200*/u08 _1200[10],TOERA,_120b,_120c,TGCRA,TOCR1A,TOCR2A;\
/*1210*/u16 _1210,_1212,TCDRA,TDDRA, _1218,_121a,_121c,_121e;\
/*1220*/u16 TCNTSA,TCBRA,_1224,_1226,_1228,_122a,_122c,_122e;\
/*1230*/u08 TITCR1A,TITCNT1A,TBTERA,_1233,TDERA,_1235,TOLBRA,_1237,\
    _1238,_1239,TITMRA,TITCR2A,TITCNT2A,_123d,_123e[0x22];\
/*1260*/u08 TWCRA,_1261[15],TMDR2A,_1271[15];\
/*1280*/u08 TSTRA,TSYRA,TCSYSTR,_1283,TRWERA,_1285[0x77b];\
\
/*1a00*/u08 _1a00[10],TOERB,_1a0b,_1a0c,_1a0d,TOCR1B,TOCR2B;\
/*1a10*/u16 _1a10,_1a12,TCDRB,TDDRB, _1a18,_1a1a,_1a1c,_1a1e;\
/*1a20*/u16 TCNTSB,TCBRB,_1a24,_1a26,_1a28,_1a2a,_1a2c,_1a2e;\
/*1a30*/u08 TITCR1B,TITCNT1B,TBTERB,_1a33,TDERB,_1a35,TOLBRB,_1a37,\
    _1a38,_1a39,TITMRB,TITCR2B,TITCNT2B,_1a3d,_1a3e[0x22];\
/*1a60*/u08 TWCRB,_1a61[15],TMDR2B,_1a71[15];\
/*1a80*/u08 TSTRB,TSYRB,_1a82,_1a83,TRWERB;\
} *const)0x000C1200)

#define MTU_CST0	U08_BIT(MTU_.TSTRA).B0
#define MTU_CST1	U08_BIT(MTU_.TSTRA).B1
#define MTU_CST2	U08_BIT(MTU_.TSTRA).B2
#define MTU_CST3	U08_BIT(MTU_.TSTRA).B6
#define MTU_CST4	U08_BIT(MTU_.TSTRA).B7
#define MTU_CST6	U08_BIT(MTU_.TSTRB).B6
#define MTU_CST7	U08_BIT(MTU_.TSTRB).B7
#define MTU_CST8	U08_BIT(MTU_.TSTRA).B3
#define MTU_CSTU5	U08_BIT(MTU_.TSTR).B2
#define MTU_CSTV5	U08_BIT(MTU_.TSTR).B1
#define MTU_CSTW5	U08_BIT(MTU_.TSTR).B0

	/* ---------------------------------------------------------
	mtu0
	mtu1
	mtu2
	mtu8
	--------------------------------------------------------- */
struct stMTU012/*common to mtu0..2*/
{	u08 TCR,TMDR1; union {u08 TIOR; u08 TIORH;}; u08 TIORL,TIER,TSR;
	u16 TCNT,TGRA,TGRB;
};/*=>0x0c*/

	//MTU0: TIORH TIORL, no TSR
#define MTU0_ (*(volatile struct {/*MTU1: SLIA-source 1-7=TGIA0..D0,TCIV0,TGIE0..F0*/\
/*1290*/u08 NFCR0,_1291[8],NFCRC,_129a[0x66];\
/*1300*/struct stMTU012 /*anonymous*/; u16 TGRC,TGRD;u08 _1310[16];\
/*1320*/u16 TGRE,TGRF;u08 TIER2,_1325,TBTM,_1327,TCR2;\
} *const)0x000C1290)

	//MTU1: TIOR, no TIORL
#define MTU1_ (*(volatile struct {/*SLIA-source 8-11=TGIA1,TGIB1,TGIV1,TGIU1*/\
/*1291*/u08 NFCR1,_1292[0xee];\
/*1380*/struct stMTU012 /*anonymous*/; u08 _138c[4];\
/*1390*/u08 TICCR,TMDR3,_1392,_1393,TCR2,_1395[11];\
/*13a0*/u32 TCNTLW,TGRALW,TGRBLW;\
} *const)0x000C1291)

	//MTU2: TIOR, no TIORL
#define MTU2_ (*(volatile struct {/*SLIA-source 12-15=TGIA2,TGIB2,TGIV2,TGIU2*/\
/*1292*/u08 NFCR2,_1293[0x16d];\
/*1400*/struct stMTU012 /*anonymous*/; u08 TCR2;\
} *const)0x000C1292)

#define MTU8_ (*(volatile struct{/*SLIA-source 41-45=TGIA8..D8,TCIV8*/\
/*1298*/u08 NFCR8,_1299_15ff[0x367];\
/*1600*/u08 TCR,TMDR1,TIORH,TIORL,TIER,_05,TCR2,_07;\
/*1608*/u32 TCNT,TGRA,TGRB,TGRC,TGRD;\
} *const)0x000C1298)

	/* ---------------------------------------------------------
	mtu3 + mtu4 interleaved
	mtu6 + mtu7 interleaved
	--------------------------------------------------------- */
	//common parts:
struct stMTU36_0 {u08 TCR,_01,TMDR1,_03,TIORH,TIORL,_06,_07,TIER,_09;};/*=>0a*/
struct stMTU36_1 {u08 _00,TCR,_02,TMDR1,_04,_05,TIORH,TIORL,_08,TIER;};
struct stMTU32_2
{u16 TCNT,_12,_14,_16,TGRA,TGRB,_1c,_1e,_20,_22,TGRC,TGRD,_28,_2a;};/*=>2c*/
struct stMTU32_3
{u16 _10,TCNT,_14,_16,_18,_1a,TGRA,TGRB,_20,_22,_24,_26,TGRC,TGRD;};

#define MTU3_ (*(volatile struct{/*MTU3: SLIA-source 16-20=TGIA3..D3,TCIV3*/\
/*1200*/struct stMTU36_0 /*anonymous*/; _120a[6];\
/*1210*/struct stMTU32_2 /*anonymous*/; u08 TSR,_2d,_2e,_2f;\
/*1230*/u08 _30_37[8],TBTM,_39,_3a_3f[6];\
/*1240*/u16 _40,_42,_44,_46,_48,_4a;u08 TCR2,_4d,_4e[0x22];\
/*1270*/u16 _70,TGRE,_74,_76; u08 _77[0x1c],NFCR3;\
} *const)0x000C1200)

#define MTU4_ (*(volatile struct{/*MTU4: SLIA-source 21-25=TGIA4..D4,TCIV4*/\
/*1200*/struct stMTU36_1 /*anonymous*/; _120a[6];\
/*1210*/struct stMTU32_3 /*anonymous*/; u08 _2c,TSR,_2e,_2f;\
/*1230*/u08 _30_37[8],_38,TBTM,_3a_3f[6];\
/*1240*/u16 TADCR,_42,TADCORA,TADCORB,TADCOBRA,TADCOBRB;u08 _4c,TCR2,_4e[0x22];\
/*1270*/u16 _70,_72,TGRE,TGRF;u08 _77[0x1c],_93,NFCR4;\
} *const)0x000C1200)

#define MTU6_ (*(volatile struct{/*MTU6: SLIA-source 30-34=TGIA6..D6,TCIV6*/\
/*1a00*/struct stMTU36_0 /*anonymous*/; _1a0a[6];\
/*1a10*/struct stMTU32_2 /*anonymous*/; u08 TSR,_2d,_2e,_2f;\
/*1a30*/u08 _30_37[8],TBTM,_39,_3a_3f[6];\
/*1a40*/u16 _40,_42,_44,_46,_48,_4a;u08 TCR2,_4d,_4e,_4f;\
/*1a50*/u08 TSYCR,_51[0x1f];\
/*1a70*/u16 _70,TGRE,_74,_76; u08 _77[0x1c],NFCR6;\
} *const)0x000C1a00)

#define MTU7_ (*(volatile struct{/*MTU7: SLIA-source 35-39=TGIA7..D7,TCIV7*/\
/*1a00*/struct stMTU36_1 /*anonymous*/; _1a0a[6];\
/*1a10*/struct stMTU32_3 /*anonymous*/; u08 _2c,TSR,_2e,_2f;\
/*1a30*/u08 _30_37[8],_38,TBTM,_3a_3f[6];\
/*1a40*/u16 TADCR,_42,TADCORA,TADCORB,TADCOBRA,TADCOBRB;u08 _4c,TCR2,_4e[0x22];\
/*1a70*/u16 _70,_72,TGRE,TGRF;u08 _77[0x1c],_93,NFCR7;\
} *const)0x000C1a00)

	/* ---------------------------------------------------------
	mtu5
	--------------------------------------------------------- */
#define MTU5_ (*(volatile struct{/*MTU5: SLIA-source 27-29=TGIU5,TGIV5,TGIW5*/\
/*1a94*/u08 _1A94,NCFR5,_1A96[0X1EA];\
/*1c80*/u16 TCNTU,TGRU; u08 TCRU,TCR2U,TIORU,_1C87[9];\
/*1c90*/u16 TCNTV,TGRV; u08 TCRV,TCR2V,TIORV,_1C97[9];\
/*1ca0*/u16 TCNTW,TGRW; u08 TCRW,TCR2W,TIORW,_1CA7[9];\
/*1cb0*/u08 _1CB0,_1CB1,TIER,_1CB3,TSTR,_1CB5,TCNTCMPCLR;\
} *const)0x000C1A94)


	//HW 25. Port Output Enable 3 (POE3a)


	//HW 26. 16-Bit Timer Pulse Unit (TPUa)


	//HW 27. Programmable Pulse Generator (PPG)


	//HW 28. 8-Bit Timer (TMR)


	//HW 29. Compare Match Timer (CMT)


	//HW 30. Compare Match Timer W (CMTW)


	//HW 31. Realtime Clock (RTCd)
#define RTC_ (*(volatile struct {\
/*c400*/u08 R64CNT,_c401,RSECCNT,_c403,RMINCNT,_c405,RHRCNT,_c407,\
	RWKCNT,_c409,RDAYCNT,_c40b,RMONCNT,_c40d; u16 RYRCNT;\
	u08 RSECAR,_c411,RMINAR,_c413,RHRAR,_c415,RWKAR,_c417,\
	RDAYAR,_c419,RMONAR,_c41b; u16 RYRAR; u08 RYRAREN,_c41f;\
/*c420*/u08 _c420,_c421,RCR1,_c423,RCR2,_c425,RCR3,_c427,\
	RCR4,_c429; u16 RFRH,RFRL; u08 RADJ; v(c42f,11);\
/*c440*/u08 RTCCR0,_c441,RTCCR1,_c443,RTCCR2; v(c445,d);\
/*c452*/u08 RSECCP0,_c453,RMINCP0,_c455,RHRCP0; v(c457,3);\
/*c45a*/u08 RDAYCP0,_c45b,RMONCP0;v(c45d,5);\
/*c462*/u08 RSECCP1,_c463,RMINCP1,_c465,RHRCP1; v(c467,3);\
/*c46a*/u08 RDAYCP1,_c46b,RMONCP1; v(c46d,5);\
/*c472*/u08 RSECCP2,_c473,RMINCP2,_c475,RHRCP2; v(c477,3);\
/*c47a*/u08 RDAYCP2,_c47b,RMONCP2;\
} *const)0x0008C400)

	/* ---------------------------------------------------------
	typedef
	--------------------------------------------------------- */

	//Big-Endian: long=b31..b0, word=b15..b0, byte=b7..b0
typedef struct st_bits31_24 {
u32 B31:1;u32 B30:1;u32 B29:1;u32 B28:1;u32 B27:1;u32 B26:1;u32 B25:1;u32 B24:1;
u32 B23:1;u32 B22:1;u32 B21:1;u32 B20:1;u32 B19:1;u32 B18:1;u32 B17:1;u32 B16:1;
u32 B15:1;u32 B14:1;u32 B13:1;u32 B12:1;u32 B11:1;u32 B10:1;u32 B9 :1;u32 B8 :1;
u32 B7 :1;u32 B6 :1;u32 B5 :1;u32 B4 :1;u32 B3 :1;u32 B2 :1;u32 B1 :1;u32 B0 :1;
} BITS31_0;
typedef struct st_bits15_0 {
u16 B15:1;u16 B14:1;u16 B13:1;u16 B12:1;u16 B11:1;u16 B10:1;u16 B9 :1;u16 B8 :1;
u16 B7 :1;u16 B6 :1;u16 B5 :1;u16 B4 :1;u16 B3 :1;u16 B2 :1;u16 B1 :1;u16 B0 :1;
} BITS15_0;
typedef struct st_bits7_0 {
u08 B7 :1;u08 B6 :1;u08 B5 :1;u08 B4 :1;u08 B3 :1;u08 B2 :1;u08 B1 :1;u08 B0 :1;
} BITS7_0;

	//Little-Endian: byte=b0..b7, word=b0..b15, long=b0..b31
typedef struct st_bits0_7 {
u08 B0 :1;u08 B1 :1;u08 B2 :1;u08 B3 :1;u08 B4 :1;u08 B5 :1;u08 B6 :1;u08 B7 :1;
} BITS0_7;
typedef struct st_bits0_15 {
u16 B0 :1;u16 B1 :1;u16 B2 :1;u16 B3 :1;u16 B4 :1;u16 B5 :1;u16 B6 :1;u16 B7 :1;
u16 B8 :1;u16 B9 :1;u16 B10:1;u16 B11:1;u16 B12:1;u16 B13:1;u16 B14:1;u16 B15:1;
} BITS0_15;
typedef struct st_bits0_31 {
u32 B0 :1;u32 B1 :1;u32 B2 :1;u32 B3 :1;u32 B4 :1;u32 B5 :1;u32 B6 :1;u32 B7 :1;
u32 B8 :1;u32 B9 :1;u32 B10:1;u32 B11:1;u32 B12:1;u32 B13:1;u32 B14:1;u32 B15:1;
u32 B16:1;u32 B17:1;u32 B18:1;u32 B19:1;u32 B20:1;u32 B21:1;u32 B22:1;u32 B23:1;
u32 B24:1;u32 B25:1;u32 B26:1;u32 B27:1;u32 B28:1;u32 B29:1;u32 B30:1;u32 B31:1;
} BITS0_31;

	/* ---------------------------------------------------------
	prototype
	--------------------------------------------------------- */
void mcu_clock_deinit(void);
void LOCO_240kHz(void);//at power-on or after mcu_clock_deinit()!
void HOCO_16MHz(void);
void HOCO_PLL_120MHz(void);

#endif/* RX65x_H_ */
