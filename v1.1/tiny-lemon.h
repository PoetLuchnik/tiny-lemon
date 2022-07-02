/* file: C header
 * name: tiny-lemon
 * auth: PoetLuchnik
 * version: 1.1 "the infinity"
 * date: 30.06.2022
 * comment:
 *   this is a header file of tiny-lemon static library that written in C.
 *   don't modify this file to use the library safely.
 * links:
 *   github:  https://github.com/PoetLuchnik/tiny-lemon
 *   youtube: https://www.youtube.com/channel/UCxDOfv6HjZd_nZ2_PoCb5SQ
 *   discord: https://discord.com/invite/vnDGsGpn7F
 */

/* allows you to include this header multiple times */
#ifndef TINY_LEMON_HEADER_INCLUDED
#define TINY_LEMON_HEADER_INCLUDED

/* it is possible to use C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

	/* size_t from */
	#include<stdlib.h>
	/* uint8_t from */
	#include<stdint.h>

//--------------------------------------------------------------------------------------------------------------
// macro

	/* address of byte that stores a pointer to an interrupt handler subroutine */
#define TL_INTH_PTR_ADR (0xFF)

//--------------------------------------------------------------------------------------------------------------
// type definitions

	/* 8-bit unsigned integer */
	typedef uint8_t TLbyte;

	/* the longest unsigned integer to counting a cpu's clocks */
	typedef size_t TLcounter;

	/* boolean type; if the TL function returns this
	   it means the value will be 1 or 0 */
	typedef int TLbool;

	// TL type declaration to use it in callback type definitions
	struct TLdata;
	
	/* callback: when input */
	typedef TLbyte(*TLin)   (struct TLdata* tl, TLbyte port);
	
	/* callback: when output */
	typedef void  (*TLout)  (struct TLdata* tl, TLbyte data, TLbyte port);
	
	/* callback: on clock */
	typedef void  (*TLclock)(struct TLdata* tl);
	
	/* callback: on halt */
	typedef void  (*TLhalt) (struct TLdata* tl);

//--------------------------------------------------------------------------------------------------------------
// main structure definition

	/* the entire state of the tiny-lemon virtual machine.
	 * all members can be read and written
	 */
	typedef struct TLdata {
		/* random access memory */
		TLbyte ram[256];
		/* stack memory */
		TLbyte stk[256];
		/* registers */
		struct {
			/* accumulator */
			TLbyte A;
			/* common purpose register */
			TLbyte D;
			/* flags */
			TLbyte F;
			/* input port */
			TLbyte I;
			/* common purpose register */
			TLbyte K;
			/* output port */
			TLbyte O;
			/* program counter */
			TLbyte P;
			/* stack pointer */
			TLbyte S;
		} reg;
		/* callbacks */
		struct {
			/* the INA operation call this */
			TLin in;
			/* the OUT operation call this */
			TLout out;
			/* the tlClock function call this */
			TLclock clock;
			/* the HLT operation call this */
			TLhalt halt;
		} callback;
		/* the tlClock function increments this member automatically */
		TLcounter clock_counter;
		/* not required for TL to work, but you can use it to link TL to other things */
		void* additional_data;
		/* represents argument address from address mode */
		TLbyte arg_address;
		/* represents argument value from address mode */
		TLbyte arg_value;
	} TL;

//--------------------------------------------------------------------------------------------------------------
// function prototypes

	/* initialize the machine's default state.
	   be sure to call this function once before operating the machine */
	void tlInit(TL* tl);

	/* sequencing:
	 * calls "on clock" callback;
	 * load instructions at P;
	 * increment P;
	 * calls address mode;
	 * calls operation;
	 * increment clock counter.
	 */
	void tlClock(TL* tl);

	/* an infinity loop of tlClock calls */
	void tlRun(TL* tl);

	/* force an external interrupt, ignoring disable interrupt flag.
	   not recommended but possible */
	void tlDoExternalInterrupt(TL* tl, TLbyte value);

	/* an external interrupt attempt.
	   returns 1 on success */
	TLbool tlTryExternalInterrupt(TL* tl, TLbyte value);

#ifdef __cplusplus
}
#endif

#endif