/*
 *  data.c
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

#include "data.h"
#include "class.h"

PyTypeObject FlowtupleDataType;

extern PyObject *flowtuple_error;

PyObject *pyflowtuple_data_from_rec(void *data) {
    flowtuple_record_t *r = (flowtuple_record_t*)data;
    FlowtupleData *self;
    self = (FlowtupleData*)FlowtupleDataType.tp_alloc(&FlowtupleDataType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Data object");
        return NULL;
    }

    self->c_data = flowtuple_record_get_data(r);
    self->record = r;
    return (PyObject*)self;
}

static void pyflowtuple_data_dealloc(PyObject *obj) {
    FlowtupleData *d = (FlowtupleData*)obj;
    d->c_data = NULL;
    flowtuple_record_free(d->record);
    d->record = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

static PyObject *pyflowtuple_data_get_class_type(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    flowtuple_class_t *c = flowtuple_data_get_class_start(d);
    uint16_t ret = flowtuple_class_get_class_type(c);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_number(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint32_t ret = flowtuple_data_get_number(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_src_ip(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint32_t ret = flowtuple_data_get_src_ip(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_dest_ip(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint32_t ret = flowtuple_data_get_dest_ip(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_src_port(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint16_t ret = flowtuple_data_get_src_port(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_dest_port(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint16_t ret = flowtuple_data_get_dest_port(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_proto(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint8_t ret = flowtuple_data_get_protocol(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_ttl(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint8_t ret = flowtuple_data_get_ttl(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_tcp_flags(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint8_t ret = flowtuple_data_get_tcp_flags(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_get_ip_len(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint16_t ret = flowtuple_data_get_ip_len(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_packet_cnt(PyObject *self, void *closure) {
    flowtuple_data_t *d = DATA_OBJ(self);
    uint32_t ret = flowtuple_data_get_packet_count(d);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_data_is_slash_eight(PyObject *self, PyObject *args) {
    flowtuple_data_t *d = DATA_OBJ(self);
    int ret = flowtuple_data_is_slash_eight(d);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static struct PyGetSetDef pyflowtuple_data_getset[] = {
    { "class_type",
      (getter)pyflowtuple_data_get_class_type,
      NULL,
      "class object related to data object", NULL },
    { "number",
      (getter)pyflowtuple_data_get_number,
      NULL,
      "data record number in class", NULL },
    { "src_ip",
      (getter)pyflowtuple_data_get_src_ip,
      NULL,
      "source ip address", NULL },
    { "dest_ip",
      (getter)pyflowtuple_data_get_dest_ip,
      NULL,
      "destination ip address", NULL },
    { "src_port",
      (getter)pyflowtuple_data_get_src_port,
      NULL,
      "source port", NULL },
    { "dest_port",
      (getter)pyflowtuple_data_get_dest_port,
      NULL,
      "destination port", NULL },
    { "protocol",
      (getter)pyflowtuple_data_get_proto,
      NULL,
      "protocol", NULL },
    { "ttl",
      (getter)pyflowtuple_data_get_ttl,
      NULL,
      "time to live", NULL },
    { "tcp_flags",
      (getter)pyflowtuple_data_tcp_flags,
      NULL,
      "tcp flags", NULL },
    { "ip_len",
      (getter)pyflowtuple_data_get_ip_len,
      NULL,
      "ip length", NULL },
    { "packet_count",
      (getter)pyflowtuple_data_packet_cnt,
      NULL,
      "packet count", NULL },
    { NULL },
};

static PyMethodDef pyflowtuple_data_methods[] = {
    { "is_slash_eight", pyflowtuple_data_is_slash_eight, METH_NOARGS,
      "check to see if destination ip is in slash eight format" },
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleDataType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Data",
    sizeof(FlowtupleData),
    0,
    .tp_dealloc = pyflowtuple_data_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple data object",
    .tp_methods = pyflowtuple_data_methods,
    .tp_getset = pyflowtuple_data_getset,
    .tp_new = NULL,
};

int init_pyflowtuple_data(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleDataType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleDataType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Data", type);

    return 0;
}
