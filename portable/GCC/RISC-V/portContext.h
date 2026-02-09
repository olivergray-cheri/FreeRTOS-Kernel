/*
 * FreeRTOS Kernel V11.2.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Copyright 2025-2026 Codasip s.r.o. <oliver.gray@codasip.com>
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef PORTCONTEXT_H
#define PORTCONTEXT_H

#ifdef __CHERI_PURE_CAPABILITY__
#define configCHERI_ENABLED 1
#else
#define configCHERI_ENABLED 0
#endif

#if ( configCHERI_ENABLED == 1 )
   #define load_a  lgc  /* Load an immediate pointer (i.e. load immediate address and make it a capability) */
   #define load_x  lc   /* Load  a pointer, i.e. a capability from memory */
   #define store_x sc   /* Store a pointer, i.e. a capability to memory */
#if __riscv_xlen == 64      
   #define portWORD_SIZE    16  /* Size of a capability on the stack */
   #define load_w           ld  /* Load a word from memory (64bit) - For CHERI this is different to a capability which is (128bit)*/
   #define store_w          sd  /* Store a word  to memory (64bit) - For CHERI this is different to a capability which is (128bit)*/ 
#elif __riscv_xlen == 32
   #define portWORD_SIZE    8   /* Size of a capability on the stack */
   #define load_w           lw  /* Load a word from memory (32bit) - For CHERI this is different to a capability which is (64bit)*/
   #define store_w          sw  /* Store a word to memory (32bit) - For CHERI this is different to a capability which is (64bit)*/
#endif
   #define X0 c0
   #define X1 c1
   #define X2 c2
   #define X3 c3
   #define X4 c4
   #define X5 c5
   #define X6 c6
   #define X7 c7
   #define X8 c8
   #define X9 c9
   #define X10 c10
   #define X11 c11
   #define X12 c12
   #define X13 c13
   #define X14 c14
   #define X15 c15
   #define X16 c16
   #define X17 c17
   #define X18 c18
   #define X19 c19
   #define X20 c20
   #define X21 c21
   #define X22 c22
   #define X23 c23
   #define X24 c24
   #define X25 c25
   #define X26 c26
   #define X27 c27
   #define X28 c28
   #define X29 c29
   #define X30 c30
   #define X31 c31
   #define RA cra
   #define SP csp
   #define T0 ct0
   #define T1 ct1
   #define A0 ca0
   #define A1 ca1
   #define A2 ca2
   #define MEPC mepcc
   #define ADDI caddi
   #define MV   cmv
#else
#if __riscv_xlen == 64
   #define portWORD_SIZE    8
   #define load_w           ld  /* Load a word from memory (64bit)*/
   #define store_w          sd  /* Store a word to memory (64bit) */ 
#elif __riscv_xlen == 32
   #define portWORD_SIZE    4
   #define load_w           lw  /* Load a word from memory (32bit)*/
   #define store_w          sw  /* Store a word to memory (32bit) */
#else
   #error Assembler did not define __riscv_xlen
#endif
   #define load_a  la        /* Load an immediate pointer (i.e. load immediate address) */
   #define load_x  load_w    /* Load  a word from memory */
   #define store_x store_w   /* Store a word to memory */
   #define X0 x0
   #define X1 x1
   #define X2 x2
   #define X3 x3
   #define X4 x4
   #define X5 x5
   #define X6 x6
   #define X7 x7
   #define X8 x8
   #define X9 x9
   #define X10 x10
   #define X11 x11
   #define X12 x12
   #define X13 x13
   #define X14 x14
   #define X15 x15
   #define X16 x16
   #define X17 x17
   #define X18 x18
   #define X19 x19
   #define X20 x20
   #define X21 x21
   #define X22 x22
   #define X23 x23
   #define X24 x24
   #define X25 x25
   #define X26 x26
   #define X27 x27
   #define X28 x28
   #define X29 x29
   #define X30 x30
   #define X31 x31
   #define RA ra
   #define SP sp
   #define T0 t0
   #define T1 t1
   #define A0 a0
   #define A1 a1
   #define A2 a2
   #define MEPC mepc
   #define ADDI addi
   #define MV   mv
#endif /*  configCHERI_ENABLED == 1 */

#include "freertos_risc_v_chip_specific_extensions.h"

/* Only the standard core registers are stored by default.  Any additional
 * registers must be saved by the portasmSAVE_ADDITIONAL_REGISTERS and
 * portasmRESTORE_ADDITIONAL_REGISTERS macros - which can be defined in a chip
 * specific version of freertos_risc_v_chip_specific_extensions.h.  See the
 * notes at the top of portASM.S file. */
#ifdef __riscv_32e
    #define portCONTEXT_SIZE               ( 15 * portWORD_SIZE )
    #define portCRITICAL_NESTING_OFFSET    14
#else
    #define portCONTEXT_SIZE               ( 31 * portWORD_SIZE )
    #define portCRITICAL_NESTING_OFFSET    30
#endif

/*-----------------------------------------------------------*/

.extern pxCurrentTCB
.extern xISRStackTop
.extern xCriticalNesting
.extern pxCriticalNesting
/*-----------------------------------------------------------*/

   .macro portcontextSAVE_CONTEXT_INTERNAL
ADDI SP,SP, -portCONTEXT_SIZE
store_x  X1,  1 * portWORD_SIZE( SP )
store_x  X5,  2 * portWORD_SIZE( SP )
store_x  X6,  3 * portWORD_SIZE( SP )
store_x  X7,  4 * portWORD_SIZE( SP )
store_x  X8,  5 * portWORD_SIZE( SP )
store_x  X9,  6 * portWORD_SIZE( SP )
store_x X10,  7 * portWORD_SIZE( SP )
store_x X11,  8 * portWORD_SIZE( SP )
store_x X12,  9 * portWORD_SIZE( SP )
store_x X13, 10 * portWORD_SIZE( SP )
store_x X14, 11 * portWORD_SIZE( SP )
store_x X15, 12 * portWORD_SIZE( SP )
#ifndef __riscv_32e
    store_x X16, 13 * portWORD_SIZE( SP )
    store_x X17, 14 * portWORD_SIZE( SP )
    store_x X18, 15 * portWORD_SIZE( SP )
    store_x X19, 16 * portWORD_SIZE( SP )
    store_x X20, 17 * portWORD_SIZE( SP )
    store_x X21, 18 * portWORD_SIZE( SP )
    store_x X22, 19 * portWORD_SIZE( SP )
    store_x X23, 20 * portWORD_SIZE( SP )
    store_x X24, 21 * portWORD_SIZE( SP )
    store_x X25, 22 * portWORD_SIZE( SP )
    store_x X26, 23 * portWORD_SIZE( SP )
    store_x X27, 24 * portWORD_SIZE( SP )
    store_x X28, 25 * portWORD_SIZE( SP )
    store_x X29, 26 * portWORD_SIZE( SP )
    store_x X30, 27 * portWORD_SIZE( SP )
    store_x X31, 28 * portWORD_SIZE( SP )
#endif /* ifndef __riscv_32e */

load_a T0, xCriticalNesting                                   /* Load the address of xCriticalNesting into T0. */
load_w t0, 0( T0 )                                            /* Load the value of xCriticalNesting into t0. */
store_w t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( SP ) /* Store the critical nesting value to the stack. */


csrr t0, mstatus /* Required for MPIE bit. */
store_w t0, portMSTATUS_OFFSET * portWORD_SIZE( SP ) /* Store the mstatus value to the stack. */

portasmSAVE_ADDITIONAL_REGISTERS /* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

load_a T0, pxCurrentTCB          /* Load the address of pxCurrentTCB. */
load_x T0, 0(T0)                 /* Load pxCurrentTCB. */
store_x SP, 0 ( T0 )             /* Write sp to first TCB member. */

   .endm
/*-----------------------------------------------------------*/

   .macro portcontextSAVE_EXCEPTION_CONTEXT
portcontextSAVE_CONTEXT_INTERNAL
csrr a0, mcause
csrr A1, MEPC
ADDI A1, A1, 4          /* Synchronous so update exception return address to the instruction after the instruction that generated the exception. */
store_x A1, 0 ( SP )    /* Save updated exception return address. */
load_a SP, xISRStackTop /* Load the address of xISRStackTop. */
load_x SP, 0 ( SP )     /* Switch to ISR stack. */
   .endm
/*-----------------------------------------------------------*/

   .macro portcontextSAVE_INTERRUPT_CONTEXT
portcontextSAVE_CONTEXT_INTERNAL
csrr a0, mcause
csrr A1, MEPC
store_x A1, 0 ( SP )    /* Asynchronous interrupt so save unmodified exception return address. */
load_a SP, xISRStackTop /* Load the address of xISRStackTop. */
load_x SP, 0 ( SP )     /* Switch to ISR stack. */
   .endm
/*-----------------------------------------------------------*/

   .macro portcontextRESTORE_CONTEXT
load_a T1, pxCurrentTCB /* Load the address of pxCurrentTCB. */
load_x T1, 0( T1 )      /* Load pxCurrentTCB. */
load_x SP, 0( T1 )      /* Read sp from first TCB member. */

/* Load mepc with the address of the instruction in the task to run next. */
load_x T0, 0 ( SP )
csrw MEPC, T0

/* Defined in freertos_risc_v_chip_specific_extensions.h to restore any registers unique to the RISC-V implementation. */
portasmRESTORE_ADDITIONAL_REGISTERS

/* Load mstatus with the interrupt enable bits used by the task. */
load_w t0, portMSTATUS_OFFSET * portWORD_SIZE( SP )
csrw mstatus, t0                                             /* Required for MPIE bit. */

load_w t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( SP ) /* Obtain xCriticalNesting value for this task from task's stack. */
load_a T1, pxCriticalNesting                                 /* Load the address of pxCriticalNesting into t1. */
load_x T1, 0( T1 )                                           /* Load the xCriticalNesting pointer (from *pxCriticalNesting) in to T1 */
store_w t0, 0 ( T1 )                                         /* Restore the critical nesting value for this task to *xCriticalNesting. */

load_x  X1,  1 * portWORD_SIZE( SP )
load_x  X5,  2 * portWORD_SIZE( SP )
load_x  X6,  3 * portWORD_SIZE( SP )
load_x  X7,  4 * portWORD_SIZE( SP )
load_x  X8,  5 * portWORD_SIZE( SP )
load_x  X9,  6 * portWORD_SIZE( SP )
load_x X10,  7 * portWORD_SIZE( SP )
load_x X11,  8 * portWORD_SIZE( SP )
load_x X12,  9 * portWORD_SIZE( SP )
load_x X13, 10 * portWORD_SIZE( SP )
load_x X14, 11 * portWORD_SIZE( SP )
load_x X15, 12 * portWORD_SIZE( SP )
#ifndef __riscv_32e
   load_x X16, 13 * portWORD_SIZE( SP )
   load_x X17, 14 * portWORD_SIZE( SP )
   load_x X18, 15 * portWORD_SIZE( SP )
   load_x X19, 16 * portWORD_SIZE( SP )
   load_x X20, 17 * portWORD_SIZE( SP )
   load_x X21, 18 * portWORD_SIZE( SP )
   load_x X22, 19 * portWORD_SIZE( SP )
   load_x X23, 20 * portWORD_SIZE( SP )
   load_x X24, 21 * portWORD_SIZE( SP )
   load_x X25, 22 * portWORD_SIZE( SP )
   load_x X26, 23 * portWORD_SIZE( SP )
   load_x X27, 24 * portWORD_SIZE( SP )
   load_x X28, 25 * portWORD_SIZE( SP )
   load_x X29, 26 * portWORD_SIZE( SP )
   load_x X30, 27 * portWORD_SIZE( SP )
   load_x X31, 28 * portWORD_SIZE( SP )
#endif /* ifndef __riscv_32e */
ADDI SP, SP, portCONTEXT_SIZE

mret
   .endm
/*-----------------------------------------------------------*/

#endif /* PORTCONTEXT_H */
