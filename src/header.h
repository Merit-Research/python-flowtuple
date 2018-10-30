#ifndef PYFLOWTUPLE_HEADER_H
#define PYFLOWTUPLE_HEADER_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleHeader {
    PyObject_HEAD
    flowtuple_header_t *c_data;
    flowtuple_record_t *record;
} FlowtupleHeader;

#define HEADER_OBJ(self) (((FlowtupleHeader*)(self))->c_data)

PyObject *pyflowtuple_header_from_rec(void *data);
int init_pyflowtuple_header(PyObject *module);

#endif