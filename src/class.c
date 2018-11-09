/*
 *  class.c
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

#include "class.h"

PyTypeObject FlowtupleClassType;

extern PyObject *flowtuple_error;

PyObject *pyflowtuple_class_from_rec(void *data) {
    flowtuple_record_t *r = (flowtuple_record_t*)data;
    FlowtupleClass *self;
    self = (FlowtupleClass*)FlowtupleClassType.tp_alloc(&FlowtupleClassType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Class object");
        return NULL;
    }

    self->c_data = flowtuple_record_get_class(r);
    self->record = r;
    return (PyObject*)self;
}

static void pyflowtuple_class_dealloc(PyObject *obj) {
    FlowtupleClass *c = (FlowtupleClass*)obj;
    c->c_data = NULL;
    flowtuple_record_free(c->record);
    c->record = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

PyObject *pyflowtuple_class_get_magic(PyObject *self, void *closure) {
    flowtuple_class_t *c = CLASS_OBJ(self);
    uint32_t ret = flowtuple_class_get_magic(c);
    return PyLong_FromUnsignedLong(ret);
}

PyObject *pyflowtuple_class_get_class_type(PyObject *self, void *closure) {
    flowtuple_class_t *c = CLASS_OBJ(self);
    uint16_t ret = flowtuple_class_get_class_type(c);
    return PyLong_FromUnsignedLong(ret);
}

PyObject *pyflowtuple_class_get_key_count(PyObject *self, void *closure) {
    flowtuple_class_t *c = CLASS_OBJ(self);
    uint32_t ret = flowtuple_class_get_key_count(c);
    return PyLong_FromUnsignedLong(ret);
}

static struct PyGetSetDef pyflowtuple_class_getset[] = {
    { "magic",
      (getter)pyflowtuple_class_get_magic,
      NULL,
      "magic value used", NULL },
    { "class_type",
      (getter)pyflowtuple_class_get_class_type,
      NULL,
      "class type", NULL },
    { "key_count",
      (getter)pyflowtuple_class_get_key_count,
      NULL,
      "key count in class (start only)", NULL },
    { NULL },
};

static PyMethodDef pyflowtuple_class_methods[] = {
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleClassType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Class",
    sizeof(FlowtupleClass),
    0,
    .tp_dealloc = pyflowtuple_class_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple class object",
    .tp_methods = pyflowtuple_class_methods,
    .tp_getset = pyflowtuple_class_getset,
    .tp_new = NULL,
};

int init_pyflowtuple_class(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleClassType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleClassType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Class", type);

    return 0;
}