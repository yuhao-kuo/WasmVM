#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <dataTypes/Value.h>
#include <core/Runtime.h>
}
#undef _Bool

SKYPAT_F(Runtime_i32_ctz, regular)
{
    // prepare
    Stack stack = new_Stack();
    Value *value1 = new_i32Value(1 << 31);
    push_Value(stack, value1);

    // run
    runtime_i32_ctz(stack);

    // check
    Value *check = NULL;
    pop_Value(stack,&check);
    EXPECT_EQ(check->value.i32, 31);

    // clean
    free_Value(check);
    free_Stack(stack);
}
