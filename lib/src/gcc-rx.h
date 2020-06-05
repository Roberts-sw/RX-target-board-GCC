#ifndef GCCRX_H_
#define GCCRX_H_
/* gcc-rx.h
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL	5-6-2020	+ b_(v,n), regb_(args...)
	RvL 4-6-2020	aanmaak
------------------------------------------------------------------------- */
#ifndef JOIN
#  define JOIN(a,b)   JOIN_2(a,b)
#  define JOIN_2(a,b) a ## b
#endif
	//registerbits
#define b_(v,n) JOIN(JOIN(b ## v ## _, R), _ ## n) = BIT_(v),
#define regb_(args...) enum JOIN(R,_BITS) { args };

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
#define bit2left(v,from,to)\
	(v)=(v) + MASK_(from,(to)-(from)+0) & ~MASK_(from,(to)-(from))
#define bit2manch(v,from,to)\
	(v)=(v) + MASK_(from,(to)-(from)+1) & ~MASK_(from,(to)-(from))
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

	//deca, hecto, kilo:
#define da( i,...) (10  * (i) )
#define da1(i, d ) (10  *((i)+(d)%10  *1) )
#define h(  i,...) (100 * (i) )
#define h1( i, d ) (100 *((i)+(d)%10 *10) )
#define h2( i, d ) (100 *((i)+(d)%100 *1) )
#define k(  i,...) (1000* (i) )
#define k1( i, d ) (1000*((i)+(d)%10*100) )
#define k2( i, d ) (1000*((i)+(d)%100*10) )
#define k3( i, d ) (1000*((i)+(d)%1000*1) )

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
	/* ---------------------------------------------------------
	HW 2.5 Endian
	   Big-Endian:    byte=b7..b0, word=b15..b0, long=b31..b0
	   Little-Endian: byte=b0..b7, word=b0..b15, long=b0..b31
	--------------------------------------------------------- */
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

	//HW 2.6.1 Fixed Vector Table: 128B
	//HW 2.6.2 Interrupt Vector Table: 1024B

	//HW 5. I/O-Registers
	/* ---------------------------------------------------------
	//  (2) Notes on writing to I/O registers:
	--------------------------------------------------------- */
#define IOREG_WRITE(r,v)	do{(r)=(v); if((r)!=(v)) NOP();} while(0)
#define WRITE_CHK(r,v)		do (r)=(v); while( (r)!=(v) )

	/* ---------------------------------------------------------
	typedef
	--------------------------------------------------------- */

#endif//GCCRX_H_
