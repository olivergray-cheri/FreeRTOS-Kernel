/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
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

#ifndef configENABLE_FPU
    #define configENABLE_FPU 0
#endif

#ifndef configENABLE_VPU
    #define configENABLE_VPU 0
#endif

#ifdef __CHERI_PURE_CAPABILITY__
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
   #define ADD  cadd
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
   #define ADD add
   #define ADDI addi
   #define MV   mv
#endif /* __CHERI_PURE_CAPABILITY__ */

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

#if ( configENABLE_FPU == 1 )
    /* Bit [14:13] in the mstatus encode the status of FPU state which is one of
     * the following values:
     * 1. Value: 0, Meaning: Off.
     * 2. Value: 1, Meaning: Initial.
     * 3. Value: 2, Meaning: Clean.
     * 4. Value: 3, Meaning: Dirty.
     */
    #define MSTATUS_FS_MASK                 0x6000
    #define MSTATUS_FS_INITIAL              0x2000
    #define MSTATUS_FS_CLEAN                0x4000
    #define MSTATUS_FS_DIRTY                0x6000
    #define MSTATUS_FS_OFFSET               13

    #ifdef __riscv_fdiv
        #if __riscv_flen == 32
            #define load_f                  flw
            #define store_f                 fsw
        #elif __riscv_flen == 64
            #define load_f                  fld
            #define store_f                 fsd
        #else
            #error Assembler did not define __riscv_flen
        #endif

        #define portFPU_REG_SIZE                ( __riscv_flen / 8 )
        #define portFPU_REG_COUNT               33 /* 32 Floating point registers plus one CSR. */
        #define portFPU_REG_OFFSET( regIndex )  ( ( 2 * portWORD_SIZE ) + ( regIndex * portFPU_REG_SIZE ) )
        #define portFPU_CONTEXT_SIZE            ( portFPU_REG_SIZE * portFPU_REG_COUNT )
    #else
        #error configENABLE_FPU must not be set to 1 if the hardware does not have FPU
    #endif
#endif

#if ( configENABLE_VPU == 1 )
    /* Bit [10:9] in the mstatus encode the status of VPU state which is one of
     * the following values:
     * 1. Value: 0, Meaning: Off.
     * 2. Value: 1, Meaning: Initial.
     * 3. Value: 2, Meaning: Clean.
     * 4. Value: 3, Meaning: Dirty.
     */
    #define MSTATUS_VS_MASK                 0x600
    #define MSTATUS_VS_INITIAL              0x200
    #define MSTATUS_VS_CLEAN                0x400
    #define MSTATUS_VS_DIRTY                0x600
    #define MSTATUS_VS_OFFSET               9

    #ifndef __riscv_vector
        #error configENABLE_VPU must not be set to 1 if the hardware does not have VPU
    #endif
#endif
/*-----------------------------------------------------------*/

.extern pxCurrentTCB
.extern xISRStackTop
.extern xCriticalNesting
.extern pxCriticalNesting
/*-----------------------------------------------------------*/

    .macro portcontexSAVE_FPU_CONTEXT
ADDI SP, SP, -( portFPU_CONTEXT_SIZE )
/* Store the FPU registers. */
store_f f0,  portFPU_REG_OFFSET( 0  )( SP )
store_f f1,  portFPU_REG_OFFSET( 1  )( SP )
store_f f2,  portFPU_REG_OFFSET( 2  )( SP )
store_f f3,  portFPU_REG_OFFSET( 3  )( SP )
store_f f4,  portFPU_REG_OFFSET( 4  )( SP )
store_f f5,  portFPU_REG_OFFSET( 5  )( SP )
store_f f6,  portFPU_REG_OFFSET( 6  )( SP )
store_f f7,  portFPU_REG_OFFSET( 7  )( SP )
store_f f8,  portFPU_REG_OFFSET( 8  )( SP )
store_f f9,  portFPU_REG_OFFSET( 9  )( SP )
store_f f10, portFPU_REG_OFFSET( 10 )( SP )
store_f f11, portFPU_REG_OFFSET( 11 )( SP )
store_f f12, portFPU_REG_OFFSET( 12 )( SP )
store_f f13, portFPU_REG_OFFSET( 13 )( SP )
store_f f14, portFPU_REG_OFFSET( 14 )( SP )
store_f f15, portFPU_REG_OFFSET( 15 )( SP )
store_f f16, portFPU_REG_OFFSET( 16 )( SP )
store_f f17, portFPU_REG_OFFSET( 17 )( SP )
store_f f18, portFPU_REG_OFFSET( 18 )( SP )
store_f f19, portFPU_REG_OFFSET( 19 )( SP )
store_f f20, portFPU_REG_OFFSET( 20 )( SP )
store_f f21, portFPU_REG_OFFSET( 21 )( SP )
store_f f22, portFPU_REG_OFFSET( 22 )( SP )
store_f f23, portFPU_REG_OFFSET( 23 )( SP )
store_f f24, portFPU_REG_OFFSET( 24 )( SP )
store_f f25, portFPU_REG_OFFSET( 25 )( SP )
store_f f26, portFPU_REG_OFFSET( 26 )( SP )
store_f f27, portFPU_REG_OFFSET( 27 )( SP )
store_f f28, portFPU_REG_OFFSET( 28 )( SP )
store_f f29, portFPU_REG_OFFSET( 29 )( SP )
store_f f30, portFPU_REG_OFFSET( 30 )( SP )
store_f f31, portFPU_REG_OFFSET( 31 )( SP )
csrr t0, fcsr
store_w t0,  portFPU_REG_OFFSET( 32 )( SP )
    .endm
/*-----------------------------------------------------------*/

    .macro portcontextRESTORE_FPU_CONTEXT
/* Restore the FPU registers. */
load_f f0,  portFPU_REG_OFFSET( 0  )( SP )
load_f f1,  portFPU_REG_OFFSET( 1  )( SP )
load_f f2,  portFPU_REG_OFFSET( 2  )( SP )
load_f f3,  portFPU_REG_OFFSET( 3  )( SP )
load_f f4,  portFPU_REG_OFFSET( 4  )( SP )
load_f f5,  portFPU_REG_OFFSET( 5  )( SP )
load_f f6,  portFPU_REG_OFFSET( 6  )( SP )
load_f f7,  portFPU_REG_OFFSET( 7  )( SP )
load_f f8,  portFPU_REG_OFFSET( 8  )( SP )
load_f f9,  portFPU_REG_OFFSET( 9  )( SP )
load_f f10, portFPU_REG_OFFSET( 10 )( SP )
load_f f11, portFPU_REG_OFFSET( 11 )( SP )
load_f f12, portFPU_REG_OFFSET( 12 )( SP )
load_f f13, portFPU_REG_OFFSET( 13 )( SP )
load_f f14, portFPU_REG_OFFSET( 14 )( SP )
load_f f15, portFPU_REG_OFFSET( 15 )( SP )
load_f f16, portFPU_REG_OFFSET( 16 )( SP )
load_f f17, portFPU_REG_OFFSET( 17 )( SP )
load_f f18, portFPU_REG_OFFSET( 18 )( SP )
load_f f19, portFPU_REG_OFFSET( 19 )( SP )
load_f f20, portFPU_REG_OFFSET( 20 )( SP )
load_f f21, portFPU_REG_OFFSET( 21 )( SP )
load_f f22, portFPU_REG_OFFSET( 22 )( SP )
load_f f23, portFPU_REG_OFFSET( 23 )( SP )
load_f f24, portFPU_REG_OFFSET( 24 )( SP )
load_f f25, portFPU_REG_OFFSET( 25 )( SP )
load_f f26, portFPU_REG_OFFSET( 26 )( SP )
load_f f27, portFPU_REG_OFFSET( 27 )( SP )
load_f f28, portFPU_REG_OFFSET( 28 )( SP )
load_f f29, portFPU_REG_OFFSET( 29 )( SP )
load_f f30, portFPU_REG_OFFSET( 30 )( SP )
load_f f31, portFPU_REG_OFFSET( 31 )( SP )
load_w t0,  portFPU_REG_OFFSET( 32 )( SP )
csrw fcsr, t0
ADDI SP, SP, ( portFPU_CONTEXT_SIZE )
    .endm
/*-----------------------------------------------------------*/

    .macro portcontexSAVE_VPU_CONTEXT
/* Un-reserve the space reserved for mstatus and epc. */
ADD SP, SP, ( 2 * portWORD_SIZE )

csrr t0, vlenb /* t0 = vlenb. vlenb is the length of each vector register in bytes. */
slli t0, t0, 3 /* t0 = vlenb * 8. t0 now contains the space required to store 8 vector registers. */
neg  t0, t0

/* Store the vector registers in group of 8. */
ADD     SP, SP, t0
vs8r.v  v24, (SP)   /* Store v24-v31. */
ADD     SP, SP, t0
vs8r.v  v16, (SP)   /* Store v16-v23. */
ADD     SP, SP, t0
vs8r.v  v8, (SP)    /* Store v8-v15. */
ADD     SP, SP, t0
vs8r.v  v0, (SP)    /* Store v0-v7. */

/* Store the VPU CSRs. */
ADDI    SP, SP, -( 4 * portWORD_SIZE )
csrr    t0, vstart
store_x t0, 0 * portWORD_SIZE( SP )
csrr    t0, vcsr
store_x t0, 1 * portWORD_SIZE( SP )
csrr    t0, vl
store_x t0, 2 * portWORD_SIZE( SP )
csrr    t0, vtype
store_x t0, 3 * portWORD_SIZE( SP )

/* Re-reserve the space for mstatus and epc. */
ADD sp, sp, -( 2 * portWORD_SIZE )
    .endm
/*-----------------------------------------------------------*/

    .macro portcontextRESTORE_VPU_CONTEXT
/* Un-reserve the space reserved for mstatus and epc. */
ADD SP, SP, ( 2 * portWORD_SIZE )

/* Restore the VPU CSRs. */
load_x  t0, 0  * portWORD_SIZE( SP )
csrw    vstart, t0
load_x  t0, 1 * portWORD_SIZE( SP )
csrw    vcsr, t0
load_x  t0, 2 * portWORD_SIZE( SP )
load_x  t1, 3 * portWORD_SIZE( SP )
vsetvl  x0, t0, t1 /* vlen and vtype can only be updated by using vset*vl* instructions. */
ADDI    SP, SP, ( 4 * portWORD_SIZE )

csrr t0, vlenb /* t0 = vlenb. vlenb is the length of each vector register in bytes. */
slli t0, t0, 3 /* t0 = vlenb * 8. t0 now contains the space required to store 8 vector registers. */

/* Restore the vector registers. */
vl8r.v  v0, (SP)    /* Restore v0-v7. */
ADD     SP, SP, t0
vl8r.v  v8, (SP)    /* Restore v8-v15. */
ADD     SP, SP, t0
vl8r.v  v16, (SP)   /* Restore v16-v23. */
ADD     SP, SP, t0
vl8r.v  v24, (SP)   /* Restore v23-v31. */
ADD     SP, SP, t0

/* Re-reserve the space for mstatus and epc. */
ADD SP, SP, -( 2 * portWORD_SIZE )
    .endm
/*-----------------------------------------------------------*/

   .macro portcontextSAVE_CONTEXT_INTERNAL
ADDI SP, SP, -portCONTEXT_SIZE
store_x X1,  2  * portWORD_SIZE( SP )
store_x X5,  3  * portWORD_SIZE( SP )
store_x X6,  4  * portWORD_SIZE( SP )
store_x X7,  5  * portWORD_SIZE( SP )
store_x X8,  6  * portWORD_SIZE( SP )
store_x X9,  7  * portWORD_SIZE( SP )
store_x X10, 8  * portWORD_SIZE( SP )
store_x X11, 9  * portWORD_SIZE( SP )
store_x X12, 10 * portWORD_SIZE( SP )
store_x X13, 11 * portWORD_SIZE( SP )
store_x X14, 12 * portWORD_SIZE( SP )
store_x X15, 13 * portWORD_SIZE( SP )
#ifndef __riscv_32e
    store_x X16, 14 * portWORD_SIZE( SP )
    store_x X17, 15 * portWORD_SIZE( SP )
    store_x X18, 16 * portWORD_SIZE( SP )
    store_x X19, 17 * portWORD_SIZE( SP )
    store_x X20, 18 * portWORD_SIZE( SP )
    store_x X21, 19 * portWORD_SIZE( SP )
    store_x X22, 20 * portWORD_SIZE( SP )
    store_x X23, 21 * portWORD_SIZE( SP )
    store_x X24, 22 * portWORD_SIZE( SP )
    store_x X25, 23 * portWORD_SIZE( SP )
    store_x X26, 24 * portWORD_SIZE( SP )
    store_x X27, 25 * portWORD_SIZE( SP )
    store_x X28, 26 * portWORD_SIZE( SP )
    store_x X29, 27 * portWORD_SIZE( SP )
    store_x X30, 28 * portWORD_SIZE( SP )
    store_x X31, 29 * portWORD_SIZE( SP )
#endif /* ifndef __riscv_32e */

load_a T0, xCriticalNesting                                   /* Load the address of xCriticalNesting into T0. */
load_w t0, 0( T0 )                                            /* Load the value of xCriticalNesting into t0. */
store_w t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( SP ) /* Store the critical nesting value to the stack. */

#if( configENABLE_FPU == 1 )
    csrr t0, mstatus
    srl t1, t0, MSTATUS_FS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 1f /* If FPU status is not dirty, do not save FPU registers. */

    portcontexSAVE_FPU_CONTEXT
1:
#endif

#if( configENABLE_VPU == 1 )
    csrr t0, mstatus
    srl t1, t0, MSTATUS_VS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 2f /* If VPU status is not dirty, do not save VPU registers. */

    portcontexSAVE_VPU_CONTEXT
2:
#endif

csrr t0, mstatus
store_w t0, 1 * portWORD_SIZE( SP )

portasmSAVE_ADDITIONAL_REGISTERS /* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

#if( configENABLE_FPU == 1 )
    /* Mark the FPU as clean, if it was dirty and we saved FPU registers. */
    srl t1, t0, MSTATUS_FS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 3f

    li t1, ~MSTATUS_FS_MASK
    and t0, t0, t1
    li t1, MSTATUS_FS_CLEAN
    or t0, t0, t1
    csrw mstatus, t0
3:
#endif

#if( configENABLE_VPU == 1 )
    /* Mark the VPU as clean, if it was dirty and we saved VPU registers. */
    srl t1, t0, MSTATUS_VS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 4f

    li t1, ~MSTATUS_VS_MASK
    and t0, t0, t1
    li t1, MSTATUS_VS_CLEAN
    or t0, t0, t1
    csrw mstatus, t0
4:
#endif

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

/* Restore mstatus register. It is important to use t3 (and not t0) here as t3
 * is not clobbered by portcontextRESTORE_VPU_CONTEXT and
 * portcontextRESTORE_FPU_CONTEXT. */
load_w t3, 1 * portWORD_SIZE( SP )
csrw mstatus, t3

#if( configENABLE_VPU == 1 )
    srl t1, t3, MSTATUS_VS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 5f /* If VPU status is not dirty, do not restore VPU registers. */

    portcontextRESTORE_VPU_CONTEXT
5:
#endif /* ifdef portasmSTORE_VPU_CONTEXT */

#if( configENABLE_FPU == 1 )
    srl t1, t3, MSTATUS_FS_OFFSET
    andi t1, t1, 3
    addi t2, x0, 3
    bne t1, t2, 6f /* If FPU status is not dirty, do not restore FPU registers. */

    portcontextRESTORE_FPU_CONTEXT
6:
#endif /* ifdef portasmSTORE_FPU_CONTEXT */

load_w t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( SP ) /* Obtain xCriticalNesting value for this task from task's stack. */
load_a T1, pxCriticalNesting                                 /* Load the address of pxCriticalNesting into t1. */
load_x T1, 0( T1 )                                           /* Load the xCriticalNesting pointer (from *pxCriticalNesting) in to T1 */
store_w t0, 0 ( T1 )                                         /* Restore the critical nesting value for this task to *xCriticalNesting. */

load_x X1,  2  * portWORD_SIZE( SP )
load_x X5,  3  * portWORD_SIZE( SP )
load_x X6,  4  * portWORD_SIZE( SP )
load_x X7,  5  * portWORD_SIZE( SP )
load_x X8,  6  * portWORD_SIZE( SP )
load_x X9,  7  * portWORD_SIZE( SP )
load_x X10, 8  * portWORD_SIZE( SP )
load_x X11, 9  * portWORD_SIZE( SP )
load_x X12, 10 * portWORD_SIZE( SP )
load_x X13, 11 * portWORD_SIZE( SP )
load_x X14, 12 * portWORD_SIZE( SP )
load_x X15, 13 * portWORD_SIZE( SP )
#ifndef __riscv_32e
    load_x X16, 14 * portWORD_SIZE( SP )
    load_x X17, 15 * portWORD_SIZE( SP )
    load_x X18, 16 * portWORD_SIZE( SP )
    load_x X19, 17 * portWORD_SIZE( SP )
    load_x X20, 18 * portWORD_SIZE( SP )
    load_x X21, 19 * portWORD_SIZE( SP )
    load_x X22, 20 * portWORD_SIZE( SP )
    load_x X23, 21 * portWORD_SIZE( SP )
    load_x X24, 22 * portWORD_SIZE( SP )
    load_x X25, 23 * portWORD_SIZE( SP )
    load_x X26, 24 * portWORD_SIZE( SP )
    load_x X27, 25 * portWORD_SIZE( SP )
    load_x X28, 26 * portWORD_SIZE( SP )
    load_x X29, 27 * portWORD_SIZE( SP )
    load_x X30, 28 * portWORD_SIZE( SP )
    load_x X31, 29 * portWORD_SIZE( SP )
#endif /* ifndef __riscv_32e */
ADDI SP, SP, portCONTEXT_SIZE

mret
   .endm
/*-----------------------------------------------------------*/

#endif /* PORTCONTEXT_H */
