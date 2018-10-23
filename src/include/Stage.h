#ifndef WASMVM_STAGE_DEF
#define WASMVM_STAGE_DEF

typedef struct _wasmvm_Stage_{
    void* input;
    void* output;
    void (*setInput)(struct _wasmvm_Stage_* stage, void* input);
    void* (*getOutput)(struct _wasmvm_Stage_* stage);
    int (*run)(struct _wasmvm_Stage_* stage); 
} Stage;

#endif