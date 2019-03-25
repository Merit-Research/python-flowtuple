/*
 *  handle.c
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

#include "handle.h"
#include "header.h"
#include "trailer.h"
#include "interval.h"
#include "class.h"
#include "data.h"

PyTypeObject FlowtupleHandleType;

extern PyObject *flowtuple_error;

static PyObject *pyflowtuple_handle_from_obj(void *data) {
    flowtuple_handle_t *h = (flowtuple_handle_t*)data;
    FlowtupleHandle *self;
    self = (FlowtupleHandle*)FlowtupleHandleType.tp_alloc(&FlowtupleHandleType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Handle object");
        return NULL;
    }

    self->c_data = h;
    return (PyObject*)self;
}

PyObject *pyflowtuple_handle_new(PyTypeObject *subtype, PyObject *args, PyObject *kwargs) {
    char *uri;
    flowtuple_handle_t *h;
    flowtuple_errno_t ft_errno;

    if (!PyArg_ParseTuple(args, "s", &uri)) {
        return NULL;
    }

    h = flowtuple_initialize(uri, &ft_errno);
    if (h != NULL) {
        return pyflowtuple_handle_from_obj(h);
    } else {
        PyObject *error_obj = Py_BuildValue("(siO)", flowtuple_strerr(ft_errno), ft_errno, Py_None);
        Py_INCREF(Py_None);
        PyErr_SetObject(flowtuple_error, error_obj);
        return NULL;
    }
}

static void pyflowtuple_handle_dealloc(PyObject *obj) {
    FlowtupleHandle *h = (FlowtupleHandle*)obj;
    flowtuple_release(h->c_data);
    h->c_data = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

struct _handle_str_get {
    const char *(*getter)(flowtuple_handle_t*);
};

static PyObject *_get_string_attr(PyObject *self, const struct _handle_str_get *closure) {
    flowtuple_handle_t *h = HANDLE_OBJ(self);
    const char *str = closure->getter(h);

    if (str == NULL) {
        PyObject *error_obj = Py_BuildValue("(siO)", "failed getting value", 0, Py_None);
        Py_INCREF(Py_None);
        PyErr_SetObject(flowtuple_error, error_obj);
        return NULL;
    }

    return Py_BuildValue("s", str);
}

static struct _handle_str_get uri_get = { flowtuple_handle_get_uri };

static PyObject *pyflowtuple_handle_get_next(PyObject *self, PyObject *args) {
    PyObject *ret;
    flowtuple_handle_t *h = HANDLE_OBJ(self);
    flowtuple_record_t *r = flowtuple_get_next(h);
    flowtuple_record_type_t type;

    if (r == NULL) {
        Py_RETURN_NONE;
    }

    type = flowtuple_record_get_type(r);
    switch (type) {
        case FLOWTUPLE_RECORD_TYPE_HEADER:
            ret = pyflowtuple_header_from_rec(r);
            break;
        case FLOWTUPLE_RECORD_TYPE_TRAILER:
            ret = pyflowtuple_trailer_from_rec(r);
            break;
        case FLOWTUPLE_RECORD_TYPE_INTERVAL:
            ret = pyflowtuple_interval_from_rec(r);
            break;
        case FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS:
            ret = pyflowtuple_class_from_rec(r);
            break;
        case FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA:
            ret = pyflowtuple_data_from_rec(r);
            break;
        case FLOWTUPLE_RECORD_TYPE_NULL:
        default:
            Py_RETURN_NONE;
    }

    return ret;
}

static struct PyGetSetDef pyflowtuple_handle_getset[] = {
    { "uri",
      (getter)_get_string_attr,
      NULL,
      "uri of file used to create handle", &uri_get },
    { NULL },
};

static PyMethodDef pyflowtuple_handle_methods[] = {
    { "get_next", pyflowtuple_handle_get_next, METH_NOARGS,
      "Get next record object from handle" },
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleHandleType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Handle",
    sizeof(FlowtupleHandle),
    0,
    .tp_dealloc = pyflowtuple_handle_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple handle object",
    .tp_methods = pyflowtuple_handle_methods,
    .tp_getset = pyflowtuple_handle_getset,
    .tp_new = pyflowtuple_handle_new,
};

int init_pyflowtuple_handle(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleHandleType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleHandleType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Handle", type);

    return 0;
}
