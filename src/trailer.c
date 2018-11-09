/*
 *  trailer.c
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

#include "trailer.h"

PyTypeObject FlowtupleTrailerType;

extern PyObject *flowtuple_error;

PyObject *pyflowtuple_trailer_from_rec(void *data) {
    flowtuple_record_t *r = (flowtuple_record_t*)data;
    FlowtupleTrailer *self;
    self = (FlowtupleTrailer*)FlowtupleTrailerType.tp_alloc(&FlowtupleTrailerType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Trailer object");
        return NULL;
    }

    self->record = r;
    self->c_data = flowtuple_record_get_trailer(r);
    return (PyObject*)self;
}

static void pyflowtuple_trailer_dealloc(PyObject *obj) {
    FlowtupleTrailer *t = (FlowtupleTrailer*)obj;
    t->c_data = NULL;
    flowtuple_record_free(t->record);
    t->record = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

static PyObject *pyflowtuple_trailer_get_packet_cnt(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint64_t ret = flowtuple_trailer_get_packet_count(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_accepted_cnt(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint64_t ret = flowtuple_trailer_get_accepted_count(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_dropped_cnt(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint64_t ret = flowtuple_trailer_get_dropped_count(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_first_packet_time(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint32_t ret = flowtuple_trailer_get_first_packet_time(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_last_packet_time(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint32_t ret = flowtuple_trailer_get_last_packet_time(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_local_final_time(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint32_t ret = flowtuple_trailer_get_local_final_time(t);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_trailer_get_runtime(PyObject *self, void *closure) {
    flowtuple_trailer_t *t = TRAILER_OBJ(self);
    uint32_t ret = flowtuple_trailer_get_runtime(t);
    return PyLong_FromUnsignedLong(ret);
}

static struct PyGetSetDef pyflowtuple_trailer_getset[] = {
    { "packet_count",
      (getter)pyflowtuple_trailer_get_packet_cnt,
      NULL,
      "total packet count", NULL },
    { "accepted_count",
      (getter)pyflowtuple_trailer_get_accepted_cnt,
      NULL,
      "total number of accepted packets", NULL },
    { "dropped_count",
      (getter)pyflowtuple_trailer_get_dropped_cnt,
      NULL,
      "total number of dropped packets", NULL },
    { "first_packet_time",
      (getter)pyflowtuple_trailer_get_first_packet_time,
      NULL,
      "time of first packet", NULL },
    { "last_packet_time",
      (getter)pyflowtuple_trailer_get_last_packet_time,
      NULL,
      "time of final packet", NULL },
    { "local_final_time",
      (getter)pyflowtuple_trailer_get_local_final_time,
      NULL,
      "final time", NULL, },
    { "runtime",
      (getter)pyflowtuple_trailer_get_runtime,
      NULL,
      "corsaro runtime", NULL },
    { NULL },
};

static PyMethodDef pyflowtuple_trailer_methods[] = {
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleTrailerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Trailer",
    sizeof(FlowtupleTrailer),
    0,
    .tp_dealloc = pyflowtuple_trailer_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple trailer object",
    .tp_methods = pyflowtuple_trailer_methods,
    .tp_getset = pyflowtuple_trailer_getset,
    .tp_new = NULL,
};

int init_pyflowtuple_trailer(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleTrailerType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleTrailerType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Trailer", type);

    return 0;
}