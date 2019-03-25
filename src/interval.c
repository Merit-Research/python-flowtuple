/*
 *  interval.c
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

#include "interval.h"

PyTypeObject FlowtupleIntervalType;

extern PyObject *flowtuple_error;

PyObject *pyflowtuple_interval_from_rec(void *data) {
    flowtuple_record_t *r = (flowtuple_record_t*)data;
    FlowtupleInterval *self;
    self = (FlowtupleInterval*)FlowtupleIntervalType.tp_alloc(&FlowtupleIntervalType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Interval object");
        return NULL;
    }

    self->c_data = flowtuple_record_get_interval(r);
    self->record = r;
    return (PyObject*)self;
}

static void pyflowtuple_interval_dealloc(PyObject *obj) {
    FlowtupleInterval *i = (FlowtupleInterval*)obj;
    i->c_data = NULL;
    flowtuple_record_free(i->record);
    i->record = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

static PyObject *pyflowtuple_interval_get_number(PyObject *self, void *closure) {
    flowtuple_interval_t *i = INTER_OBJ(self);
    uint16_t ret = flowtuple_interval_get_number(i);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_interval_get_time(PyObject *self, void *closure) {
    flowtuple_interval_t *i = INTER_OBJ(self);
    uint32_t ret = flowtuple_interval_get_time(i);
    return PyLong_FromUnsignedLong(ret);
}

static struct PyGetSetDef pyflowtuple_interval_getset[] = {
    { "number",
      (getter)pyflowtuple_interval_get_number,
      NULL,
      "interval number", NULL },
    { "time",
      (getter)pyflowtuple_interval_get_time,
      NULL,
      "interval time", NULL },
    { NULL },
};

static PyMethodDef pyflowtuple_interval_methods[] = {
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleIntervalType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Interval",
    sizeof(FlowtupleInterval),
    0,
    .tp_dealloc = pyflowtuple_interval_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple interval object",
    .tp_methods = pyflowtuple_interval_methods,
    .tp_getset = pyflowtuple_interval_getset,
    .tp_new = NULL,
};

int init_pyflowtuple_interval(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleIntervalType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleIntervalType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Interval", type);

    return 0;
}
