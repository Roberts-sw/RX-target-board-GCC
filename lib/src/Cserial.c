/* Cserial.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

changes:
	RvL  11-4-2020	concept
------------------------------------------------------------------------- */
#include "Cserial.h"
#define SCI_BUFSIZE	512

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
#define CALL(f) Cserial_ ## f (self)
#define CALL1(f, b) Cserial_ ## f (self, b)
#define OBJ(f) Cserial_ ## f (Shwserial *self)
#define OBJ1(f,b) Cserial_ ## f (Shwserial *self, b)

#define R SMR	//HW 37.2.9 Serial Mode Register (SMR)
regb_(	b_(7,CM)		b_(6,CHR)		b_(5,PE)		b_(4,PM)
		b_(3,STOP)		b_(2,MP)		b_(1,CKS1)		b_(0,CKS0)		)
#undef R

#define R SCR	//HW 37.2.10 Serial Control Register (SCR)
regb_(	b_(7,TIE)		b_(6,RIE)		b_(5,TE)		b_(4,RE)
		b_(3,MPIE)		b_(2,TEIE)		b_(1,CKE1)		b_(0,CKE0)		)
#undef R

#define R SSR	//HW 37.2.11 Serial Status Register (SSR/SSRFIFO)
regb_(	b_(7,TDRE)		b_(6,RDRF)		b_(5,ORER)		b_(4,FER)
		b_(3,PER)		b_(2,TEND)		b_(1,MPB)		b_(0,MPBT)		)
#undef R

#define R SCMR	//HW 37.2.12 Smart Card Mode Register (SCMR)
regb_(	b_(7,BCP2)										b_(4,CHR1)
		b_(3,SDIR)		b_(2,SINV)						b_(0,SMIF)		)
#undef R

#define R SEMR	//HW 37.2.15 Serial Extended Mode Register (SEMR)
regb_(	b_(7,RXDESEL)	b_(6,BGDM)		b_(5,NFEN)		b_(4,ABCS)
						b_(2,BRME)						b_(0,ACS0)		)
#undef R
#undef  regb_
#undef  b_

static struct st_sci_interrupt const intreg_table[] =
{	//HW 15.3.1 Interrupt Vector Table, vector: 2n+0=RXI-, 2n+1=TXI-
	//HW 15.4.4 Group Interrupts, groupbit: 2n+0=TEI-, 2n+1=ERI-
//{  v,g*32+ r},  {  v,g*32+ r},  {  v,g*32+ r},  {  v,g*32+ r}
  { 58,0*32+ 0},  { 60,0*32+ 2},  { 62,0*32+ 4},  { 80,0*32+ 6},
  { 82,0*32+ 8},  { 84,0*32+10},  { 86,0*32+12},  { 98,0*32+14},
  {100,1*32+24},  {102,1*32+26},  {104,2*32+ 8},  {114,2*32+12},
  {116,0*32+16},
};
static void rxi_prio (u08 sci_chan, u08 priority)
{	u08 vec=intreg_table[ sci_chan].vector&~1;//RXI-offset 0
	intvec_prio_enable(vec,priority);
}
static void txi_prio (u08 sci_chan, u08 priority)
{	u08 vec=intreg_table[ sci_chan].vector| 1;//TXI-offset 1
	intvec_prio_enable(vec,priority);
}
static void tei_prio (u08 sci_chan, u08 priority)
{	u08 vec=intreg_table[sci_chan].group&~1;//TEI_-groupoffset 0
	u08 source=vec%32;
	vec=vec/32+110;//GROUPBL0+...
	intvec_prio_enable(vec,priority);
	vec-=110;
	((volatile u32 *)(&(ICU_.GRPBL0)))[vec]&=~(1U<<source);
	((volatile u32 *)(&(ICU_.GENBL0)))[vec]|= (1U<<source);
}
static void eri_prio (u08 sci_chan, u08 priority)
{	u08 vec=intreg_table[sci_chan].group| 1;//ERI_-groupoffset 1
	u08 source=vec%32;
	vec=vec/32+110;//GROUPBL0+...
	intvec_prio_enable(vec,priority);
	vec-=110;
	((volatile u32 *)(&(ICU_.GRPBL0)))[vec]&=~(1U<<source);
	((volatile u32 *)(&(ICU_.GENBL0)))[vec]|= (1U<<source);
}

enum SCIMODE {SCI_UART, SCI_IIC,  SCI_SPI,  SCI_SMARTCARD, SCI_MODES};
enum PARMODE {PAR_NONE, PAR_ODD,  PAR_EVEN, PAR_MULTI};
u08 const *SCIMODE="UIPS";
u08 const *PARMODE="NOEM";
static u08 mode2num(u08 m)   {return m<=3 ? m : search4(SCIMODE, m&~0x20);}
static u08 parity2num(u08 p) {return p<=3 ? p : search4(PARMODE, p&~0x20);}
#define databits2num(d)	     ((d)&15)//7 8 9 '7' '8' '9' => 7 8 9
typedef struct st_baudset
{	u32 baud;       //(num)     char
	u08 mode;       //0 1 2 3 | (U) I P S | u i p s
	u08 parity;     //0 1 2 3 | (N) O E M | n o e m
	u08 databits;   //0 7 8 9 | (8) 7 8 9
	u08 stopbits;   //0 1 2   | (2) 1 2
} Sbaudset;
Sbaudset const UART_115k2N81 =//initstring: "U115k2N81"
{	.mode='U',	.baud=115200,	.parity='N',	.databits=8,	.stopbits=1	};

union un_pinsci
{	struct{eIOPIN txd,	rxd,  rts, cts; };//rts == drive_enable
	struct{eIOPIN mosi, miso, sck, ssel;};
	struct{eIOPIN sda,	scl;			};
};

struct st_hwserial
{	volatile uSCIg *_sci;	//HW-registers
	Sbaudset const *_set;	//init-settings
	union un_pinsci pin_;	//CHIP(X) => IO_MODE[], MPC_FUNC[], MSTP_ID[]
	u08 channel_;        	//nr 0..12 => intreg_table[] => (group) vectors

	volatile u16 rx_in, rx_out, tx_in, tx_out;
	volatile u08 rx_buf[SCI_BUFSIZE], tx_buf[SCI_BUFSIZE];
	volatile u08 init, sending;
};
Shwserial Serial5=
{	._sci=&SCI5_,	.channel_=5,	._set=&UART_115k2N81,
	.pin_={.txd=PA4, .rxd=PA3}
};


static u08 OBJ(uart_create)//rxd || txd, opt: rts, cts
{	//HW 37.3.7 SCI Initialization (Asynchronous Mode), p.1684
	eIOPIN pin; u08 id, n=0;
#define m_id(p)	id = n&&id!=MSTP_ID[p] ? MSTP_NO_ID : MSTP_ID[p], n=1
	if( iopin_init(pin=self->pin_.txd, IO_MODE[pin]) )	m_id(pin);
	if( iopin_init(pin=self->pin_.rxd, IO_MODE[pin]) )	m_id(pin);
#undef  m_id
	iopin_init(pin=self->pin_.rts, IO_MODE[pin]);
	iopin_init(pin=self->pin_.cts, IO_MODE[pin]);
	return id;
}

u08 const PARITY2SMR[4]=
{	0,//N
	b5_SMR_PE|b4_SMR_PM,//O
	b5_SMR_PE,//E
	b2_SMR_MP,//M
};
static u08 async_divisor (u32 baud, u32 *_ck)//n, clock pre_div:
{	//HW 37.2.9 Serial Mode Register (SMR)
	//Table 37.11 Relationship between N Setting in BRR Register and Bit Rate B
	//   N   =PCLK / {(16|32|64)*2**(2n-1)*B} - 1 <==>
	//B*(N+1)=PCLK/8/{(1|2|4)*2**2n)}, 1<=(N+1)<=256, 0<=n<=3
	u32 bd=baud*256; u08 n=0;
	if( (*_ck/=8) <= bd )   // B A n  div
	    return              // 1 1 0  8
	    	b6_SEMR_BGDM|b4_SEMR_ABCS;
	do  if(*_ck/4 <= bd )
	        break;
	while( *_ck/=4, ++n<3 );
	return(*_ck/2 <= bd )
	    ? n|b6_SEMR_BGDM    // 1 0 0+ 16 64  256 1024 => baud * 2
	    : n;                // 0 0 0+ 32 128 512 2048 => baud * 4
}
static u08 OBJ1(uart_init, u32 pclk)
{	//HW 37.3.7 SCI Initialization (Asynchronous Mode), p.1684
	//0. disable SCI (after releasing module from stop)
	//self->_sci->SCR=0x00;
	//1. IO-pins for TxD, RxD, etc.
	//2. SCR.CKE[1:0]-bits: use on-chip baud rate generator
	self->_sci->SCR=0x01;       //00
	//3. SIMR1-, SPMR-bits
	self->_sci->SIMR1=0x00;     //00 !IICM
	self->_sci->SPMR=0x00;      //00 !CKPOL !CKPHA
	//4. SMR, SCMR, SEMR data format + input clock pre-div
	u08 tmp=async_divisor(self->_set->baud, &pclk);//pclk=PCLK/{16*2**(2n-1)}
	u08 smr= (tmp&(b1_SMR_CKS1|b0_SMR_CKS0))
		+ (self->_set->stopbits&1 ? 0 : b3_SMR_STOP)
		+ PARITY2SMR[parity2num(self->_set->parity)];
	u08 data=databits2num(self->_set->databits);
	if(7==data)
		smr|=b6_SMR_CHR;
	self->_sci->SMR = smr;      //0??????? !CM ?CHR ?PE ?PM ?STOP ?MP ?CKS[1:0]
	self->_sci->SCMR= data<9
		? 0xf2 : 0xe2;			//111?0010 ?CHR1: not 9-bit data
	data=(b6_SEMR_BGDM|b4_SEMR_ABCS)
			& tmp;				//0?0?0-00 !RXDESEL ?BGDM !NFEN ?ABCS 0 0 !ACS0
	u32 baud=self->_set->baud;  // 1 1
	if(!(data & b6_SEMR_BGDM) )	baud<<=1;
	if(!(data & b4_SEMR_ABCS) )	baud<<=1;
	tmp=pclk/baud;			    //N+1
	u32 mod= 100*pclk/tmp - 100*baud;
	if(baud<=mod)               //afwijking meer dan 1%
		data|=b2_SEMR_BRME;
	self->_sci->SEMR=data;	    //0?0?0?00 !RXDESEL ?BGDM !NFEN ?ABCS 0 ?BRME 0 !ACS0
	//5-6. BRR, MDDR
	self->_sci->BRR=tmp-1;      //???????? N
	if(data & b2_SEMR_BRME)
	{	mod=(512*baud*tmp/pclk+1)/2;
		self->_sci->MDDR=mod;      //???????? M => baud = pclk/(N+1) * M/256
	}
	//ex: baud=921k6 pclk=PCLKB=60MHz => async_divisor(921600,&pclk)
	//  => pclk=7500kHz, BAn=110
	//  => N+1=8 ( => pclk/(N+1) = 937500=921k6+1,73% )
	//  baud <= mod=1590000 => M=(503+1)/2=252 => baud=922851=921k6+0,13%
//	self->rx_out=self->rx_in=0;
//	self->tx_out=self->tx_in=0;
	return 1;
}

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
u08 search4 (u08 const *_str, u08 ch)
{	u08 i=0;
	while(_str[i]!=ch)
		if(4<=++i)	break;
	return i;//& 3;//for default value 0
}

void OBJ1(init_uart, u32 pclk)
{	//1. check settings for using SCI in UART-mode:
	if(SCI_UART!=mode2num(self->_set->mode) )	return;
	//2. check/initialize TxD and/or RxD pin:
	u08 id=CALL(uart_create);
	//3. release the module from stop:
	if(!module_start(id) )						return;
	//4. make settings for use, depending on input clock:
	if(!CALL1(uart_init, pclk) )				return;
	//5. prepare interrupts and set pin functions for SCI:
	eIOPIN pin;
	if(pin=self->pin_.txd, _isiopin(pin) )
	{	txi_prio(self->channel_,2);
		tei_prio(self->channel_,2);
		vset_(self->_sci->SCR,b5_SCR_TE);
		iopin_start(pin, MPC_FUNC[pin]);
		iopin_start(pin=self->pin_.rts, MPC_FUNC[pin]);
	}
	if(pin=self->pin_.rxd, _isiopin(pin) )
	{	rxi_prio(self->channel_,2);
		eri_prio(self->channel_,2);
		vset_(self->_sci->SCR,b6_SCR_RIE|b4_SCR_RE);
		iopin_start(pin, MPC_FUNC[pin]);
		iopin_start(pin=self->pin_.cts, MPC_FUNC[pin]);
	}
	self->init=1;
}

int OBJ(read)
{//	pre: init of SCI and RxD-pin
	if diff_(self->rx)
	{	u08 c; get_(self->rx, SCI_BUFSIZE, c); return c;
	}	return -1;
}
u08 OBJ1(write, u08 c)
{//	pre: init of SCI and TxD-pin
	unsigned int t_ = (self->tx_in+1) % SCI_BUFSIZE;
	while(t_==self->tx_out)	;
	self->tx_buf[self->tx_in]=c;
	self->tx_in=t_;
	if(!self->sending)
		atom()
			CALL(te_irq), self->sending=1;
	return 1;
}

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
void OBJ(er_irq)//void Cserial_rx_complete_irq (Shwserial *self)
{	//HW 37.2.11 Serial Status Register:
	// *1: To clear this flag, confirm that the flag is 1 and then set it to 0
	vclrset_(self->_sci->SSR,
		b5_SSR_ORER|b4_SSR_FER|b3_SSR_PER,
	// *2: Write 1 when writing is necessary
		b6_SSR_RDRF);
}
void OBJ(rx_irq)
{	u08 c=self->_sci->RDR;
//	(optional error handling code comes here when not using ERI-interrupt)
	put_(self->rx, SCI_BUFSIZE, c);
}
void OBJ(tx_irq)
{	if diff_(self->tx)
		get_(self->tx, SCI_BUFSIZE, self->_sci->TDR);
	else vclrset_(self->_sci->SCR, b7_SCR_TIE, b2_SCR_TEIE);
}
void OBJ(te_irq)
{	vclr_(self->_sci->SCR, b2_SCR_TEIE);
	if diff_(self->tx)
	{//HW 37.3.8 Serial Data Transmission (Asynchronous Mode)
	 //The TXI interrupt request at the beginning of transmission is generated
	 //when the SCR.TE bit is set to 1 after the SCR.TIE bit is set to 1 or
	 //when these 2 bits are set to 1 simultaneously by a single instruction.
		vclr_(self->_sci->SCR, b5_SCR_TE);
		vset_(self->_sci->SCR, b5_SCR_TE|b7_SCR_TIE);
	} else self->sending=0;
}
