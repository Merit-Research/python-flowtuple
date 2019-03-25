/*
 *  flowtuple.c
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2012 The Regents of the University of California.
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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

static void init_pyflowtuple_error(PyObject *module) {
    flowtuple_error = PyErr_NewExceptionWithDoc("flowtuple.error",
        "Exception raised when an error occurs within libflowtuple",
        NULL, NULL);
    PyModule_AddObject(module, "error", flowtuple_error);
    Py_INCREF(flowtuple_error);

}

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

    init_pyflowtuple_error(m);
    init_pyflowtuple_handle(m);
    init_pyflowtuple_header(m);
    init_pyflowtuple_trailer(m);
    init_pyflowtuple_interval(m);
    init_pyflowtuple_class(m);
    init_pyflowtuple_data(m);

    return m;
}
