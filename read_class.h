#ifndef READ_CLASS_H
#define READ_CLASS_H

#include "class_file.h"

/**
 * Gets the constant at the given index in a constant pool.
 * Asserts that the index is valid (i.e. between 1 and the pool size).
 *
 * @param constant_pool the class's constant pool
 * @param index the 1-indexed constant pool index
 * @return the constant at the given index
 */
cp_info *get_constant(constant_pool_t *constant_pool, uint16_t index);
/**
 * Finds the method with the given name and signature.
 * The descriptor is necessary because Java allows method overloading.
 * This only needs to be called directly to invoke main();
 * for the invokestatic instruction, use find_method_from_index().
 *
 * @param name the method name, e.g. "factorial"
 * @param descriptor the method descriptor string, e.g. "(I)I"
 * @param class the parsed class file
 * @return the method if it was found, or NULL
 */
method_t *find_method(const char *name, const char *descriptor, class_file_t *class);
/**
 * Finds the method corresponding to the given constant pool index.
 *
 * @param index the constant pool index of the Methodref to call
 * @param class the parsed class file
 * @return
 */
method_t *find_method_from_index(uint16_t index, class_file_t *class);
/**
 * Gets the number of (integer) parameters a method takes.
 * Uses the descriptor string of the method to determine its signature.
 */
uint16_t get_number_of_parameters(method_t *method);

/**
 * Reads an entire class file.
 * The end of the parsed methods array is marked by a method with a NULL name.
 *
 * @param class_file the open file to read
 * @return the parsed class file
 */
class_file_t get_class(FILE *class_file);

/**
 * Frees the memory used by a parsed class file.
 *
 * @param class the parsed class file
 */
void free_class(class_file_t *class);

#endif /* READ_CLASS_H */
