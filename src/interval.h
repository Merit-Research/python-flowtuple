#ifndef PYFLOWTUPLE_INTERVAL_H
#define PYFLOWTUPLE_INTERVAL_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleInterval {
    PyObject_HEAD
    flowtuple_interval_t *c_data;
    flowtuple_record_t *record;
} FlowtupleInterval;

#define INTER_OBJ(self) (((FlowtupleInterval*)(self))->c_data)

PyObject *pyflowtuple_interval_from_rec(void *data);
int init_pyflowtuple_interval(PyObject *module);

#endif