#ifndef PYFLOWTUPLE_DATA_H
#define PYFLOWTUPLE_DATA_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleData {
    PyObject_HEAD
    flowtuple_data_t *c_data;
    flowtuple_record_t *record;
} FlowtupleData;

#define DATA_OBJ(self) (((FlowtupleData*)(self))->c_data)

PyObject *pyflowtuple_data_from_rec(void *data);
int init_pyflowtuple_data(PyObject *module);

#endif