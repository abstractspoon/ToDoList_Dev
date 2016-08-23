/***************************************************************
* C file: cpuid.c... for cpuinf32 DLL
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



/***************************************************************
* This is a 32-bit MS-Windows* DLL.  It uses the standard Intel
* 32-bit cpuid assembly code to determine what type of processor
* is in the computer.
*
* Warning: avoid making frequent calls to wincpuid() or using 
* the CPUID instruction frequently.  This instruction & function
* require several clocks to execute, and they cause 
* serialization of the instruction stream.
***************************************************************/

#include <windows.h> 
#include "cpuid.h"
#include "speed.h"


// Global Variable /////////////////////////////////////////////
int clone_flag;				// Flag to show whether processor
							//   is an Intel clone



// Public DLL Functions ////////////////////////////////////////

/***************************************************************
* wincpuidsupport()
*
* Inputs: none
*
* Returns:
*  1 = CPUID opcode is supported
*  0 = CPUID opcode is not supported
***************************************************************/

WORD wincpuidsupport() {
	int cpuid_support = 1;

	_asm {
        pushfd					// Get original EFLAGS
		pop		eax
		mov 	ecx, eax
        xor     eax, 200000h	// Flip ID bit in EFLAGS
        push    eax				// Save new EFLAGS value on
        						//   stack
        popfd					// Replace current EFLAGS value
        pushfd					// Get new EFLAGS
        pop     eax				// Store new EFLAGS in EAX
        xor     eax, ecx		// Can not toggle ID bit,
        jnz     support			// Processor=80486
		
		mov cpuid_support,0		// Clear support flag
support:
      }
	
	return cpuid_support;

} // wincpuidsupport()



/***************************************************************
* wincpuid()
*
* Inputs: none
*
* Returns:
*  0 = 8086/88
*  2 = 80286
*  3 = 80386
*  4 = 80486
*  5 = Pentium(R) Processor
*  6 = PentiumPro(R) Processor
*  7 or higher = Processor beyond the PentiumPro6(R) Processor
*
*  Note: This function also sets the global variable clone_flag
***************************************************************/

WORD wincpuid() {

	WORD cpuid;
	
	if ( wincpuidsupport() ) 	// Determine whether CPUID 
								//   opcode is supported
		cpuid=check_IDProc();

	else {
		
		clone_flag=check_clone();
	
		cpuid=check_8086();			// Will return FFFFh or 0
		if (cpuid == 0) goto end;
	
    	cpuid=check_80286();       	// Will return FFFFh or 2
		if (cpuid == 2) goto end;

    	cpuid=check_80386();       	// Will return FFFFh or 3
		if (cpuid == 3) goto end;    // temporarily commented out.
        
        cpuid=4;		// If the processor does not support CPUID,
        				//  is not an 8086, 80286, or 80386, assign
        				//  processor to be an 80486
	}

end:
	if (clone_flag)
		cpuid = cpuid | CLONE_MASK;	// Signify that a clone has been
									//   detected by setting MSB high 

   	return cpuid;

} // wincpuid ()



/***************************************************************
* wincpuidext()
*
* Inputs: none
*
* Returns:
* AX(15:14) = Reserved (mask these off in the calling code 
*				before using)
* AX(13:12) = Processor type (00=Standard OEM CPU, 01=OverDrive,
*				10=Dual CPU, 11=Reserved)
* AX(11:8)  = CPU Family (the same 4-bit quantity as wincpuid())
* AX(7:4)   = CPU Model, if the processor supports the CPUID 
*				opcode; zero otherwise
* AX(3:0)   = Stepping #, if the processor supports the CPUID 
*				opcode; zero otherwise
*
*  Note: This function also sets the global variable clone_flag
***************************************************************/

WORD wincpuidext() {

		int i=0;
		WORD cpu_type=0x0000;
		WORD cpuidext=0x0000;
		BYTE vendor_id[13]="------------";
		BYTE intel_id[13]="GenuineIntel";

	if ( wincpuidsupport() ) {

_asm {      

		xor     eax, eax		// Set up for CPUID instruction
        
		CPU_ID                  // Get and save vendor ID

		mov     dword ptr vendor_id, ebx
		mov     dword ptr vendor_id[+4], edx
		mov     dword ptr vendor_id[+8], ecx
}

for (i=0;i<12;i++)
{
	if (!(vendor_id[i]==intel_id[i]))
		clone_flag = 1;    
}

_asm {
        
		cmp     eax, 1			// Make sure 1 is valid input 
        						//   for CPUID
        
        jl      end_cpuidext	// If not, jump to end
        xor     eax, eax
        inc		eax
        CPU_ID					// Get family/model/stepping/
        						//   features

		mov		cpuidext, ax

end_cpuidext:
		mov		ax, cpuidext
    	}
	}
	else {

	cpu_type = wincpuid();		// If CPUID opcode is not
	cpuidext = cpu_type << 8;	//   supported, put family
								//   value in extensions and
	}							//   return
	
	return cpuidext;

} // wincpuidext()



/***************************************************************
* ()
*
* Inputs: none
*
* Returns:
*   0 = Processor which does not execute the CPUID instruction.
*          This includes 8086, 8088, 80286, 80386, and some 
*		   older 80486 processors.                       
*
* Else
*   Feature Flags (refer to App Note AP-485 for description).
*      This DWORD was put into EDX by the CPUID instruction.
*
*	Current flag assignment is as follows:
*
*		bit31..10   reserved (=0)
*		bit9=1      CPU contains a local APIC (iPentium-3V)
*		bit8=1      CMPXCHG8B instruction supported
*		bit7=1      machine check exception supported
*		bit6=0      reserved (36bit-addressing & 2MB-paging)
*		bit5=1      iPentium-style MSRs supported
*		bit4=1      time stamp counter TSC supported
*		bit3=1      page size extensions supported
*		bit2=1      I/O breakpoints supported
*		bit1=1      enhanced virtual 8086 mode supported
*		bit0=1      CPU contains a floating-point unit (FPU)
*
*	Note: New bits will be assigned on future processors... see
*         processor data books for updated information
*
*	Note: This function also sets the global variable clone_flag
***************************************************************/

DWORD wincpufeatures() {

	int i=0;
	DWORD cpuff=0x00000000;
	BYTE vendor_id[13]="------------";
	BYTE intel_id[13]="GenuineIntel";

	if ( wincpuidsupport() ) {

_asm {      

		xor     eax, eax		// Set up for CPUID instruction
        
		CPU_ID                  // Get and save vendor ID

        mov     dword ptr vendor_id, ebx
        mov     dword ptr vendor_id[+4], edx
        mov     dword ptr vendor_id[+8], ecx
}

for (i=0;i<12;i++)
{
	if (!(vendor_id[i]==intel_id[i]))
		clone_flag = 1;    
}

_asm {
         
		cmp     eax, 1			// Make sure 1 is valid input 
        						//   for CPUID
        
        jl      end_cpuff		// If not, jump to end
        xor     eax, eax
        inc		eax
        CPU_ID					// Get family/model/stepping/
        						//   features

		mov		cpuff, edx

end_cpuff:
		mov		eax, cpuff
      }
	}

	return cpuff;

} // wincpufeatures()



/***************************************************************
* winrdtsc()
*
* Inputs: none
*
* Returns:
*   0 = CPU does not support the time stamp register
*
* Else
*   Returns a variable of type TIME_STAMP which is composed of 
*      two DWORD variables. The 'High' DWORD contains the upper
*      32-bits of the Time Stamp Register. The 'Low' DWORD 
*      contains the lower 32-bits of the Time Stamp Register.
*
*  Note: This function also sets the global variable clone_flag
***************************************************************/

struct TIME_STAMP winrdtsc() {

	struct TIME_STAMP timestamp;    // Return variable for time
									//   stamp read
	DWORD features = wincpufeatures();	// Processor Features
	
	timestamp.Low  = 0;
	timestamp.High = 0;
		
	if ( features & 0x00000010 ) {

		RDTSC						// Read Time Stamp

		_asm
			{
			MOV timestamp.Low, EAX
			MOV timestamp.High, EDX

			}
	}
	
	return timestamp;

} // winrdtsc



/***************************************************************
* getdllversion()
*
* Inputs: none
*
* Returns:  Major and Minor version of this DLL.
* 		
*		i.e.	getdllversion() = 0x01 00
*					  Major Version<--|-->Minor Version
*			
***************************************************************/

unsigned short getdllversion(void) {
	unsigned short Version = VERSION;
	
	return Version;

} // getdllversion()



// Internal Private Functions //////////////////////////////////

/***************************************************************
* check_clone()
*
* Inputs: none
*
* Returns:
*   1      if processor is clone (limited detection ability)
*   0      otherwise
***************************************************************/

static WORD check_clone()
{
	short cpu_type=0;

	_asm 
		{
  					MOV AX,5555h	// Check to make sure this
					XOR DX,DX		//   is a 32-bit processor
					MOV CX,2h
					DIV CX			// Perform Division
					CLC
					JNZ no_clone
					JMP clone
		no_clone:	STC
		clone:		PUSHF
					POP AX          // Get the flags
					AND AL,1
					XOR AL,1        // AL=0 is probably Intel,
									//   AL=1 is a Clone
					
					MOV cpu_type, ax
		}
	
    cpu_type = cpu_type & 0x0001;
    
	return cpu_type;
		
} // check_clone()



/***************************************************************
* check_8086()
*
* Inputs: none
*
* Returns: 
*   0      if processor 8086
*   0xffff otherwise
***************************************************************/

static WORD check_8086()
{

		WORD cpu_type=0xffff;

_asm {
        pushf                   // Push original FLAGS
        pop     ax              // Get original FLAGS
        mov     cx, ax          // Save original FLAGS
        and     ax, 0fffh       // Clear bits 12-15 in FLAGS
        push    ax              // Save new FLAGS value on stack
        popf                    // Replace current FLAGS value
        pushf                   // Get new FLAGS
        pop     ax              // Store new FLAGS in AX
        and     ax, 0f000h      // If bits 12-15 are set, then
        cmp     ax, 0f000h      //   processor is an 8086/8088
        mov     cpu_type, 0    	// Turn on 8086/8088 flag
        je      end_8086    	// Jump if processor is 8086/
        						//   8088
        mov		cpu_type, 0ffffh
end_8086:
		push 	cx
		popf
		mov		ax, cpu_type

      }
	
	return cpu_type;

} // check_8086()



/***************************************************************
* check_80286()
*
* Inputs: none
*
* Returns:
*   2      if processor 80286
*   0xffff otherwise
***************************************************************/

static WORD check_80286()
{

		WORD cpu_type=0xffff;

_asm {
		pushf
		pop		cx
		mov		bx, cx
        or      cx, 0f000h      // Try to set bits 12-15
        push    cx              // Save new FLAGS value on stack
        popf                    // Replace current FLAGS value
        pushf                   // Get new FLAGS
        pop     ax              // Store new FLAGS in AX
        and     ax, 0f000h      // If bits 12-15 are clear
        
        mov     cpu_type, 2     // Processor=80286, turn on 
        						//   80286 flag
        
        jz      end_80286       // If no bits set, processor is 
        						//   80286
		
		mov		cpu_type, 0ffffh
end_80286:
		push	bx
		popf
		mov		ax, cpu_type

      }
	
	return cpu_type;

} // check_80286()



/***************************************************************
* check_80386()
*
* Inputs: none
*
* Returns:
*   3      if processor 80386
*   0xffff otherwise
***************************************************************/

static WORD check_80386()
{

		WORD cpu_type=0xffff;

_asm {   
		mov 	bx, sp
		and		sp, not 3
        pushfd					// Push original EFLAGS 
        pop     eax				// Get original EFLAGS
        mov     ecx, eax		// Save original EFLAGS
        xor     eax, 40000h		// Flip AC bit in EFLAGS
        
        push    eax             // Save new EFLAGS value on
        						//   stack
        
        popfd                   // Replace current EFLAGS value
        pushfd					// Get new EFLAGS
        pop     eax             // Store new EFLAGS in EAX
        
        xor     eax, ecx        // Can't toggle AC bit, 
        						//   processor=80386
        
        mov     cpu_type, 3		// Turn on 80386 processor flag
        jz      end_80386		// Jump if 80386 processor
		mov		cpu_type, 0ffffh
end_80386:
		push	ecx
		popfd
		mov		sp, bx
		mov		ax, cpu_type
		and		eax, 0000ffffh
      }

	return cpu_type;

} // check_80386()



/***************************************************************
* check_IDProc()
*
* Inputs: none
*
* Returns:
*  CPU Family (i.e. 4 if Intel 486, 5 if Pentium(R) Processor)
*
*  Note: This function also sets the global variable clone_flag
***************************************************************/

static WORD check_IDProc() {

		int i=0;
		WORD cpu_type=0xffff;
		BYTE stepping=0;
		BYTE model=0;
		BYTE vendor_id[13]="------------";
		BYTE intel_id[13]="GenuineIntel";

_asm {      

        xor     eax, eax		// Set up for CPUID instruction
        
        CPU_ID                  // Get and save vendor ID

        mov     dword ptr vendor_id, ebx
        mov     dword ptr vendor_id[+4], edx
        mov     dword ptr vendor_id[+8], ecx
}

for (i=0;i<12;i++)
{
	if (!(vendor_id[i]==intel_id[i]))
		clone_flag = 1;    
}

_asm {

        cmp     eax, 1			// Make sure 1 is valid input 
        						//   for CPUID
        
        jl      end_IDProc		// If not, jump to end
        xor     eax, eax
        inc		eax
        CPU_ID					// Get family/model/stepping/
        						//   features

		mov 	stepping, al
		and		stepping, 0x0f //0fh
		
		and 	al, 0f0h
		shr		al, 4
		mov 	model, al
		
		and		eax, 0f00h
        shr     eax, 8			// Isolate family
		and		eax, 0fh
        mov     cpu_type, ax	// Set _cpu_type with family

end_IDProc:
		mov		ax, cpu_type
      }
	
	return cpu_type;

} // Check_IDProc()



