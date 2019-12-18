#ifndef CLASS_FILE_H
#define CLASS_FILE_H

/*
 * Definitions for pieces of a Java class file. If you're interested,
 * https://docs.oracle.com/javase/specs/jvms/se12/html/jvms-4.html
 * has the complete specification.
 *
 * Since some of these structs correspond to structs in the JVM specification,
 * we use the same names although they don't follow the code quality guidelines.
 */

#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

typedef struct {
    u4 magic;
    u2 minor_version;
    u2 major_version;
} class_header_t;

typedef struct {
    u2 access_flags;
    u2 this_class;
    u2 super_class;
} class_info_t;

typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
} method_info;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
} attribute_info;

typedef struct {
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
} code_t;

typedef struct {
    char *name;
    char *descriptor;
    code_t code;
} method_t;

typedef enum {
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_NameAndType = 12
} cp_tag_t;

typedef struct {
    u2 string_index;
} CONSTANT_Class_info;

typedef struct {
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_FieldOrMethodref_info;

typedef struct {
    int32_t bytes;
} CONSTANT_Integer_info;

typedef struct {
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct {
    cp_tag_t tag;
    u1 *info;
} cp_info;

typedef struct {
    u2 constant_pool_count;
    cp_info *constant_pool;
} constant_pool_t;

typedef struct {
    constant_pool_t constant_pool;
    method_t *methods;
} class_file_t;

#endif /* CLASS_FILE_H */
