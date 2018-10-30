#ifndef PYFLOWTUPLE_TRAILER_H
#define PYFLOWTUPLE_TRAILER_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleTrailer {
    PyObject_HEAD
    flowtuple_trailer_t *c_data;
    flowtuple_record_t *record;
} FlowtupleTrailer;

#define TRAILER_OBJ(self) (((FlowtupleTrailer*)(self))->c_data)

PyObject *pyflowtuple_trailer_from_rec(void *data);
int init_pyflowtuple_trailer(PyObject *module);

#endif