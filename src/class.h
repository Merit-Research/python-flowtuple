/*
 *  class.h
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
