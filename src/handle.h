#ifndef PYFLOWTUPLE_HANDLE_H
#define PYFLOWTUPLE_HANDLE_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleHandle {
    PyObject_HEAD
    flowtuple_handle_t *c_data;
} FlowtupleHandle;

#define HANDLE_OBJ(self) (((FlowtupleHandle*)(self))->c_data)

int init_pyflowtuple_handle(PyObject *module);

#endif