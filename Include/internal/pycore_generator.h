#ifndef Py_INTERNAL_GENERATOR_H
#define Py_INTERNAL_GENERATOR_H

#include "ceval_meta.h"
#include "genobject.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

enum PyGeneratorStatus {
    GEN_CREATED   = 0,
    GEN_SUSPENDED = 1,
    GEN_RUNNING   = 2,
    GEN_CLOSED    = 3
};

/* Asynchronous Generators */

typedef struct {
    PyGenObject base;
    PyObject *finalizer;

    /* Flag is set to 1 when hooks set up by sys.set_asyncgen_hooks
       were called on the generator, to avoid calling them more
       than once. */
    int hooks_inited;

    /* Flag is set to 1 when aclose() is called for the first time, or
       when a StopAsyncIteration exception is raised. */
    int closed;

    int running_async;
} PyAsyncGenObject;

PyGenObject *
PyGen_NewWithCode(PyThreadState *ts, PyCodeObject *co);

PyAPI_FUNC(PyObject *) _PyGen_FetchStopIterationValue2(void);
PyAPI_FUNC(PyObject *) _PyGen_YieldFrom(PyGenObject *gen, PyObject *awaitable, PyObject *arg);
void _PyGen_Finalize(PyObject *self);

PyObject *_PyCoro_GetAwaitableIter(PyObject *o);

static inline PyGenObject *
PyGen_FromThread(struct ThreadState *ts)
{
    assert(ts->thread_type == THREAD_GENERATOR);
    return (PyGenObject *)((char*)ts - offsetof(PyGenObject, base.thread));
}

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_GENERATOR_H */
