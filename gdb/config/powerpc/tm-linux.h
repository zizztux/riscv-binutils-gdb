/* Definitions to target GDB to Linux on 386.
   Copyright 1992, 1993 Free Software Foundation, Inc.

This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef TM_LINUX_H
#define TM_LINUX_H

#include "powerpc/tm-ppc-eabi.h"
/* Avoid warning from redefinition in tm-sysv4.h (included from tm-linux.h) */
#undef SKIP_TRAMPOLINE_CODE
#include "tm-linux.h"

/* We can single step on linux */
#undef  SOFTWARE_SINGLE_STEP
#define SOFTWARE_SINGLE_STEP(p,q) abort() /* Will never execute! */
#undef  SOFTWARE_SINGLE_STEP_P
#define SOFTWARE_SINGLE_STEP_P 0

/* Make sure nexti gets the help it needs for debugging assembly code
   without symbols */

#define AT_SUBROUTINE_CALL_INSTRUCTION_TARGET(prevpc,stoppc) \
   at_subroutine_call_instruction_target(prevpc,stoppc)
extern int at_subroutine_call_instruction_target();

/* We _want_ the SVR4 section offset calculations (see syms_from_objfile()
   in symfile.c) */
#undef IBM6000_TARGET

/* Offset to saved PC in sigcontext, from <linux/signal.h>.  */
#define SIGCONTEXT_PC_OFFSET 184

extern CORE_ADDR ppc_linux_skip_trampoline_code (CORE_ADDR pc);
#undef SKIP_TRAMPOLINE_CODE
#define	SKIP_TRAMPOLINE_CODE(pc) ppc_linux_skip_trampoline_code (pc)

extern int ppc_linux_in_sigtramp (CORE_ADDR pc, char *func_name);
#undef IN_SIGTRAMP
#define IN_SIGTRAMP(pc,func_name) ppc_linux_in_sigtramp (pc,func_name)

extern unsigned long ppc_linux_frame_saved_pc (struct frame_info *);
#undef FRAME_SAVED_PC
#define FRAME_SAVED_PC(FRAME) ppc_linux_frame_saved_pc (FRAME)

extern void ppc_linux_init_extra_frame_info (int fromleaf, struct frame_info *);
#undef  INIT_EXTRA_FRAME_INFO
#define	INIT_EXTRA_FRAME_INFO(fromleaf, fi) \
  ppc_linux_init_extra_frame_info (fromleaf, fi)

extern int ppc_linux_frameless_function_invocation (struct frame_info *);
#undef FRAMELESS_FUNCTION_INVOCATION
#define FRAMELESS_FUNCTION_INVOCATION(FI) \
  (ppc_linux_frameless_function_invocation (FI))

extern void ppc_linux_frame_init_saved_regs (struct frame_info *);
#undef FRAME_INIT_SAVED_REGS
#define FRAME_INIT_SAVED_REGS(FI) ppc_linux_frame_init_saved_regs (FI)

CORE_ADDR ppc_linux_frame_chain (struct frame_info *);
#undef FRAME_CHAIN
#define FRAME_CHAIN(thisframe) ppc_linux_frame_chain (thisframe)

CORE_ADDR ppc_sysv_abi_push_arguments (int, struct value **, CORE_ADDR, int,
				       CORE_ADDR);
#undef PUSH_ARGUMENTS
#define	PUSH_ARGUMENTS(nargs, args, sp, struct_return, struct_addr) \
  (ppc_sysv_abi_push_arguments((nargs), (args), (sp), (struct_return), (struct_addr)))

#define CANNOT_FETCH_REGISTER(regno) ((regno) >= MQ_REGNUM)
#define CANNOT_STORE_REGISTER(regno) ((regno) >= MQ_REGNUM)

/* Linux doesn't use the PowerOpen ABI for function pointer representation */
#undef CONVERT_FROM_FUNC_PTR_ADDR

#if 0 /* If skip_prologue() isn't too greedy, we don't need this */
/* There is some problem with the debugging symbols generated by the
   compiler such that the debugging symbol for the first line of a
   function overlap with the function prologue.  */
#define PROLOGUE_FIRSTLINE_OVERLAP
#endif

/* Needed to handled the self-modifying code situation due to the dynamic
   linker. */
int ppc_linux_memory_remove_breakpoint (CORE_ADDR addr, char *contents_cache);
#undef MEMORY_REMOVE_BREAKPOINT
#define MEMORY_REMOVE_BREAKPOINT(addr, contents_cache) \
  ppc_linux_memory_remove_breakpoint(addr, contents_cache)

/* N_FUN symbols in shared libaries have 0 for their values and need
   to be relocated. */
#define SOFUN_ADDRESS_MAYBE_MISSING

#endif  /* #ifndef TM_LINUX_H */
