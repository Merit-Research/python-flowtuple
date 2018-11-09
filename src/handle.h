/*
 *  handle.h
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

#ifndef PYFLOWTUPLE_HANDLE_H
#define PYFLOWTUPLE_HANDLE_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleHandle {
    PyObject_HEAD
    flowtuple_handle_t *c_data;
} FlowtupleHandle;

#define HANDLE_OBJ(self) (((FlowtupleHandle*)(self))->c_data)

int init_pyflowtuple_handle(PyObject *module);

#endif