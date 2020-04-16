#ifndef RX65x_H_
#define RX65x_H_
/* rx65x.h 
   ------------------------------------------ TAB-size 4, code page UTF-8 --
hardware manual (HW):
	https://www.renesas.com/eu/en/doc/products/mpumcu/doc/rx_family/001/r01uh0590ej0230-rx651.pdf

Wijzigingen:
	RvL 10-4-2020	+ atom()
	RvL  5-4-2020	+ TMR, CMT, CMTW, SCIg, SCIh, SCIi, IPR_NR()
	RvL  4-4-2020	+ TPUa
	RvL  3-4-2020	+ bitstructs + Endian + module stop bits
	RvL 29-3-2020	opname in lib/src
	RvL 23-3-2020	aanmaak, zie ../../doc/nl/header.pdf
------------------------------------------------------------------------- */
#define NOP()	__asm("nop")//asm not optimized out?
#define sei() __builtin_rx_setpsw('I')
#define cli() __builtin_rx_clrpsw('I')
#define isNoInterrupts() (!(__builtin_rx_mvfc(0x0) && 0x00010000))

static __inline__ int cli_ret_1( void ) {return cli(), 1;}
#define pushi_do_()	int di_=isNoInterrupts(), do_=cli_ret_1()
#define undo_popi()	do_=0, di_ ? do_=0 : sei()
#define atom()\
	for(pushi_do_(); do_; undo_popi() )

	//get-macro (C: constant/variable):
#define BF_XTR_(c,p,sz) ((c)>> (p) & BIT_(sz)-1)//RxV3: BFMOV, !dlsb
#define BIT_(C)         (1ULL<< (C) )
#define BITGET_(C,B)    ((C) & BIT_(B) )//BITGET_(~0,3) == 8
#define BITVAL_(C,B)    ((C)>> (B) & 1) //BITVAL_(~0,3) == 1
#define ELEMENTS_(_C)	(sizeof(_C)/sizeof(*_C) )
#define GETLSB1(C)      ((C) & -(C) )		//-C == ~C+1
#define GET_(C,M)       ((C) & (M) )
#define MASK_(P,SZ)     (BIT_(SZ)-1<<(P) )
#define MAXINDEX_(_C)   (ELEMENTS_(_C)-1)
#define XOR_(C,B)       ((C) ^ (B) )

	//set-macro (v: variable):
#define bf_ins_(v,p,sz,n)   ((v)^= ((n)&(BIT_(sz)-1))<<(p) )//RxV3: BFMOV, !slsb
#define bitclr_(v,b)        ((v)&=~BIT_(b) )
#define bitinv_(v,b)        ((v)^= BIT_(b) )
#define bitset_(v,b)        ((v)|= BIT_(b) )
#define vclr_(v,m)          ((v)&=~(m) )
#define vclrset_(v,m,n)     ((v) = (v) & ~(m)|(n) )
#define vinsert_(v,m,n)     ((v)^= (m) & XOR(v,n) )
#define vlsbclr_(v)         ((v)&= (v)-1)
#define vset_(v,m)          ((v)|= (m) )
#define vxor_(v,m)          ((v)^= (m) )
#define xmax(x,M)           if(x>(M) )	x=(M)
#define xmin(x,m)           if(x<(m) )	x=(m)
#define xlim(x,m,M)         do{xmin(x,m); else xmax(x,M) } while(0)

#define v(pos, sz, ...)     u08 _ ## pos[0x ## sz]	//void (positioned) array

#define deca(i, ...) (10  * (i) )
#define deca1(i, m ) (10  *((i)+(m)%10  *1) )
#define hecto(i,...) (100 * (i) )
#define hecto1(i,m ) (100 *((i)+(m)%10 *10) )
#define hecto2(i,m ) (100 *((i)+(m)%100 *1) )
#define kilo(i, ...) (1000* (i) )
#define kilo1(i, m ) (1000*((i)+(m)%10*100) )
#define kilo2(i, m ) (1000*((i)+(m)%100*10) )
#define kilo3(i, m ) (1000*((i)+(m)%1000*1) )

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
	//HW 2.6.1 Exception Vector Table: 124B
	//HW 2.6.2 Interrupt Vector Table: 1024B


	//HW 5. I/O-Registers, p. 187
	//  (2) Notes on writing to I/O registers:
#define IOREG_WRITE(r,v)	do{(r)=(v); if((r)!=(v)) NOP();} while(0)
#define WRITE_CHK(r,v)		do (r)=(v); while( (r)!=(v) )


	//HW 7. Option-Setting Memory (OFSM)
#define OFSM_ (*(struct {u32 MDE,OFS0,OFS1,_0c, TMINF,_14[3], BANKSEL,_24[7],\
/*0x40*/SPCC,_44,TMEF,_4c, OSIS,_54[4],FAW,_68[2], ROMCODE,_74[2],TSCDR,\
/*0x80*/_80[4], UIDR0,UIDR1,UIDR2,UIDR3;\
} *const)0xFE7F5D00)

	
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

#define MSTP_EXDMAC_  U32_BIT(SYSTEM_.MSTPCRA).b29
#define MSTP_DMADTC_  U32_BIT(SYSTEM_.MSTPCRA).b28
#define MSTP_DA_      U32_BIT(SYSTEM_.MSTPCRA).b19
#define MSTP_S12AD_   U32_BIT(SYSTEM_.MSTPCRA).b17
#define MSTP_S12AD1_  U32_BIT(SYSTEM_.MSTPCRA).b16
#define MSTP_CMT01_   U32_BIT(SYSTEM_.MSTPCRA).b15
#define MSTP_CMT23_   U32_BIT(SYSTEM_.MSTPCRA).b14
#define MSTP_TPU_     U32_BIT(SYSTEM_.MSTPCRA).b13
#define MSTP_PPG0_    U32_BIT(SYSTEM_.MSTPCRA).b11
#define MSTP_PPG1_    U32_BIT(SYSTEM_.MSTPCRA).b10
#define MSTP_MTU_     U32_BIT(SYSTEM_.MSTPCRA).b9
#define MSTP_TMR01_   U32_BIT(SYSTEM_.MSTPCRA).b5
#define MSTP_TMR23_   U32_BIT(SYSTEM_.MSTPCRA).b4
#define MSTP_CMTW0_   U32_BIT(SYSTEM_.MSTPCRA).b1
#define MSTP_CMTW1_   U32_BIT(SYSTEM_.MSTPCRA).b0

#define MSTP_SCI0_    U32_BIT(SYSTEM_.MSTPCRB).b31
#define MSTP_SCI1_    U32_BIT(SYSTEM_.MSTPCRB).b30
#define MSTP_SCI2_    U32_BIT(SYSTEM_.MSTPCRB).b29
#define MSTP_SCI3_    U32_BIT(SYSTEM_.MSTPCRB).b28
#define MSTP_SCI4_    U32_BIT(SYSTEM_.MSTPCRB).b27
#define MSTP_SCI5_    U32_BIT(SYSTEM_.MSTPCRB).b26
#define MSTP_SCI6_    U32_BIT(SYSTEM_.MSTPCRB).b25
#define MSTP_SCI7_    U32_BIT(SYSTEM_.MSTPCRB).b24
#define MSTP_CRC_     U32_BIT(SYSTEM_.MSTPCRB).b23
#define MSTP_PDC_     U32_BIT(SYSTEM_.MSTPCRB).b22
#define MSTP_RIIC0_   U32_BIT(SYSTEM_.MSTPCRB).b21
#define MSTP_RIIC1_   U32_BIT(SYSTEM_.MSTPCRB).b20
#define MSTP_USB0_    U32_BIT(SYSTEM_.MSTPCRB).b19
#define MSTP_RSPI0_   U32_BIT(SYSTEM_.MSTPCRB).b17
#define MSTP_RSPI1_   U32_BIT(SYSTEM_.MSTPCRB).b16
#define MSTP_ETHERC0_ U32_BIT(SYSTEM_.MSTPCRB).b15
#define MSTP_ELC_     U32_BIT(SYSTEM_.MSTPCRB).b9
#define MSTP_TEMPS_   U32_BIT(SYSTEM_.MSTPCRB).b8
#define MSTP_DOC_     U32_BIT(SYSTEM_.MSTPCRB).b6
#define MSTP_SCI12_   U32_BIT(SYSTEM_.MSTPCRB).b4
#define MSTP_CAN1_    U32_BIT(SYSTEM_.MSTPCRB).b1
#define MSTP_CAN0_    U32_BIT(SYSTEM_.MSTPCRB).b0

#define MSTP_GLCDC_   U32_BIT(SYSTEM_.MSTPCRC).b29
#define MSTP_DRW2D_   U32_BIT(SYSTEM_.MSTPCRC).b28
#define MSTP_SCI8_    U32_BIT(SYSTEM_.MSTPCRC).b27
#define MSTP_SCI9_    U32_BIT(SYSTEM_.MSTPCRC).b26
#define MSTP_SCI10_   U32_BIT(SYSTEM_.MSTPCRC).b25
#define MSTP_SCI11_   U32_BIT(SYSTEM_.MSTPCRC).b24
#define MSTP_QSPI_    U32_BIT(SYSTEM_.MSTPCRC).b23
#define MSTP_RSPI2_   U32_BIT(SYSTEM_.MSTPCRC).b22
#define MSTP_CAC_     U32_BIT(SYSTEM_.MSTPCRC).b19
#define MSTP_RIIC2_   U32_BIT(SYSTEM_.MSTPCRC).b17
#define MSTP_STBYRAM_ U32_BIT(SYSTEM_.MSTPCRC).b7
#define MSTP_RAM2_    U32_BIT(SYSTEM_.MSTPCRC).b2
#define MSTP_RAM0_    U32_BIT(SYSTEM_.MSTPCRC).b0

#define MSTP_TSIP_    U32_BIT(SYSTEM_.MSTPCRD).b27
#define MSTP_MMCIF_   U32_BIT(SYSTEM_.MSTPCRD).b21
#define MSTP_SDHI_    U32_BIT(SYSTEM_.MSTPCRD).b19
#define MSTP_SDSI_    U32_BIT(SYSTEM_.MSTPCRD).b13

	//HW 15. Interrupt Controller (ICUB)
	/* ---------------------------------------------------------
	p.456: Table 15.3 Interrupt Sources for Software Configurable Interrupt B 
	p.460: Table 15.4 Interrupt Sources for Software Configurable Interrupt A 
	p.465: Table 15.5 Interrupt Vector Table
	p.474: Table 15.7 Group Interrupt Requests
	for vector nr x:
		ICU_.IPR[IPR_NR(x)]=prio;  //set interrupt prio
		ICU_.IR[x]=0;              //clear interrupt
		bitset_(ICU_.IER[x/8],x%8);//enable interupt
	--------------------------------------------------------- */
enum en_irq_edge {eIRQ_LOW, eIRQ_FALL, eIRQ_RISE, eIRQ_BOTH};
	//VEC->IPR: 16 18->0 21->1 23->2 26->3 28..31->4..7 32+->32+
#define IPR_NR(vec) ((vec)>=32?(vec):(vec)<28?((vec)-16)/3:(vec)-24)
#define intvec_prio_enable(v, p) do{ICU_.IR[v]=0;  ICU_.IPR[IPR_NR(v)]=(p);\
	bitset_(ICU_.IER[(v)/8],(v)%8);\
} while(0)
#define ICU_ (*(volatile struct {\
/*7000*/u08 IR[256];	/*16..255, p.465*/\
/*7100*/u08 DTCER[256];	/*26..255, p.465*/\
/*7200*/u08 IER[32];	/* 2..31,  p.465*/ v(7220,c0);\
/*72e0*/u08 SWINTR,SWINT2R; v(7222,e);\
/*72f0*/u16 FIR; v(72f2,e);\
/*7300*/u08 IPR[256];	/*0..7,34..255, p.465*/\
/*7400*/u08 DMRSR0; v(7401,3); u08 DMRSR1; v(7405,3);\
/*7408*/u08 DMRSR2; v(7409,3); u08 DMRSR3; v(740d,3);\
/*7410*/u08 DMRSR4; v(7411,3); u08 DMRSR5; v(7415,3);\
/*7418*/u08 DMRSR6; v(7419,3); u08 DMRSR7; v(741d,e3);\
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


	//HW 16. Buses
	//HW 17. Memory-Protection Unit (MPU)
	//HW 18. DMA Controller (DMACAa)
	//HW 19. EXDMA Controller (EXDMACa)
	//HW 20. Data Transfer Controller (DTCb)
	//HW 21. Event Link Controller (ELC)


	//HW 22. I/O Ports
	//HW 23. Multi-Function Pin Controller (MPC)
	/* ---------------------------------------------------------
	Problems in accessing port register (bit):
	- interleaved ODR0 and ODR1
	- non-existent PORTI
	--------------------------------------------------------- */
struct st_io
{/*0x00*/u08 _PDR[32],_PODR[32],_PIDR[32],_PMR[32],_ODR[32][2];/*interleaved*/
 /*0xc0*/u08 _PCR[32],_DSCR[32],_offset_100h_[0x28],_DSCR2[32];
};
#define _PORT(n) ((volatile struct st_io *const)(0x0008c000+(n)))
#define CH2P_(c)/* character-to-port_number */\
	((c)<='9' ? (c)-'0' : ('i'<=((c)|0x20) ? 9 : 10) + ((c)|0x20)-'a')
	//register (bit) access:
#define ODR0(n)           PORTREG(n,ODR)[0]
#define ODR1(n)           PORTREG(n,ODR)[1]
#define PORTREG(n,reg)    (_PORT(0)->_## reg[n])
#define ODR0BIT(n,bn)     (U08_BIT(ODR0(n)).b ## bn)
#define ODR1BIT(n,bn)     (U08_BIT(ODR1(n)).b ## bn)
#define PORTBIT(n,reg,bn) (U08_BIT(PORTREG(n,reg)).b ## bn)

#define IO_ (*_PORT(0))
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

#define MPC_ (*(volatile struct\
{	u08 PFCSE,_01,PFCSS0,PFCSS1,PFAOE0,PFAOE1,PFBCR0,PFBCR1,PFBCR2,PFBCR3,\
	_0a[4],PFENET,_0f[0x10],PWPR,_20[0x20],_PFS[19*8];\
} *const)0x0008C100)
#define PmnPFS(port,pin) (MPC_._PFS[8*(port)+(pin)])	
#define PFS_unlock() do{MPC_.PWPR&=~0x80;MPC_.PWPR|=0x40;}while(0)
#define PFS_relock() do{MPC_.PWPR&=~0x40;MPC_.PWPR|=0x80;}while(0)


	//HW 24. Multi-Function Timer Pulse Unit 3 (MTU3a)
	/* ---------------------------------------------------------
	HW Table 15.4, p.460: SLIA-sources
		 1- 7 =TGIA0 TGIB0 TGIC0 TGID0 TGIV0 TGIE0 TGIF0
		 8-11 =TGIA1 TGIB1 TGIV1 TGIU1
		12-15 =TGIA2 TGIB2 TGIV2 TGIU2
		16-20 =TGIA3 TGIB3 TGIC3 TGID3 TGIV3
		21-25 =TGIA4 TGIB4 TGIC4 TGID4 TGIV4
		27-29 =TGIU5 TGIV5 TGIW5
		30-34 =TGIA6 TGIB6 TGIC6 TGID6 TGIV6
		35-39 =TGIA7 TGIB7 TGIC7 TGID7 TGIV7
		41-45 =TGIA8 TGIB8 TGIC8 TGID8 TGIV8
	--------------------------------------------------------- */
	//general:
enum en_mtu_edge {eMTU_RISE, eMTU_FALL, eMTU_BOTH};
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

#define MTU_CST0	U08_BIT(MTU_.TSTRA).b0
#define MTU_CST1	U08_BIT(MTU_.TSTRA).b1
#define MTU_CST2	U08_BIT(MTU_.TSTRA).b2
#define MTU_CST3	U08_BIT(MTU_.TSTRA).b6
#define MTU_CST4	U08_BIT(MTU_.TSTRA).b7
#define MTU_CST6	U08_BIT(MTU_.TSTRB).b6
#define MTU_CST7	U08_BIT(MTU_.TSTRB).b7
#define MTU_CST8	U08_BIT(MTU_.TSTRA).b3
#define MTU_CSTU5	U08_BIT(MTU_.TSTR).b2
#define MTU_CSTV5	U08_BIT(MTU_.TSTR).b1
#define MTU_CSTW5	U08_BIT(MTU_.TSTR).b0

	/* ---------------------------------------------------------
	mtu0: TIORH TIORL, no TSR
	mtu1: TIOR, no TIORL
	mtu2: TIOR, no TIORL
	mtu8: TIORH TIORL, no TSR
	--------------------------------------------------------- */
struct stMTU/*common to mtu0..2*/
{	u08 TCR,TMDR1; union {u08 TIOR; u08 TIORH;}; u08 TIORL,TIER,TSR;
	u16 TCNT,TGRA,TGRB;
};/*=>0x0c*/
#define MTU0_ (*(volatile struct {u08 NFCR0,_1291[8],NFCRC,_129a[0x66];\
/*1300*/struct stMTU /*anonymous*/; u16 TGRC,TGRD;\
/*1310*/u08 _1310[16]; u16 TGRE,TGRF;u08 TIER2,_1325,TBTM,_1327,TCR2;\
} *const)0x000c1290)
#define MTU1_ (*(volatile struct {u08 _1290,NFCR1,_1292[0xee];\
/*1380*/struct stMTU /*anonymous*/; u16 _0c, _0e;\
/*1390*/u08 TICCR,TMDR3,_12,_13,TCR2,_15[0x0b]; u32 TCNTLW,TGRALW,TGRBLW;\
} *const)0x000c1290)
#define MTU2_ (*(volatile struct {u08 _1290,_1291,NFCR2,_1293[0x16d];\
/*1400*/struct stMTU /*anonymous*/; u08 TCR2;\
} *const)0x000c1290)

	//interleaved: mtu3/4 mtu6/7
struct stMTU36 {u08 TCR,_01,TMDR1,_03,TIORH,TIORL,_06,_07,TIER,_09,_0a[6];\
/*0x10*/u16 TCNT,_12,_14,_16,TGRA,TGRB,_1c,_1e,_20,_22,TGRC,TGRD,_28,_2a;\
/*0x2c*/u08 TSR,_2d,_2e,_2f,_30[8],TBTM,_39,_3a[0x12],TCR2,_4d[3];	};
#define MTU3_ (*(volatile struct {struct stMTU36 /*anonymous*/;\
u08 _50,  _51[0x1f]; u16 _70,TGRE,_74,_76; u08 _77[0x1c]; u08 NFCR3;\
} *const)0x000c1200)
#define MTU6_ (*(volatile struct {struct stMTU36 /*anonymous*/;\
u08 TSYCR,_51[0x1f]; u16 _70,TGRE,_74,_76; u08 _77[0x1c]; u08 NFCR6;\
} *const)0x000c1a00)

struct stMTU47 {u08 _00,TCR,_02,TMDR1,_04,_05,TIORH,TIORL,_08,TIER,_0a[6];\
/*0x10*/u16 _10,TCNT,_14,_16,_18,_1a,TGRA,TGRB,_20,_22,_24,_26,TGRC,TGRD;\
/*0x2c*/u08 _2c,TSR,_2e,_2f,_30[8],_38,TBTM,_3a[6];\
/*0x40*/u16 TADCR,_42,TADCORA,TADCORB,TADCOBRA,TADCOBRB;u08 _4c,TCR2,_4e[0x22];\
/*0x70*/u16 _70,_72,TGRE,TGRF; u08 _77[0x1c],_93;	};
#define MTU4_ (*(volatile struct {struct stMTU47 /*anonymous*/; u08 NFCR4;\
} *const)0x000c1200)
#define MTU7_ (*(volatile struct {struct stMTU47 /*anonymous*/; u08 NFCR7;\
} *const)0x000c1a00)

	//mtu5: input capture
#define MTU5_ (*(volatile struct{u08 _1A94,NCFR5,_1A96[0X1EA];\
/*1c80*/u16 TCNTU,TGRU; u08 TCRU,TCR2U,TIORU,_1C87[9];\
/*1c90*/u16 TCNTV,TGRV; u08 TCRV,TCR2V,TIORV,_1C97[9];\
/*1ca0*/u16 TCNTW,TGRW; u08 TCRW,TCR2W,TIORW,_1CA7[9];\
/*1cb0*/u08 _1CB0,_1CB1,TIER,_1CB3,TSTR,_1CB5,TCNTCMPCLR;\
} *const)0x000C1A94)

	//mtu8: 32-bit
#define MTU8_ (*(volatile struct{u08 _1290[8],NFCR8,_1299[0x367];\
/*1600*/u08 TCR,TMDR1,TIORH,TIORL,TIER,_05,TCR2,_07;\
/*1608*/u32 TCNT,TGRA,TGRB,TGRC,TGRD;\
} *const)0x000c1290)


	//HW 25. Port Output Enable 3 (POE3a)


	//HW 26. 16-Bit Timer Pulse Unit (TPUa)
	/* ---------------------------------------------------------
	HW Table 15.3, p.456: SLIB-sources
		15-19 = TGI0A TGI0B TGI0C TGI0D TCI0V
		20-23 = TGI1A TGI1B TCI1V TCI1U
		24-27 = TGI2A TGI2B TCI2V TCI2U
		28-32 = TGI3A TGI3B TGI3C TGI3D TCI3V
		33-36 = TGI4A TGI4B TCI4V TCI4U
		37-40 = TGI5A TGI5B TCI5V TCI5U
	--------------------------------------------------------- */
enum en_tpu_edge {eTPU_FALL, eTPU_RISE, eTPU_BOTH};
struct stTPU/*common to tpu*/
{	u08 TCR, TMDR; union {u08 TIOR; u08 TIORH;}; u08 TIORL,TIER,TSR;
	u16 TCNT,TGRA,TGRB,TGRC,TGRD;
};/*=>0x0c*/
#define TPUA_ (*(volatile struct {u08 TSTR,TSYR,_02[6],NFCR[6],_0e,_0f;\
/*8110*/struct stTPU TPU[6];\
} *const)0x00088100)
#define TPU0_ TPUA_.TPU[0]	//TIORH TIORL TGRA TGRB TGRC TGRD
#define TPU1_ TPUA_.TPU[1]	//TIOR  -     TGRA TGRB -    -
#define TPU2_ TPUA_.TPU[2]	//TIOR  -     TGRA TGRB -    -
#define TPU3_ TPUA_.TPU[3]	//TIORH TIORL TGRA TGRB TGRC TGRD
#define TPU4_ TPUA_.TPU[4]	//TIOR  -     TGRA TGRB -    -
#define TPU5_ TPUA_.TPU[5]	//TIOR  -     TGRA TGRB -    -
	//counter start bits
#define TPU_CST0	U08_BIT(TPUA_.TSTR).b0
#define TPU_CST1	U08_BIT(TPUA_.TSTR).b1
#define TPU_CST2	U08_BIT(TPUA_.TSTR).b2
#define TPU_CST3	U08_BIT(TPUA_.TSTR).b3
#define TPU_CST4	U08_BIT(TPUA_.TSTR).b4
#define TPU_CST5	U08_BIT(TPUA_.TSTR).b5


	//HW 27. Programmable Pulse Generator (PPG)


	//HW 28. 8-Bit Timer (TMR)	interleaved: tmr0/tmr1, tmr2/tmr3
	/* ---------------------------------------------------------
	HW Table 15.3, p.456: SLIB-sources
		 3- 5 = CMIA0 CMIB0 OVI0
		 6- 8 = CMIA1 CMIB1 OVI1
		 9-11 = CMIA2 CMIB2 OVI2
		12-14 = CMIA3 CMIB3 OVI3
	--------------------------------------------------------- */
struct stTMR0
{u08 TCR,_01,TCSR,_03,TCORA,_05,TCORB,_07,TCNT,_09,TCCR,_0b,TCSTR,_0d;};
struct stTMR1
{u08 _00,TCR,_02,TCSR,_04,TCORA,_06,TCORB,_08,TCNT,_0a,TCCR,_0c,TCSTR;};
#define TMR0_ (*(volatile struct stTMR0 *const)0x00088200)
#define TMR1_ (*(volatile struct stTMR1 *const)0x00088200)
#define TMR2_ (*(volatile struct stTMR0 *const)0x00088210)
#define TMR3_ (*(volatile struct stTMR1 *const)0x00088210)
struct stTMR01 { u16 _00,_02,TCORA,TCORB,TCNT,TCCR; };//16-bit access
#define TMR01_ (*(volatile struct stTMR01 *const)0x00088200)
#define TMR23_ (*(volatile struct stTMR01 *const)0x00088210)


	//HW 29. Compare Match Timer (CMT)
	/* ---------------------------------------------------------
	HW Table 15.5, p.465: interrupt vector table
		28-29 = CMI0 CMI1
	HW Table 15.3, p.456: SLIB-sources
		 1- 2 = CMI2 CMI3
	--------------------------------------------------------- */
struct st_CMTch {u16 CMCR,CMCNT,CMCOR;};
#define CMT_ (*(volatile struct {u16 CMSTR0,_02[3],_08[3],_0e;\
u16 CMSTR1,_12[3],_18[3];} *const)0x00088000)
#define CMT0_ (*(struct st_CMTch *)(CMT_._02))
#define CMT1_ (*(struct st_CMTch *)(CMT_._08))
#define CMT2_ (*(struct st_CMTch *)(CMT_._12))
#define CMT3_ (*(struct st_CMTch *)(CMT_._18))
#define CMT_CST0	U16_BIT(CMT_.CMSTR0).b0
#define CMT_CST1	U16_BIT(CMT_.CMSTR0).b1
#define CMT_CST2	U16_BIT(CMT_.CMSTR1).b0
#define CMT_CST3	U16_BIT(CMT_.CMSTR1).b1


	//HW 30. Compare Match Timer W (CMTW)
	/* ---------------------------------------------------------
	HW Table 15.5, p.465: interrupt vector table
		30-31 = CMWI0 CMWI1
	HW Table 15.3, p.457: SLIB-sources
		41-44 = IC0I0 IC1I0 OC0I0 OC1I0
		45-48 = IC0I1 IC1I1 OC0I1 OC1I1
	--------------------------------------------------------- */
struct st_CMTW_ 
{u16 CMWSTR,_02,CMWCR,_06,CMWIOR,_0a,_0c,_0e;
 u32 CMWICR0,CMWICR1,CMWOCR0,CMWOCR1;	};
#define CMTW0_ (*(volatile struct st_CMTW_ *const)0x00094200)
#define CMTW1_ (*(volatile struct st_CMTW_ *const)0x00094280)


	//HW 31. Realtime Clock (RTCd)
	/* ---------------------------------------------------------
	HW Table 15.5, p.465: interrupt vector table
		92-93 = ALM PRD
	HW Table 15.3, p.457: SLIB-sources
		49    = CUP
	--------------------------------------------------------- */
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

	//HW 37. Serial Communications Interface (SCIg, SCIi, SCIh)
	/* ---------------------------------------------------------
	sci_interrupts:
	HW Table 15.5 Interrupt Vector Table, p.465+:
	HW Table 15.7 Group Interrupt Requests, p.474+:
		 group vector 110 111 112 113 = BL0 BL1 AL0 AL1, each 32 entries
	vector v, v+1 = SCIyRXIy, SCIy_TXIy		communication interrupt
	 group g, g+1 = SCIyTEIy, SCIy_ERIy		error interrupt
	combine group g and request r => 32*g + r < 128
	--------------------------------------------------------- */
struct st_sci_interrupt {u08 vector, group;};//see comment above
struct st_SMC {	u08 SMR,_01,SCR,_03,SSR,_05,SCMR;	};//smart card
typedef union {struct st_SMC smc; struct {//SCIg
	u08 SMR,BRR,SCR,TDR,SSR,RDR,SCMR,SEMR,SNFR,SIMR1,SIMR2,SIMR3,SISR,SPMR;
/*0x0e*/union {struct{u08 TDRH,TDRL;}; u16 TRDHL;};
/*0x10*/union {struct{u08 RDRH,RDRL;}; u16 RRDHL;}; u08 MDDR;/*=>0x13*/};
} uSCIg;
typedef union {struct st_SMC smc; struct {//SCIh
	u08 SMR,BRR,SCR,TDR,SSR,RDR,SCMR,SEMR,SNFR,SIMR1,SIMR2,SIMR3,SISR,SPMR;
/*0x0e*/union {struct{u08 TDRH,TDRL;}; u16 TRDHL;};
/*0x10*/union {struct{u08 RDRH,RDRL;}; u16 RRDHL;}; u08 MDDR, _13[0x0d];
/*0x20*/u08 ESMER,CR0,CR1,CR2,CR3,PCR,ICR,STR,STCR,CF0DR,CF0CR,CF0RR;
/*0x2c*/u08 PCF1DR,SCF1DR,CF1CR,CF1RR,TCR,TMR,TPRE,TCNT;	};
} uSCIh;
typedef union {u16 WORD; struct {u08 H,L;};} UHL;
typedef union {struct st_SMC smc; struct {//SCIi
	u08 SMR,BRR,SCR,TDR,SSR,RDR,SCMR,SEMR,SNFR,SIMR1,SIMR2,SIMR3,SISR,SPMR;
/*0x0e*/union {struct{u08 TDRH,TDRL;}; u16 TRDHL;};
/*0x10*/union {struct{u08 RDRH,RDRL;}; u16 RRDHL;}; u08 MDDR, DCCR;};
struct {u08 _0[4],SSRFIFO,_5[0x9]; UHL FTDR,FRDR,_12,FCR,FDR,LSR,CDR;u08 SPTR;};
} uSCIi;
	//sci0..9: SCIg, PCLKB, sci5..6 TMR clock input, sci5 ELC-function
#define SCI0_ (*(volatile uSCIg *const)0x0008a000)
#define SCI1_ (*(volatile uSCIg *const)0x0008a020)
#define SCI2_ (*(volatile uSCIg *const)0x0008a040)
#define SCI3_ (*(volatile uSCIg *const)0x0008a060)
#define SCI4_ (*(volatile uSCIg *const)0x0008a080)
#define SCI5_ (*(volatile uSCIg *const)0x0008a0a0)
#define SCI6_ (*(volatile uSCIg *const)0x0008a0c0)
#define SCI7_ (*(volatile uSCIg *const)0x0008a0e0)
#define SCI8_ (*(volatile uSCIg *const)0x0008a100)
#define SCI9_ (*(volatile uSCIg *const)0x0008a120)
	//sci10..11: SCIi, PCLKA, fifo + data match
#define SCI10_ (*(volatile uSCIi *const)0x000d0040)
#define SCI11_ (*(volatile uSCIi *const)0x000d0060)
	//sci12: SCIh, PCLKB, extended serial mode: start/information frame
#define SCI12_ (*(volatile uSCIh *const)0x0008b300)


	/* ---------------------------------------------------------
	typedef
	--------------------------------------------------------- */

	//Big-Endian: long=b31..b0, word=b15..b0, byte=b7..b0
typedef struct st_bits31_24 {
u32 b31:1;u32 b30:1;u32 b29:1;u32 b28:1;u32 b27:1;u32 b26:1;u32 b25:1;u32 b24:1;
u32 b23:1;u32 b22:1;u32 b21:1;u32 b20:1;u32 b19:1;u32 b18:1;u32 b17:1;u32 b16:1;
u32 b15:1;u32 b14:1;u32 b13:1;u32 b12:1;u32 b11:1;u32 b10:1;u32 b9 :1;u32 b8 :1;
u32 b7 :1;u32 b6 :1;u32 b5 :1;u32 b4 :1;u32 b3 :1;u32 b2 :1;u32 b1 :1;u32 b0 :1;
} BITS31_0;
typedef struct st_bits15_0 {
u16 b15:1;u16 b14:1;u16 b13:1;u16 b12:1;u16 b11:1;u16 b10:1;u16 b9 :1;u16 b8 :1;
u16 b7 :1;u16 b6 :1;u16 b5 :1;u16 b4 :1;u16 b3 :1;u16 b2 :1;u16 b1 :1;u16 b0 :1;
} BITS15_0;
typedef struct st_bits7_0 {
u08 b7 :1;u08 b6 :1;u08 b5 :1;u08 b4 :1;u08 b3 :1;u08 b2 :1;u08 b1 :1;u08 b0 :1;
} BITS7_0;

	//Little-Endian: byte=b0..b7, word=b0..b15, long=b0..b31
typedef struct st_bits0_7 {
u08 b0 :1;u08 b1 :1;u08 b2 :1;u08 b3 :1;u08 b4 :1;u08 b5 :1;u08 b6 :1;u08 b7 :1;
} BITS0_7;
typedef struct st_bits0_15 {
u16 b0 :1;u16 b1 :1;u16 b2 :1;u16 b3 :1;u16 b4 :1;u16 b5 :1;u16 b6 :1;u16 b7 :1;
u16 b8 :1;u16 b9 :1;u16 b10:1;u16 b11:1;u16 b12:1;u16 b13:1;u16 b14:1;u16 b15:1;
} BITS0_15;
typedef struct st_bits0_31 {
u32 b0 :1;u32 b1 :1;u32 b2 :1;u32 b3 :1;u32 b4 :1;u32 b5 :1;u32 b6 :1;u32 b7 :1;
u32 b8 :1;u32 b9 :1;u32 b10:1;u32 b11:1;u32 b12:1;u32 b13:1;u32 b14:1;u32 b15:1;
u32 b16:1;u32 b17:1;u32 b18:1;u32 b19:1;u32 b20:1;u32 b21:1;u32 b22:1;u32 b23:1;
u32 b24:1;u32 b25:1;u32 b26:1;u32 b27:1;u32 b28:1;u32 b29:1;u32 b30:1;u32 b31:1;
} BITS0_31;

	/* ---------------------------------------------------------
	prototype
	--------------------------------------------------------- */
void mcu_clock_deinit(void);
void LOCO_240kHz(void);//at power-on or after mcu_clock_deinit()!
void HOCO_16MHz(void);
void HOCO_PLL_120MHz(void);

	//1875 kHz ocunt, 1kHz interrupt:
void CMT0_init(void);
void CMT0_start(void);
//void INT_Excep_CMT0_CMI0(void) {Micros+=1000, ++Millis;}

	//1875 kHz ocunt, 1kHz interrupt:
void TMR01_init(void);
void TMR01_start(void);
//void INT_Excep_PERIB_INTB146(void) {Micros+=1000, ++Millis;}

#endif/* RX65x_H_ */
