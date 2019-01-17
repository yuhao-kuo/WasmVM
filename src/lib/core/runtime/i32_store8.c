#include <core/Runtime.h>
#include <dataTypes/Value.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int runtime_i32_store8(Stack* stack, MemInst* memory, uint32_t offset, uint32_t align)
{
    Value *value1 = NULL;
    Value *value2 = NULL;
    int8_t *dataPointer = NULL;
    uint32_t ea = 0;

    // pop data form stack
    stack->entries->pop(stack->entries, (void **)&value2);
    stack->entries->pop(stack->entries, (void **)&value1);

    // check memory range
    ea = (uint32_t)value1->value.i32 + offset;
    if(ea + 1 >= memory->max * 65536) {
        fprintf(stderr, "over the memory range! \n");
        free_Value(value1);
        free_Value(value2);
        return -1;
    }

    // store data to memory
    dataPointer = (int8_t *)((uint8_t *)memory->data->data + ea);
    *dataPointer = (int8_t)value2->value.i32;

    // free object
    free(value1);
    free(value2);

    return 0;
}
