#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "jvm.h"
#include "read_class.h"

/** The name of the method to invoke to run the class file */
const char *MAIN_METHOD = "main";
/**
 * The "descriptor" string for main(). The descriptor encodes main()'s signature,
 * i.e. main() takes a String[] and returns void.
 * If you're interested, the descriptor string is explained at
 * https://docs.oracle.com/javase/specs/jvms/se12/html/jvms-4.html#jvms-4.3.2.
 */

const char *MAIN_DESCRIPTOR = "([Ljava/lang/String;)V";


//bipush is the bipush function
//0x10 b	bipush b	Pushes b onto the operand stack.
void bipush(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr)
{
    int8_t param = code_arr[pc+1];
    op_stack[op_counter] = param;

}

//sipush
//0x11 b1 b2	sipush b1 b2	(b1<<8)|b2
//pushing a signed short onto op_stack
void sipush(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr)
{
    uint8_t param1 = code_arr[pc+1];
    uint8_t param2 = code_arr[pc+2];
    int16_t res =  ((param1<<8)|param2);
    op_stack[op_counter] = res;

}

//iadd is the iadd function
//0x60	iadd
//Pops the top two values from the operand stack and pushes back the res
void iadd(int32_t *op_stack, int32_t op_counter)
{

    int32_t op1 = op_stack[op_counter-1];
    int32_t op2 = op_stack[op_counter-2];
    int32_t res = op1 + op2;
    op_stack[op_counter-2] = res;

}


//isub is the isub function
//0x60	isub
//Pops the top two values from the operand stack and pushes back the res
void isub(int32_t *op_stack, int32_t op_counter)
{

    int32_t op1 = op_stack[op_counter-1];
    int32_t op2 = op_stack[op_counter-2];
    op_stack[op_counter-2] = op2 - op1;

}

//imul is the imul function
//0x60	imul
//Pops the top two values from the operand stack and pushes back the res
void imul(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    int32_t op2 = op_stack[op_counter-2];
    op_stack[op_counter-2] = op2 * op1;
}

//idiv is the idiv function
//0x60	idiv
//Pops the top two values from the operand stack and pushes back the res
void idiv(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    int32_t op2 = op_stack[op_counter-2];
    op_stack[op_counter-2] = op2/op1;
}

//irem function
//0x60	irem
//Pops the top two values from the operand stack and pushes back the result.
void irem(int32_t *op_stack, int32_t op_counter)
{
    //0x60	irem
    //Pops the top two values from the operand stack and pushes back the result.
    int32_t op1 = op_stack[op_counter-1];
    int32_t op2 = op_stack[op_counter-2];
    op_stack[op_counter-2] = op2%op1;
}

//ineg function
//negates the top element on the operand stack
void ineg(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    op_stack[op_counter-1] = -1 * op1;
}

//getstatic function
void getstatic()
{
}

//invokevirtual
//prints to the file
void invokevirtual(int32_t *op_stack, int32_t op_counter)
{
    int32_t op = op_stack[op_counter-1];
    printf("%d\n", op);
}

//iconst
//loads a constant onto the op stack
void iconst(int32_t *op_stack, int32_t op_counter, uint8_t curr_code)
{
    op_stack[op_counter] = curr_code - 0x03;
}


//istore_c
//pops top of the op_stack sets the c position of the locals array to the popped element
void istore_c(int32_t *op_stack, int32_t op_counter, int32_t *locals, uint8_t curr_code)
{
    int32_t index = curr_code-0x3b;
    locals[index] = op_stack[op_counter-1];
}

/**
 * Runs a method's instructions until the method returns.
 *
 * @param method the method to run
 * @param locals the array of local variables, including the method parameters.
 *   Except for parameters, the locals are uninitialized.
 * @param class the class file the method belongs to
 * @return if the method returns an int, a heap-allocated pointer to it;
 *   if the method returns void, NULL
 */
int32_t *execute(method_t *method, int32_t *locals, class_file_t *class)
{
    /* You should remove these casts to void in your solution.
     * They are just here so the code compiles without warnings.
    */
    code_t code = method->code;
    int32_t op_stack[code.max_stack];

    //stays one above the current top index
    uint32_t op_counter = 0;

    //stays at the program to be run
    uint32_t pc = 0;
    uint8_t *code_arr = code.code;



    int loopCount = 0;


    while (pc < code.code_length)
    {

        loopCount+=1;
        uint8_t curr_code = code_arr[pc];



        switch (curr_code)
        {



            case i_ireturn:
            {
                //@return if the method returns an int, a heap-allocated pointer to it;
                //if the method returns void, NULL
                int32_t res = op_stack[op_counter-1];

                int32_t *ret = malloc(sizeof(int32_t));

                *ret = res;

                return ret;


            }
            break;

            case 0xb1:
            {
                //0xb1	return
                //Return NULL (main should exit with status code 0)

                return NULL;

            }
            break;

            case i_invokestatic:
            {
                //recursively calls methods in the code
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];
                uint16_t index =  ((param1<<8)|param2);

                //the method to be called
                method_t *my_method = find_method_from_index(index, class);

                uint16_t num_params = get_number_of_parameters(my_method);

                int32_t my_locals[my_method->code.max_locals];

                //copies the op_stack onto a queue
                for (int i = num_params-1; i>=0; i--)
                {
                    my_locals[i] = op_stack[op_counter-1];
                    op_counter-=1;
                }

                int32_t *exec_res = execute(my_method, my_locals, class);

                if ((exec_res) != NULL)
                {
                    op_stack[op_counter] = *exec_res;
                    op_counter+=1;
                }

                free (exec_res);



                pc+=3;
            }
            break;

            //quite a bit of code duplication in these comparison functions
            //I wroted it previously all-in-one, but it was difficult to read
            //much easier on the eyes this way
            //if equals
            case i_ifeq:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];


                //3 for the current program
                pc+=3;
                if (conditional == 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;

            //if not equals
            case i_ifne:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];



                //3 for the current program
                pc+=3;
                if (conditional != 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;

            //if less than 0
            case i_iflt:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];



                //3 for the current program
                pc+=3;
                if (conditional < 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;

            //if greater than or equal to zero
            case i_ifge:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];



                //3 for the current program
                pc+=3;
                if (conditional >= 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;

            //if greater than zero
            case i_ifgt:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];



                //3 for the current program
                pc+=3;
                if (conditional > 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;

            //if less than or equal to zero
            case i_ifle:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t conditional = op_stack[op_counter-1];




                //3 for the current program
                pc+=3;
                if (conditional <= 0)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=1;
            }
            break;


            //if equals
            case i_if_icmpeq:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];






                //3 for the current program
                pc+=3;
                if (op2 == op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //if not equals
            case i_if_icmpne:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];



                //3 for the current program
                pc+=3;
                if (op2 != op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //if less than
            case i_if_icmplt:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];



                //3 for the current program
                pc+=3;
                if (op2 < op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //if greater than or equal to
            case i_if_icmpge:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];



                //3 for the current program
                pc+=3;
                if (op2 >= op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //if greater than
            case i_if_icmpgt:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];



                //3 for the current program
                pc+=3;
                if (op2 > op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //if less than or equal to
            case i_if_icmple:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];

                int32_t op1 = op_stack[op_counter-1];
                int32_t op2 = op_stack[op_counter-2];


                //3 for the current program
                pc+=3;
                if (op2 <= op1)
                {
                    int16_t res =  ((param1<<8)|param2);
                    pc+=res;
                    pc-=3;
                }
                op_counter-=2;
            }
            break;

            //goto the address
            case i_goto:
            {
                uint8_t param1 = code_arr[pc+1];
                uint8_t param2 = code_arr[pc+2];



                //3 for the current program
                int16_t res =  ((param1<<8)|param2);
                pc+=res;
            }
            break;



            //load constant from the constant_pool
            case i_ldc:
            {


                //get the constant pool in the class
                constant_pool_t constant_pool = class->constant_pool;


                //find the param which will be the index from which we take the
                //constant in the constant pool
                int16_t param = code_arr[pc+1];


                //get the constant in the form of a cp-info struct
                cp_info *cp = get_constant(&constant_pool, param);
                uint8_t *info = cp->info;


                CONSTANT_Integer_info *int_info = (CONSTANT_Integer_info*) (info);

                int32_t loaded = int_info->bytes;


                op_stack[op_counter] = loaded;
                pc += 2;
                op_counter+=1;
            }
            break;


            //these are load_0 - load_3
            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
            {
                //subtraction of the first op_code
                //finds the constant to be loaded
                int32_t param = curr_code - 0x1a;
                int32_t loaded = locals[param];
                op_stack[op_counter] = loaded;
                pc += 1;
                op_counter+=1;
            }
            break;

            case i_iload:
            {
                int32_t param = code_arr[pc+1];
                int32_t loaded = locals[param];

                op_stack[op_counter] = loaded;
                pc += 2;
                op_counter+=1;
            }
            break;

            case i_istore:
            {

                int32_t param = code_arr[pc+1];
                int32_t stored = op_stack[op_counter-1];

                locals[param] = stored;

                pc += 2;
                op_counter-=1;
            }
            break;

            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            {

                //istore_constant
                //subtraction of the first op_code
                //finds the constant to be stored
                int32_t param = curr_code - 0x3b;
                int32_t stored = op_stack[op_counter-1];

                locals[param] = stored;

                pc += 1;
                op_counter-=1;
            }
            break;

            //iinc i b
            //locals[i] += b
            //b is a signed value
            case i_iinc:
            {

                uint8_t i = code_arr[pc+1];
                int8_t b = code_arr[pc+2];

                locals[i] += b;
                pc += 3;

            }
            break;


            case 0x10:
            {
                //0x10 b	bipush b	Pushes b onto the operand stack.
                bipush(op_stack, op_counter, pc, code_arr);
                op_counter += 1;
                pc += 2;

            }
            break;
            //addition
            case 0x60:
            {

                //0x60	iadd
                //Pops the top two values from the operand stack and pushes back the res
                iadd( op_stack,  op_counter);
                op_counter -= 1;
                pc +=1;

            }
            break;

            //subtraction
            case 0x64:
            {

                //0x60	isub
                //Pops the top two values from the operand stack and pushes back the res
                isub(op_stack,  op_counter);
                op_counter -= 1;
                pc +=1;

            }
            break;

            case 0x68:
            {

                //0x60	imul
                //Pops the top two values from the operand stack and pushes back the res
                imul(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;

            }
            break;
            case 0x6c:
            {

                //0x60	idiv
                //Pops the top two values from the operand stack and pushes back the res
                idiv(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;


            }
            break;
            case 0x70:
            {

                irem(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;


            }
            break;
            case 0x74:
            {

                //0x60	ineg
                //Pops the top two values from the operand stack and pushes back the res
                ineg(op_stack, op_counter);
                pc +=1;

            }
            break;


            case 0xb2:
            {
                //0xb2 b1 b2
                //getstatic b1 b2
                //Moves the program counter past b2 (i.e., increment it by three).
                getstatic();
                pc += 3;


            }
            break;

            case 0xb6:
            {
                //0xb6 b1 b2
                //invokevirtual b1 b2
                //Pops and prints the top value of the
                //operand stack followed by a newline character.
                //Then, moves the program counter past b2 (i.e., increment it by three).
                invokevirtual(op_stack, op_counter);
                op_counter -= 1;
                pc += 3;



            }
            break;
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
            case 0x08:
            {
                //iconst_m1 = 2 (0x2)
                //iconst_0 = 3 (0x3)
                //iconst_1 = 4 (0x4)
                //iconst_2 = 5 (0x5)
                //iconst_3 = 6 (0x6)
                //iconst_4 = 7 (0x7)
                //iconst_5 = 8 (0x8)
                //Push the int constant <i> (-1, 0, 1, 2, 3, 4 or 5)
                //onto the operand stack.
                iconst(op_stack, op_counter, curr_code);
                op_counter+=1;
                pc += 1;


            }
                break;

            case 0x11:
            {
                //0x11 b1 b2	sipush b1 b2	(b1<<8)|b2
                //pushing a signed short onto op_stack
                sipush(op_stack, op_counter, pc, code_arr);
                op_counter +=1;
                pc+=3;


            }
            break;


        }
    }
    return NULL;

}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    // Open the class file for reading
    FILE *class_file = fopen(argv[1], "r");
    assert(class_file && "Failed to open file");

    // Parse the class file
    class_file_t class = get_class(class_file);
    int error = fclose(class_file);
    assert(!error && "Failed to close file");
    // Execute the main method
    method_t *main_method = find_method(MAIN_METHOD, MAIN_DESCRIPTOR, &class);
    assert(main_method && "Missing main() method");
    /* In a real JVM, locals[0] would contain a reference to String[] args.
     * But since TeenyJVM doesn't support Objects, we leave it uninitialized. */
    int32_t locals[main_method->code.max_locals];
    int32_t *result = execute(main_method, locals, &class);
    assert(!result && "main() should return void");

    // Free the internal data structures
    free_class(&class);
}
