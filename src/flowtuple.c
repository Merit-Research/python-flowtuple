/*
 *  flowtuple.c
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2018 The Regents of the University of California.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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