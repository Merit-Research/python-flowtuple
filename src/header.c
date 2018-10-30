#include "header.h"

PyTypeObject FlowtupleHeaderType;

extern PyObject *flowtuple_error;

PyObject *pyflowtuple_header_from_rec(void *data) {
    flowtuple_record_t *r = (flowtuple_record_t*)data;
    FlowtupleHeader *self;
    self = (FlowtupleHeader*)FlowtupleHeaderType.tp_alloc(&FlowtupleHeaderType, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to create flowtuple.Header object");
        return NULL;
    }

    self->c_data = flowtuple_record_get_header(r);
    self->record = r;
    return (PyObject*)self;
}

static void pyflowtuple_header_dealloc(PyObject *obj) {
    FlowtupleHeader *h = (FlowtupleHeader*)obj;
    h->c_data = NULL;
    flowtuple_record_free(h->record);
    h->record = NULL;
    Py_TYPE(obj)->tp_free(obj);
}

struct _header_str_get {
    const char *(*getter)(flowtuple_header_t*);
};

static PyObject *_get_string_attr(PyObject *self, const struct _header_str_get *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    const char *str = closure->getter(h);

    if (str == NULL) {
        PyObject *error_obj = Py_BuildValue("(siO)", "failed getting value", 0, Py_None);
        Py_INCREF(Py_None);
        PyErr_SetObject(flowtuple_error, error_obj);
        return NULL;
    }

    return Py_BuildValue("s", str);
}

static struct _header_str_get traceuri_get = { flowtuple_header_get_traceuri };

static PyObject *pyflowtuple_header_get_version_major(PyObject *self, void *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    uint8_t ret = flowtuple_header_get_version_major(h);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_header_get_version_minor(PyObject *self, void *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    uint8_t ret = flowtuple_header_get_version_minor(h);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_header_get_local_init_time(PyObject *self, void *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    uint32_t ret = flowtuple_header_get_local_init_time(h);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_header_get_interval_len(PyObject *self, void *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    uint16_t ret = flowtuple_header_get_interval_length(h);
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *pyflowtuple_header_get_plugins(PyObject *self, void *closure) {
    flowtuple_header_t *h = HEADER_OBJ(self);
    uint32_t *plugins = flowtuple_header_get_plugins(h);
    uint16_t len = flowtuple_header_get_plugin_count(h);
    PyObject *ret = PyList_New(len);
    PyObject *tmp;

    for (uint16_t i = 0; i < len; i++) {
        tmp = Py_BuildValue("i", plugins[i]);
        PyList_SetItem(ret, i, tmp);
    }

    return ret;
}

static struct PyGetSetDef pyflowtuple_header_getset[] = {
    { "version_major",
      (getter)pyflowtuple_header_get_version_major,
      NULL,
      "major version of corsaro that created this file", NULL },
    { "version_minor",
      (getter)pyflowtuple_header_get_version_minor,
      NULL,
      "major version of corsaro that created this file", NULL },
    { "local_init_time",
      (getter)pyflowtuple_header_get_local_init_time,
      NULL,
      "local initialization time of file", NULL },
    { "interval_length",
      (getter)pyflowtuple_header_get_interval_len,
      NULL,
      "interval length", NULL },
    { "traceuri",
      (getter)_get_string_attr,
      NULL,
      "uri of source packet capture", &traceuri_get },
    { "plugins",
      (getter)pyflowtuple_header_get_plugins,
      NULL,
      "list of plugin magics included in file", NULL },
    { NULL },
};

static PyMethodDef pyflowtuple_header_methods[] = {
    { NULL, NULL, 0, NULL },
};

PyTypeObject FlowtupleHeaderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "flowtuple.Header",
    sizeof(FlowtupleHeader),
    0,
    .tp_dealloc = pyflowtuple_header_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A flowtuple header object",
    .tp_methods = pyflowtuple_header_methods,
    .tp_getset = pyflowtuple_header_getset,
    .tp_new = NULL,
};

int init_pyflowtuple_header(PyObject *module) {
    PyObject *type;
    if (PyType_Ready(&FlowtupleHeaderType) < 0) {
        return -1;
    }
    type = (PyObject*)&FlowtupleHeaderType;
    Py_INCREF(type);
    PyModule_AddObject(module, "Header", type);

    return 0;
}