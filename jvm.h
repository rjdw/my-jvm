#ifndef JVM_H
#define JVM_H

/**
 * JVM integer instruction mnemonics and opcodes. If you're interested,
 * https://docs.oracle.com/javase/specs/jvms/se12/html/jvms-6.html
 * has the full list of JVM instructions and their specifications.
 */
typedef enum {
    i_iconst_m1 = 0x2,
    i_iconst_0 = 0x3,
    i_iconst_1 = 0x4,
    i_iconst_2 = 0x5,
    i_iconst_3 = 0x6,
    i_iconst_4 = 0x7,
    i_iconst_5 = 0x8,
    i_bipush = 0x10,
    i_sipush = 0x11,
    i_ldc = 0x12,
    i_iload = 0x15,
    i_iload_0 = 0x1a,
    i_iload_1 = 0x1b,
    i_iload_2 = 0x1c,
    i_iload_3 = 0x1d,
    i_istore = 0x36,
    i_istore_0 = 0x3b,
    i_istore_1 = 0x3c,
    i_istore_2 = 0x3d,
    i_istore_3 = 0x3e,
    i_iadd = 0x60,
    i_isub = 0x64,
    i_imul = 0x68,
    i_idiv = 0x6c,
    i_irem = 0x70,
    i_ineg = 0x74,
    i_iinc = 0x84,
    i_ifeq = 0x99,
    i_ifne = 0x9a,
    i_iflt = 0x9b,
    i_ifge = 0x9c,
    i_ifgt = 0x9d,
    i_ifle = 0x9e,
    i_if_icmpeq = 0x9f,
    i_if_icmpne = 0xa0,
    i_if_icmplt = 0xa1,
    i_if_icmpge = 0xa2,
    i_if_icmpgt = 0xa3,
    i_if_icmple = 0xa4,
    i_goto = 0xa7,
    i_ireturn = 0xac,
    i_return = 0xb1,
    i_getstatic = 0xb2,
    i_invokevirtual = 0xb6,
    i_invokestatic = 0xb8,
} jvm_instruction_t;

#endif /* JVM_H */

