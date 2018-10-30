#ifndef PYFLOWTUPLE_CLASS_H
#define PYFLOWTUPLE_CLASS_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleClass {
    PyObject_HEAD
    flowtuple_class_t *c_data;
    flowtuple_record_t *record;
} FlowtupleClass;

#define CLASS_OBJ(self) (((FlowtupleClass*)(self))->c_data)

PyObject *pyflowtuple_class_from_rec(void *data);
int init_pyflowtuple_class(PyObject *module);

#endif