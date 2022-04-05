#ifndef TINY_LEMON
#define TINY_LEMON

#include<stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef unsigned char  TLbyte;
	typedef unsigned short TLword;
	typedef int            TLbool;
	typedef size_t         TLcounter;

	typedef TLbyte(*TLinputFunc)  (TLbyte inputport);
	typedef void  (*TLoutputFunc) (TLbyte value, TLbyte outputport);
	typedef void  (*TLonClockFunc)(TLbyte pc);

	/* Bits of Flag register */
	typedef enum {
		/* result is 0 */
		TL_ZERO           = (1 << 0), // Z
		/* carry :D */
		TL_CARRY          = (1 << 1), // C
		/* result is negative */
		TL_NEGATIVE       = (1 << 2), // N
		/* negative number + negative number = positive number or vice versa */
		TL_OVERSIGNED     = (1 << 3), // O
		/* disable TryInterrupt function (DoInterrupt function & BRK instruction always works) */
		TL_NO_INTERRUPT   = (1 << 4), // I
		/* BRK instruction set it */
		TL_BREAK          = (1 << 5), // B
		/* push & pop instructions set it and DoInterrupt */
		TL_STACK_OVERFLOW = (1 << 6), // S
		/* unused */
		TL_UNKNOWN        = (1 << 7), // U
	} TLflag;
	
	/* set input callback  (INA instruction) */
	void tlInputFunc  (TLinputFunc   callback);
	/* set output callback (OUT instruction) */
	void tlOutputFunc (TLoutputFunc  callback);
	/* set onclock callback (calls before each instruction) */
	void tlOnClockFunc(TLonClockFunc callback);

	/* run infinity loop of interpreting instructions (increment clock counter foreach instruction) */
	void tlRun();

	/* reset registers & clock counter */
	void tlReset();

	/* random access memory */
	TLbyte tlGetByte(TLbyte address);
	/* random access memory */
	void   tlSetByte(TLbyte address, TLbyte value);

	/* stack memory */
	TLbyte tlStackGetByte(TLbyte address);
	/* stack memory */
	void   tlStackSetByte(TLbyte address, TLbyte value);
	
	/* get bit of flag register (return 0 or 1) */
	TLbool tlGetFlag(TLflag flag);
	/* set bit of flag register */
	void   tlSetFlag(TLflag flag, TLbool value);

	/* do interrupt if TL_NO_INTERRUPT flag set to 0 */
	void   tlTryInterrupt();
	/* push A, D, I, K, O, F, P. jump to interrupt address and set F register to 0 (interrupt address stores at 0xFF byte) */
	void   tlDoInterrupt();

	/* A */
	TLbyte tlGetRegA();
	/* D */
	TLbyte tlGetRegD();
	/* Flag */
	TLbyte tlGetRegF();
	/* input port index */
	TLbyte tlGetRegI();
	/* K */
	TLbyte tlGetRegK();
	/* output port index */
	TLbyte tlGetRegO();
	/* program counter */
	TLbyte tlGetRegP();
	/* stack pointer */
	TLbyte tlGetRegS();

	/* A */
	void tlSetRegA(TLbyte value);
	/* D */
	void tlSetRegD(TLbyte value);
	/* Flag */
	void tlSetRegF(TLbyte value);
	/* input port index */
	void tlSetRegI(TLbyte value);
	/* K */
	void tlSetRegK(TLbyte value);
	/* output port index */
	void tlSetRegO(TLbyte value);
	/* program counter */
	void tlSetRegP(TLbyte value);
	/* stack pointer */
	void tlSetRegS(TLbyte value);

	/* clock counter */
	TLcounter tlGetClockCount();

	// Registers:
	typedef enum {
		TLR_A, // A
		TLR_D, // D
		TLR_F, // Flags
		TLR_I, // Input port index
		TLR_K, // K
		TLR_O, // Output port index
		TLR_P, // Program counter
		TLR_S, // Stack pointer
	} TLregister;

	// Address Modes:
	// // __X - X register stores address of byte
	// // _LX - X register stores address of address of byte :D
	typedef enum {
		TLAM__A,
		TLAM_LA,
		TLAM__D,
		TLAM_LD,
		TLAM__K,
		TLAM_LK,
		TLAM__P, // P register == instruction address + 1
		TLAM_LP,
		TLAM_NA, // _NA - no address
	} TLaddressMode;

	// Instructions:
	// // opcode & address mode
	typedef enum {
		//
		// no operation
		TLI_NOP = 0x00,
		//
		// A = register
		// flags: Z, N
		TLI_MVD = 0x01, TLI_MVF = 0x02, TLI_MVI = 0x03, TLI_MVK = 0x04, TLI_MVO = 0x05, TLI_MVP = 0x06, TLI_MVS = 0x07,
		//
		// register = A
		// flags: Z, N (except LDF)
		TLI_LDA = 0x08, TLI_LDD = 0x09, TLI_LDF = 0x0A, TLI_LDI = 0x0B, TLI_LDK = 0x0C, TLI_LDO = 0x0D, TLI_LDP = 0x0E, TLI_LDS = 0x0F,
		//
		// A = byte
		// flags: Z, N
		TLI_LDA__A = 0x10, TLI_LDA_LA = 0x11, TLI_LDA__D = 0x12, TLI_LDA_LD = 0x13, TLI_LDA__K = 0x14, TLI_LDA_LK = 0x15, TLI_LDA__P = 0x16, TLI_LDA_LP = 0x17,
		//
		// byte = A
		TLI_STA__A = 0x18, TLI_STA_LA = 0x19, TLI_STA__D = 0x1A, TLI_STA_LD = 0x1B, TLI_STA__K = 0x1C, TLI_STA_LK = 0x1D, TLI_STA__P = 0x1E, TLI_STA_LP = 0x1F,
		//
		// exchange A with register
		// flags: Z, N
		TLI_XCA = 0x20, TLI_XCD = 0x21, TLI_XCF = 0x22, TLI_XCI = 0x23, TLI_XCK = 0x24, TLI_XCO = 0x25, TLI_XCP = 0x26, TLI_XCS = 0x27,
		//
		// exchange A with byte
		// flags: Z, N
		TLI_XCB__A = 0x28, TLI_XCB_LA = 0x29, TLI_XCB__D = 0x2A, TLI_XCB_LD = 0x2B, TLI_XCB__K = 0x2C, TLI_XCB_LK = 0x2D, TLI_XCB__P = 0x2E, TLI_XCB_LP = 0x2F,
		//
		// register = 0
		// flags: Z = 1, N = 0 (except ZRF)
		TLI_ZRA = 0x30, TLI_ZRD = 0x31, TLI_ZRF = 0x32, TLI_ZRI = 0x33, TLI_ZRK = 0x34, TLI_ZRO = 0x35, TLI_ZRP = 0x36, TLI_ZRS = 0x37,
		//
		// byte = 0
		TLI_ZRB__A = 0x38, TLI_ZRB_LA = 0x39, TLI_ZRB__D = 0x3A, TLI_ZRB_LD = 0x3B, TLI_ZRB__K = 0x3C, TLI_ZRB_LK = 0x3D, TLI_ZRB__P = 0x3E, TLI_ZRB_LP = 0x3F,
		//
		// flag = 0 or 1
		TLI_FFZ = 0x40, TLI_FFC = 0x41, TLI_FFN = 0x42, TLI_FFO = 0x43, TLI_FFI = 0x44, TLI_FFB = 0x45, TLI_FFS = 0x46, TLI_FFU = 0x47,
		TLI_FTZ = 0x48, TLI_FTC = 0x49, TLI_FTN = 0x4A, TLI_FTO = 0x4B, TLI_FTI = 0x4C, TLI_FTB = 0x4D, TLI_FTS = 0x4E, TLI_FTU = 0x4F,
		//
		// jump if flag == 0 or 1
		TLI_JFZ_LP = 0x50, TLI_JFC_LP = 0x51, TLI_JFN_LP = 0x52, TLI_JFO_LP = 0x53, TLI_JFI_LP = 0x54, TLI_JFB_LP = 0x55, TLI_JFS_LP = 0x56, TLI_JFU_LP = 0x57,
		TLI_JTZ_LP = 0x58, TLI_JTC_LP = 0x59, TLI_JTN_LP = 0x5A, TLI_JTO_LP = 0x5B, TLI_JTI_LP = 0x5C, TLI_JTB_LP = 0x5D, TLI_JTS_LP = 0x5E, TLI_JTU_LP = 0x5F,
		//
		// jump
		TLI_JMP__A = 0x60, TLI_JMP_LA = 0x61, TLI_JMP__D = 0x62, TLI_JMP_LD = 0x63, TLI_JMP__K = 0x64, TLI_JMP_LK = 0x65, TLI_JMP__P = 0x66, TLI_JMP_LP = 0x67,
		// 
		// jump to sub-routine (push P register)
		TLI_JSR__A = 0x68, TLI_JSR_LA = 0x69, TLI_JSR__D = 0x6A, TLI_JSR_LD = 0x6B, TLI_JSR__K = 0x6C, TLI_JSR_LK = 0x6D, TLI_JSR__P = 0x6E, TLI_JSR_LP = 0x6F,
		// 
		// loop
		// A - 1 and jump if A != 0
		TLI_LUP__A = 0x70, TLI_LUP_LA = 0x71, TLI_LUP__D = 0x72, TLI_LUP_LD = 0x73, TLI_LUP__K = 0x74, TLI_LUP_LK = 0x75, TLI_LUP__P = 0x76, TLI_LUP_LP = 0x77,
		// 
		// push: A, D, I, K, O, F
		TLI_PSR = 0x78,
		//
		// pop:  F, O, K, I, D, A
		TLI_PPR = 0x79,
		//
		// return from interrupt
		TLI_RTI = 0x7A,
		//
		// interrupt
		// flags: B = 1
		TLI_BRK = 0x7B,
		//
		// input to port I & output to port O
		// (callbacks)
		// flags: Z, N
		TLI_INA = 0x7C,
		TLI_OUT = 0x7D,
		//
		// halt
		TLI_HLT = 0x7E,
		//  ??? = 0x7F,
		//
		// push register
		TLI_PSA = 0x80, TLI_PSD = 0x81, TLI_PSF = 0x82, TLI_PSI = 0x83, TLI_PSK = 0x84, TLI_PSO = 0x85, TLI_PSP = 0x86, TLI_PSS = 0x87,
		//
		// push byte
		TLI_PSH__A = 0x88, TLI_PSH_LA = 0x89, TLI_PSH__D = 0x8A, TLI_PSH_LD = 0x8B, TLI_PSH__K = 0x8C, TLI_PSH_LK = 0x8D, TLI_PSH__P = 0x8E, TLI_PSH_LP = 0x8F,
		//
		// pop register
		TLI_PPA = 0x90, TLI_PPD = 0x91, TLI_PPF = 0x92, TLI_PPI = 0x93, TLI_PPK = 0x94, TLI_PPO = 0x95, TLI_PPP = 0x96, TLI_PPS = 0x97,
		//
		// pop byte
		TLI_POP__A = 0x98, TLI_POP_LA = 0x99, TLI_POP__D = 0x9A, TLI_POP_LD = 0x9B, TLI_POP__K = 0x9C, TLI_POP_LK = 0x9D, TLI_POP__P = 0x9E, TLI_POP_LP = 0x9F,
		//
		// peek register
		TLI_PKA = 0xA0, TLI_PKD = 0xA1, TLI_PKF = 0xA2, TLI_PKI = 0xA3, TLI_PKK = 0xA4, TLI_PKO = 0xA5, TLI_PKP = 0xA6, TLI_PKS = 0xA7,
		//
		// peek byte
		TLI_PIK__A = 0xA8, TLI_PIK_LA = 0xA9, TLI_PIK__D = 0xAA, TLI_PIK_LD = 0xAB, TLI_PIK__K = 0xAC, TLI_PIK_LK = 0xAD, TLI_PIK__P = 0xAE, TLI_PIK_LP = 0xAF,
		//
		// compare (set flags for A - byte)
		// flags: Z, N, C, O
		TLI_CMP__A = 0xB0, TLI_CMP_LA = 0xB1, TLI_CMP__D = 0xB2, TLI_CMP_LD = 0xB3, TLI_CMP__K = 0xB4, TLI_CMP_LK = 0xB5, TLI_CMP__P = 0xB6, TLI_CMP_LP = 0xB7,
		//
		// test (set flags for A & byte)
		// flags: Z, N
		TLI_TST__A = 0xB8, TLI_TST_LA = 0xB9, TLI_TST__D = 0xBA, TLI_TST_LD = 0xBB, TLI_TST__K = 0xBC, TLI_TST_LK = 0xBD, TLI_TST__P = 0xBE, TLI_TST_LP = 0xBF,
		//
		// add, add with carry, sub, sub with carry
		// flags: Z, N, C, O
		TLI_ADD__P = 0xC0, TLI_ADC__P = 0xC1, TLI_SUB__P = 0xC2, TLI_SBC__P = 0xC3,
		//
		// and, or, nor, xor
		// flags: Z, N
		TLI_AND__P = 0xC4, TLI_ORR__P = 0xC5, TLI_NOR__P = 0xC6, TLI_XOR__P = 0xC7,
		//
		// add, add with carry, sub, sub with carry
		// flags: Z, N, C, O
		TLI_ADD_LP = 0xC8, TLI_ADC_LP = 0xC9, TLI_SUB_LP = 0xCA, TLI_SBC_LP = 0xCB,
		//
		// and, or, nor, xor
		// flags: Z, N
		TLI_AND_LP = 0xCC, TLI_ORR_LP = 0xCD, TLI_NOR_LP = 0xCE, TLI_XOR_LP = 0xCF,
		//
		// inc, dec, not, neg A register
		// flags: Z, N, C, O
		TLI_ICA = 0xD0, TLI_DCA = 0xD1, TLI_NTA = 0xD2, TLI_NGA = 0xD3,
		// 
		// shift (left || right) (with carry) A register
		// flags: Z, N, C
		TLI_SHL = 0xD4, TLI_SLC = 0xD5, TLI_SHR = 0xD6, TLI_SRC = 0xD7,
		//
		// inc, dec, not, neg byte
		// flags: Z, N, C, O
		TLI_INC_LP = 0xD8, TLI_DEC_LP = 0xD9, TLI_NOT_LP = 0xDA, TLI_NEG_LP = 0xDB,
		//
		// inc & dec D & K
		TLI_ICD = 0xDC, TLI_DCD = 0xDD, TLI_ICK = 0xDE, TLI_DCK = 0xDF,
		//
		// ???
		// 0xE0 .. 0xFF

	} TLinstructionByte;

#ifdef __cplusplus
}
#endif

#endif