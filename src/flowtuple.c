#include <Python.h>

#include "handle.h"
#include "header.h"
#include "trailer.h"
#include "interval.h"
#include "class.h"
#include "data.h"

PyObject *flowtuple_error = NULL;

static PyMethodDef methods[] = {
    { NULL, NULL, 0, NULL },
};

static struct PyModuleDef flowtuple_def = {
    PyModuleDef_HEAD_INIT,
    "flowtuple",
    "This module wraps the flowtuple library",
    -1,
    methods,
    NULL, NULL, NULL, NULL,
};

PyMODINIT_FUNC PyInit_flowtuple(void) {
    PyObject *m = PyModule_Create(&flowtuple_def);

    init_pyflowtuple_handle(m);
    init_pyflowtuple_header(m);
    init_pyflowtuple_trailer(m);
    init_pyflowtuple_interval(m);
    init_pyflowtuple_class(m);
    init_pyflowtuple_data(m);

    return m;
}