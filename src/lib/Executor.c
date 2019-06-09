#include <Executor.h>
#include <core/Core.h>

#include <stdlib.h>

static int run_Executor(Executor* executor)
{
    if(executor->status != Executor_Stop) {
        return -1;
    }
    for(uint32_t i = 0; i < vector_size(executor->cores); ++i) {
        Core* core = vector_at(Core*, executor->cores, i);
        int res = core->run(core);
        if(res) {
            return res;
        }
    }
    executor->status = Executor_Running;
    return 0;
}

static int stop_Executor(Executor* executor)
{
    if(executor->status != Executor_Running) {
        return -1;
    }
    executor->status = Executor_Terminated;
    for(uint32_t i = 0; i < vector_size(executor->cores); ++i) {
        Core* core = vector_at(Core*, executor->cores, i);
        int res = core->stop(core);
        if(res) {
            return res;
        }
    }
    executor->status = Executor_Stop;
    return 0;
}

static int join_Executor(Executor* executor)
{
    if(executor->status != Executor_Running) {
        return 0;
    }
    while(atomic_load(&(executor->runningCores)) > 0) {
        pthread_mutex_lock(&(executor->mutex));
        pthread_cond_wait(&(executor->cond), &(executor->mutex));
        pthread_mutex_unlock(&(executor->mutex));
    }
    executor->status = Executor_Stop;
    return 0;
}

static int addModule_Executor(Executor* executor, ModuleInst* module, uint32_t startFuncIndex)
{
    if(executor->status == Executor_Terminated) {
        return -1;
    }
    vector_push_back(executor->modules, module);
    Core* core = new_Core(executor, module, *vector_at(uint32_t*, module->funcaddrs, startFuncIndex));
    vector_push_back(executor->cores, core);
    if(executor->status == Executor_Running) {
        return core->run(core);
    }
    return 0;
}

Executor* new_Executor()
{
    Executor* executor = (Executor*) malloc(sizeof(Executor));
    atomic_init(&(executor->runningCores), 0);
    pthread_mutex_init(&(executor->mutex), NULL);
    pthread_cond_init(&(executor->cond), NULL);
    executor->status = Executor_Stop;
    executor->run = run_Executor;
    executor->stop = stop_Executor;
    executor->join = join_Executor;
    executor->cores = new_vector_p(sizeof(Core), (void(*)(void*))clean_Core);
    executor->modules = new_vector_p(sizeof(ModuleInst), (void(*)(void*))clean_ModuleInst);
    executor->store = new_Store();
    executor->addModule = addModule_Executor;
    return executor;
}
void free_Executor(Executor* executor)
{
    free_vector_p(executor->cores);
    free_vector_p(executor->modules);
    free_Store(executor->store);
    free(executor);
}