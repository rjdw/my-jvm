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


//updates the size of opstack, works like an arraylist
//guess I don't need this, didn't see max_stack
//I'll go fuck myself i guess
// int32_t* updateOpStack(uint32_t opCounter, int32_t* opStack)
// {
//     uint32_t len = sizeof(opStack)/sizeof(int32_t);
//     if (opCounter >= len)
//     {
//         int32_t temp[len*2];
//         memcpy(temp, opStack, len+1);
//         return temp;
//     }
//     return opStack;
//
// }


//bipush is the bipush function
void bipush(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr)
{

    int8_t param = code_arr[pc+1];
    op_stack[op_counter] = param;

}

//sipush
void sipush(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr)
{
    uint8_t param1 = code_arr[pc+1];
    uint8_t param2 = code_arr[pc+2];
    int16_t res =  ((param1<<8)|param2);
    op_stack[op_counter] = res;

}

//iadd is the iadd function
void iadd(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    int32_t op2 = op_stack[op_counter-2];
    //op_stack[op_counter-2] = NULL;
    //printf("addition: %d + %d = %d", op1, op2, op1+op2);
    int32_t res = op1 + op2;
    op_stack[op_counter-2] = res;
}


//isub is the isub function
void isub(int32_t *op_stack, int32_t op_counter)
{

    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    int32_t op2 = op_stack[op_counter-2];
    //op_stack[op_counter-2] = NULL;
    //printf("subtraction: %d + %d = %d", op1, op2, op1-op2);
    op_stack[op_counter-2] = op2 - op1;

}

//imul is the imul function
void imul(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    int32_t op2 = op_stack[op_counter-2];
    //op_stack[op_counter-2] = NULL;
    //printf("mul: %d + %d = %d", op1, op2, op1*op2);
    op_stack[op_counter-2] = op1 * op2;
}

//idiv is the idiv function
void idiv(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    int32_t op2 = op_stack[op_counter-2];
    //op_stack[op_counter-2] = NULL;
    //printf("div: %d + %d = %d", op1, op2, op1/op2);
   // if (op2 == 0)
   // {
    //    op_stack[op_counter-2] = 0;
   // }
   // else {
        op_stack[op_counter-2] = op2/op1;
    //}
}

//irem function
void irem(int32_t *op_stack, int32_t op_counter)
{
    //0x60	irem
    //Pops the top two values from the operand stack and pushes back the result.
    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    int32_t op2 = op_stack[op_counter-2];
    //op_stack[op_counter-2] = NULL;
    //printf("mod: %d + %d = %d", op1, op2, op1%op2);
   // if (op2 == 0)
   // {
   //     op_stack[op_counter-2] = 0;
   // }
   /// else
   // {
        op_stack[op_counter-2] = op2%op1;
   // }

}

//ineg function
void ineg(int32_t *op_stack, int32_t op_counter)
{
    int32_t op1 = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;
    //printf("neg: %d + %d = %d", op1, op2, -1*op2);
    op_stack[op_counter-1] = -1 * op1;
}

//getstatic function
void getstatic()
{
}

//invokevirtual
void invokevirtual(int32_t *op_stack, int32_t op_counter)
{
    int32_t op = op_stack[op_counter-1];
    //op_stack[op_counter-1] = NULL;

    printf("%d\n", op);
}


//iconst
void iconst(int32_t *op_stack, int32_t op_counter, uint8_t curr_code)
{
    op_stack[op_counter] = curr_code - 0x03;
}

//iload
void iload(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr, int32_t *locals)
{
    int8_t param = code_arr[pc+1];
    op_stack[op_counter] = locals[param];
}

//istore
void istore(int32_t *op_stack, int32_t op_counter, uint32_t pc, uint8_t *code_arr, int32_t *locals)
{
    int32_t op = op_stack[op_counter-1];
    int8_t  param = code_arr[pc+1];
    locals[param] = op;


    int32_t opSize = sizeof(op_stack)/sizeof(int32_t);
    int32_t locSize = sizeof(locals)/sizeof(locals[0]);


    fprintf(stderr, "\n\n\nop_stack length: %d, index param size %d: \n\n\n", opSize, op_counter-1);
    fprintf(stderr, "\n\n\nlocals length: %d, index param size %d: \n\n\n", locSize, param);


}


//istore_c
//pops top of the op_stack sets the c position of the locals array to the popped element
void istore_c(int32_t *op_stack, int32_t op_counter, int32_t *locals, uint8_t curr_code)
{
    int32_t index = curr_code-0x3b;
    locals[index] = op_stack[op_counter-1];
}

//iload_c
//loads the cth element of the locals array onto the op_stack
void iload_c(int32_t *op_stack, int32_t op_counter, int32_t *locals, uint8_t curr_code)
{
    int32_t index = curr_code-0x1a;
    op_stack[op_counter] = locals[index];
}

//iinc i b
//locals[i] += b
void iinc(uint32_t pc, uint8_t *code_arr, int32_t *locals)
{

    int32_t inc = code_arr[pc+2];
    int32_t index= code_arr[pc+1];
    locals[index] += inc;

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
int32_t *execute(method_t *method, int32_t *locals, class_file_t *class) {
    /* You should remove these casts to void in your solution.
     * They are just here so the code compiles without warnings.
    */
    code_t code = method->code;
    int32_t op_stack[code.max_stack];
    uint32_t op_counter = 0;
    uint32_t pc = 0;
    uint8_t *code_arr = code.code;
    //int32_t pc_max = sizeof(code.code)/sizeof(int8_t);

    //typedef uint8_t u1;
    //typedef uint16_t u2;
    //typedef uint32_t u4;
    //iterates through the entire code


    while (pc < code.code_length) 
    {
        //*op_stack = updateop_stack(op_counter, *op_stack);

        uint8_t curr_code = code_arr[pc];
switch (curr_code)
        {
	    
	    case 0x15:
            {
                fprintf(stderr, "\n%s\n", "iload is called");

                //iload b loads the var in the b index of the locals array onto the op_stack
                iload(op_stack, op_counter, pc, code_arr, locals);
                op_counter += 1;
                pc += 2;
                break;

            }
            case 0x36:
            {

                //0x36 b	istore b
                //moves top of the op_stack to the b position of the locals arr
                //pops the op_stack
                fprintf(stderr, "\n%s\n", "istore is called");
		                fprintf(stderr, "\nthe program counter %d\n", pc);

                fprintf(stderr, "\n%s\n", "the op code ");
                for (int i = 0; i<sizeof(code_arr)/sizeof(int8_t); i++)
                {
                    fprintf(stderr, "%d\n", code_arr[i]);
                }

                istore(op_stack, op_counter, pc, code_arr, locals);
                op_counter -= 1;
                pc += 2;
                break;

            }

            case 0x84:
            {
                fprintf(stderr, "\n%s\n", "iinc is called");

                //0x84 i b	iinc i b
                //increments the element at the i position of the locals array by b
                //locals[i]+=b
                iinc( pc, code_arr, locals);
                pc += 3;
                break;

            }

            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
            {
                fprintf(stderr, "\n%s\n", "iload is called");
                //iload_0 - iload3
                iload_c(op_stack, op_counter, locals, curr_code);
                op_counter+=1;
                pc += 1;
                break;

            }


            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            {
                fprintf(stderr, "\n%s\n", "ist is called");
                //istore_0 - istore_3
                istore_c(op_stack, op_counter, locals, curr_code);
                op_counter-=1;
                pc += 1;
                break;

            }
       
	    case 0x10:
            {
            //0x10 b	bipush b	Pushes b onto the operand stack.

                bipush(op_stack, op_counter, pc, code_arr);
                op_counter += 1;
                pc += 2;
                break;
            }
            //addition
            case 0x60:
            {

                //0x60	iadd
                //Pops the top two values from the operand stack and pushes back the result.
                iadd( op_stack,  op_counter);
                op_counter -= 1;
                pc +=1;
                break;
            }

            //subtraction
            case 0x64:
            {

                //0x60	isub
                //Pops the top two values from the operand stack and pushes back the result.
                isub(op_stack,  op_counter);
                op_counter -= 1;
                pc +=1;
                break;
            }

            case 0x68:
            {

                //0x60	imul
                //Pops the top two values from the operand stack and pushes back the result.
                imul(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;
                break;
            }

            case 0x6c:
            {

                //0x60	idiv
                //Pops the top two values from the operand stack and pushes back the result.
                idiv(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;
                break;

            }

            case 0x70:
            {
                irem(op_stack, op_counter);
                op_counter -= 1;
                pc +=1;
                break;

            }

            case 0x74:
            {

                //0x60	ineg
                //Pops the top two values from the operand stack and pushes back the result.
                ineg(op_stack, op_counter);
                pc +=1;
                break;
            }


            case 0xb2:
            {
                //0xb2 b1 b2
                //getstatic b1 b2
                //Moves the program counter past b2 (i.e., increment it by three).
                getstatic();
                pc += 3;
                break;

            }

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
                break;


            }

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
                //Push the int constant <i> (-1, 0, 1, 2, 3, 4 or 5) onto the operand stack.
                iconst(op_stack, op_counter, curr_code);
                op_counter+=1;
                pc += 1;
                break;

            }

            case 0x11:
            {

		fprintf(stderr, "\n\nsipush is called\n\n");    
                //0x11 b1 b2	sipush b1 b2	(b1<<8)|b2
                //pushing a signed short onto op_stack
                sipush(op_stack, op_counter, pc, code_arr);
                op_counter +=1;
                pc+=3;
                break;

            }

            case 0xb1:
            {
                //0xb1	return
                //Return NULL (main should exit with status code 0)

                if (strcmp(method->name, MAIN_METHOD) == 0)
                {
                    exit(0);
                }
                return NULL;
                break;
            }
        }	
    }
    return NULL;

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <class file>\n", argv[0]);
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
