/* tc-i386.h -- Header file for tc-i386.c
   Copyright (C) 1989-2019 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef TC_I386
#define TC_I386 1

#include "opcodes/i386-opc.h"

struct fix;

#define TARGET_BYTES_BIG_ENDIAN	0

#define TARGET_ARCH		(i386_arch ())
#define TARGET_MACH		(i386_mach ())
extern enum bfd_architecture i386_arch (void);
extern unsigned long i386_mach (void);

#ifdef TE_FreeBSD
#define AOUT_TARGET_FORMAT	"a.out-i386-freebsd"
#endif
#ifdef TE_NetBSD
#define AOUT_TARGET_FORMAT	"a.out-i386-netbsd"
#endif
#ifdef TE_386BSD
#define AOUT_TARGET_FORMAT	"a.out-i386-bsd"
#endif
#ifdef TE_LINUX
#define AOUT_TARGET_FORMAT	"a.out-i386-linux"
#endif
#ifdef TE_Mach
#define AOUT_TARGET_FORMAT	"a.out-mach3"
#endif
#ifdef TE_DYNIX
#define AOUT_TARGET_FORMAT	"a.out-i386-dynix"
#endif
#ifndef AOUT_TARGET_FORMAT
#define AOUT_TARGET_FORMAT	"a.out-i386"
#endif

#ifdef TE_FreeBSD
#define ELF_TARGET_FORMAT	"elf32-i386-freebsd"
#define ELF_TARGET_FORMAT64	"elf64-x86-64-freebsd"
#elif defined (TE_VXWORKS)
#define ELF_TARGET_FORMAT	"elf32-i386-vxworks"
#elif defined (TE_NACL)
#define ELF_TARGET_FORMAT	"elf32-i386-nacl"
#define ELF_TARGET_FORMAT32	"elf32-x86-64-nacl"
#define ELF_TARGET_FORMAT64	"elf64-x86-64-nacl"
#endif

#ifdef TE_HERMIT
#define ELF_TARGET_FORMAT64     "elf64-x86-64-hermit"
#endif

#ifdef TE_SOLARIS
#define ELF_TARGET_FORMAT	"elf32-i386-sol2"
#define ELF_TARGET_FORMAT64	"elf64-x86-64-sol2"
#endif

#ifndef ELF_TARGET_FORMAT
#define ELF_TARGET_FORMAT	"elf32-i386"
#endif

#ifndef ELF_TARGET_FORMAT64
#define ELF_TARGET_FORMAT64	"elf64-x86-64"
#endif

#ifndef ELF_TARGET_FORMAT32
#define ELF_TARGET_FORMAT32	"elf32-x86-64"
#endif

#ifndef ELF_TARGET_L1OM_FORMAT
#define ELF_TARGET_L1OM_FORMAT	"elf64-l1om"
#endif

#ifndef ELF_TARGET_K1OM_FORMAT
#define ELF_TARGET_K1OM_FORMAT	"elf64-k1om"
#endif

#ifndef ELF_TARGET_IAMCU_FORMAT
#define ELF_TARGET_IAMCU_FORMAT	"elf32-iamcu"
#endif

#if ((defined (OBJ_MAYBE_COFF) && defined (OBJ_MAYBE_AOUT)) \
     || defined (OBJ_ELF) || defined (OBJ_MAYBE_ELF) \
     || defined (TE_PE) || defined (TE_PEP) || defined (OBJ_MACH_O))
extern const char *i386_target_format (void);
#define TARGET_FORMAT i386_target_format ()
#else
#ifdef TE_GO32
#define TARGET_FORMAT		"coff-go32"
#endif
#ifdef OBJ_AOUT
#define TARGET_FORMAT		AOUT_TARGET_FORMAT
#endif
#endif

#define SUB_SEGMENT_ALIGN(SEG, FRCHAIN) 0

/* '$' may be used as immediate prefix.  */
#undef LOCAL_LABELS_DOLLAR
#define LOCAL_LABELS_DOLLAR 0
#undef LOCAL_LABELS_FB
#define LOCAL_LABELS_FB 1

extern const char extra_symbol_chars[];
#define tc_symbol_chars extra_symbol_chars

extern const char *i386_comment_chars;
#define tc_comment_chars i386_comment_chars

/* The name of the global offset table generated by the compiler. Allow
   this to be overridden if need be.  */
#ifndef GLOBAL_OFFSET_TABLE_NAME
#define GLOBAL_OFFSET_TABLE_NAME "_GLOBAL_OFFSET_TABLE_"
#endif

#if (defined (OBJ_ELF) || defined (OBJ_MAYBE_ELF)) && !defined (LEX_AT)
#define TC_PARSE_CONS_EXPRESSION(EXP, NBYTES) x86_cons (EXP, NBYTES)
#endif
extern bfd_reloc_code_real_type x86_cons (expressionS *, int);

#define TC_CONS_FIX_NEW(FRAG, OFF, LEN, EXP, RELOC)	\
  x86_cons_fix_new(FRAG, OFF, LEN, EXP, RELOC)
extern void x86_cons_fix_new
(fragS *, unsigned int, unsigned int, expressionS *, bfd_reloc_code_real_type);

#define TC_ADDRESS_BYTES x86_address_bytes
extern int x86_address_bytes (void);

#define DIFF_EXPR_OK    /* foo-. gets turned into PC relative relocs */

#define NO_RELOC BFD_RELOC_NONE

void i386_validate_fix (struct fix *);
#define TC_VALIDATE_FIX(FIX,SEGTYPE,SKIP) i386_validate_fix(FIX)

#define tc_fix_adjustable(X)  tc_i386_fix_adjustable(X)
extern int tc_i386_fix_adjustable (struct fix *);

/* Values passed to md_apply_fix don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

/* ELF wants external syms kept, as does PE COFF.  */
#if defined (TE_PE) && defined (STRICT_PE_FORMAT)
#define EXTERN_FORCE_RELOC				\
  (OUTPUT_FLAVOR == bfd_target_elf_flavour		\
   || OUTPUT_FLAVOR == bfd_target_coff_flavour)
#else
#define EXTERN_FORCE_RELOC				\
  (OUTPUT_FLAVOR == bfd_target_elf_flavour)
#endif

/* This expression evaluates to true if the relocation is for a local
   object for which we still want to do the relocation at runtime.
   False if we are willing to perform this relocation while building
   the .o file.  GOTOFF and GOT32 do not need to be checked here because
   they are not pcrel.  .*/

#define TC_FORCE_RELOCATION_LOCAL(FIX)				\
  (GENERIC_FORCE_RELOCATION_LOCAL (FIX)				\
   || (FIX)->fx_r_type == BFD_RELOC_386_PLT32			\
   || (FIX)->fx_r_type == BFD_RELOC_386_GOTPC			\
   || (FIX)->fx_r_type == BFD_RELOC_X86_64_GOTPCREL		\
   || (FIX)->fx_r_type == BFD_RELOC_X86_64_GOTPCRELX		\
   || (FIX)->fx_r_type == BFD_RELOC_X86_64_REX_GOTPCRELX)

extern int i386_parse_name (char *, expressionS *, char *);
#define md_parse_name(s, e, m, c) i386_parse_name (s, e, c)

extern operatorT i386_operator (const char *name, unsigned int operands, char *);
#define md_operator i386_operator

extern int i386_need_index_operator (void);
#define md_need_index_operator i386_need_index_operator

#define md_register_arithmetic 0

extern const struct relax_type md_relax_table[];
#define TC_GENERIC_RELAX_TABLE md_relax_table

extern int optimize_align_code;

#define md_do_align(n, fill, len, max, around)				\
if ((n)									\
    && !need_pass_2							\
    && optimize_align_code						\
    && (!(fill)								\
	|| ((char)*(fill) == (char)0x90 && (len) == 1))			\
    && subseg_text_p (now_seg))						\
  {									\
    frag_align_code ((n), (max));					\
    goto around;							\
  }

#define MAX_MEM_FOR_RS_ALIGN_CODE 4095

void i386_print_statistics (FILE *);
#define tc_print_statistics i386_print_statistics

extern unsigned int i386_frag_max_var (fragS *);
#define md_frag_max_var i386_frag_max_var

#define md_number_to_chars number_to_chars_littleendian

enum processor_type
{
  PROCESSOR_UNKNOWN,
  PROCESSOR_I386,
  PROCESSOR_I486,
  PROCESSOR_PENTIUM,
  PROCESSOR_PENTIUMPRO,
  PROCESSOR_PENTIUM4,
  PROCESSOR_NOCONA,
  PROCESSOR_CORE,
  PROCESSOR_CORE2,
  PROCESSOR_COREI7,
  PROCESSOR_L1OM,
  PROCESSOR_K1OM,
  PROCESSOR_IAMCU,
  PROCESSOR_K6,
  PROCESSOR_ATHLON,
  PROCESSOR_K8,
  PROCESSOR_GENERIC32,
  PROCESSOR_GENERIC64,
  PROCESSOR_AMDFAM10,
  PROCESSOR_BD,
  PROCESSOR_ZNVER,
  PROCESSOR_BT
};

extern enum processor_type cpu_arch_tune;
extern enum processor_type cpu_arch_isa;
extern i386_cpu_flags cpu_arch_isa_flags;

struct i386_tc_frag_data
{
  enum processor_type isa;
  i386_cpu_flags isa_flags;
  enum processor_type tune;
};

/* We need to emit the right NOP pattern in .align frags.  This is
   done after the text-to-bits assembly pass, so we need to mark it with
   the isa/tune settings at the time the .align was assembled.  */
#define TC_FRAG_TYPE struct i386_tc_frag_data

#define TC_FRAG_INIT(FRAGP)					\
 do								\
   {								\
     (FRAGP)->tc_frag_data.isa = cpu_arch_isa;			\
     (FRAGP)->tc_frag_data.isa_flags = cpu_arch_isa_flags;	\
     (FRAGP)->tc_frag_data.tune = cpu_arch_tune;		\
   }								\
 while (0)

#define WORKING_DOT_WORD 1

/* How to generate NOPs for .nop direct directive.  */
extern void i386_generate_nops (fragS *, char *, offsetT, int);
#define md_generate_nops(frag, where, amount, control) \
  i386_generate_nops ((frag), (where), (amount), (control))

#define HANDLE_ALIGN(fragP)						\
if (fragP->fr_type == rs_align_code) 					\
  {									\
    offsetT __count = (fragP->fr_next->fr_address			\
		       - fragP->fr_address				\
		       - fragP->fr_fix);				\
    if (__count > 0 && __count <= MAX_MEM_FOR_RS_ALIGN_CODE)		\
      md_generate_nops (fragP, fragP->fr_literal + fragP->fr_fix,	\
			__count, 0);					\
  }

/* We want .cfi_* pseudo-ops for generating unwind info.  */
#define TARGET_USE_CFIPOP 1

extern unsigned int x86_dwarf2_return_column;
#define DWARF2_DEFAULT_RETURN_COLUMN x86_dwarf2_return_column

extern int x86_cie_data_alignment;
#define DWARF2_CIE_DATA_ALIGNMENT x86_cie_data_alignment

extern int x86_dwarf2_addr_size (void);
#define DWARF2_ADDR_SIZE(bfd) x86_dwarf2_addr_size ()

#define tc_parse_to_dw2regnum tc_x86_parse_to_dw2regnum
extern void tc_x86_parse_to_dw2regnum (expressionS *);

#define tc_cfi_frame_initial_instructions tc_x86_frame_initial_instructions
extern void tc_x86_frame_initial_instructions (void);

#define md_elf_section_type(str,len) i386_elf_section_type (str, len)
extern int i386_elf_section_type (const char *, size_t);

#ifdef TE_SOLARIS
#define md_fix_up_eh_frame(sec) i386_solaris_fix_up_eh_frame (sec)
extern void i386_solaris_fix_up_eh_frame (segT);
#endif

/* Support for SHF_X86_64_LARGE */
extern bfd_vma x86_64_section_word (char *, size_t);
extern bfd_vma x86_64_section_letter (int, const char **);
#define md_elf_section_letter(LETTER, PTR_MSG)	x86_64_section_letter (LETTER, PTR_MSG)
#define md_elf_section_word(STR, LEN)		x86_64_section_word (STR, LEN)

#if defined (OBJ_ELF) || defined (OBJ_MAYBE_ELF)
extern void x86_cleanup (void);
#define md_cleanup() x86_cleanup ()
#endif

#ifdef TE_PE

#define O_secrel O_md1

#define TC_DWARF2_EMIT_OFFSET  tc_pe_dwarf2_emit_offset
void tc_pe_dwarf2_emit_offset (symbolS *, unsigned int);

#endif /* TE_PE */

/* X_add_symbol:X_op_symbol (Intel mode only) */
#define O_full_ptr O_md2

#ifdef OBJ_MACH_O

#define TC_FORCE_RELOCATION(FIX) (obj_mach_o_force_reloc (FIX))

#define TC_FORCE_RELOCATION_SUB_SAME(FIX,SEG) \
	  (obj_mach_o_force_reloc_sub_same (FIX, SEG))

#define TC_FORCE_RELOCATION_SUB_LOCAL(FIX,SEG) \
	(obj_mach_o_force_reloc_sub_local (FIX, SEG))

#define TC_VALIDATE_FIX_SUB(FIX, SEG) 1

#endif /* OBJ_MACH_O */

#endif /* TC_I386 */
