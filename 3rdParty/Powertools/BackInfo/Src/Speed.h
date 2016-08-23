/***************************************************************
*
* C file:   speed.h... for cpuinf32 DLL
*
*       This program has been developed by Intel Corporation.  
*		You have Intel's permission to incorporate this code 
*       into your product, royalty free.  Intel has various 
*	    intellectual property rights which it may assert under
*       certain circumstances, such as if another manufacturer's
*       processor mis-identifies itself as being "GenuineIntel"
*		when the CPUID instruction is executed.
*
*       Intel specifically disclaims all warranties, express or
*       implied, and all liability, including consequential and
*		other indirect damages, for the use of this code, 
*		including liability for infringement of any proprietary
*		rights, and including the warranties of merchantability
*		and fitness for a particular purpose.  Intel does not 
*		assume any responsibility for any errors which may 
*		appear in this code nor any responsibility to update it.
*
*  * Other brands and names are the property of their respective
*    owners.
*
*  Copyright (c) 1995, Intel Corporation.  All rights reserved.
***************************************************************/
 
#ifndef speed_h
#define speed_h



// CONSTANT DEFINITIONS ////////////////////////////////////////
#define CLONE_MASK		0x8000	// Mask to be 'OR'ed with proc-
#define MAXCLOCKS		150		// Maximum number of cycles per
								//   BSF instruction
	// ACCURACY AFFECTING CONSTANTS ////////////////////////////
#define ITERATIONS		4000	// Number of times to repeat BSF
								//   instruction in samplings.
								//   Initially set to 4000.

#define MAX_TRIES		20		// Maximum number of samplings
								//   to allow before giving up
								//   and returning current 
								//   average. Initially set to
								//   20.
	
#define TOLERANCE		1		// Number of MHz to allow
								//   samplings to deviate from
								//   average of samplings.
								//   Initially set to 2.

#define	SAMPLINGS		10		// Number of BSF sequence 
								//   samplings to make.
								//   Initially set to 10.

// VARIABLE STRUCTURE DEFINITIONS //////////////////////////////
struct FREQ_INFO
{
	unsigned long in_cycles;	// Internal clock cycles during
								//   test
								
	unsigned long ex_ticks;		// Microseconds elapsed during 
								//   test
								
	unsigned long raw_freq;		// Raw frequency of CPU in MHz
	
	unsigned long norm_freq;	// Normalized frequency of CPU
								//   in MHz.
};


typedef unsigned short ushort;
typedef unsigned long  ulong;



/***************************************************************
* CpuSpeed() -- Return the raw clock rate of the host CPU.
*
* Inputs:
*	clocks:		NULL: Use default value for number of cycles
*				   per BSF instruction.
*   			Positive Integer: Use clocks value for number
*				   of cycles per BSF instruction.
*                 -1: Use CMos timer to calculate speed
*                     (May not work for WinNT.
*
* Returns:
*		If error then return all zeroes in FREQ_INFO structure
*		Else return FREQ_INFO structure containing calculated 
*       clock frequency, normalized clock frequency, number of 
*       clock cycles during test sampling, and the number of 
*       microseconds elapsed during the sampling.
***************************************************************/
FREQ_INFO cpuspeed(int clocks);
unsigned long cpurawspeed(int clocks);
unsigned long cpunormspeed(int clocks);
unsigned long ProcessorCount();

#endif speed_h
 
