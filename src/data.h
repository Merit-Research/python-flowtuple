/*
 *  data.h
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

#ifndef PYFLOWTUPLE_DATA_H
#define PYFLOWTUPLE_DATA_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleData {
    PyObject_HEAD
    flowtuple_data_t *c_data;
    flowtuple_record_t *record;
} FlowtupleData;

#define DATA_OBJ(self) (((FlowtupleData*)(self))->c_data)

PyObject *pyflowtuple_data_from_rec(void *data);
int init_pyflowtuple_data(PyObject *module);

#endif