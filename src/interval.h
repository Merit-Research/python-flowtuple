/*
 *  interval.h
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

#ifndef PYFLOWTUPLE_INTERVAL_H
#define PYFLOWTUPLE_INTERVAL_H

#include <Python.h>
#include <flowtuple.h>

typedef struct _FlowtupleInterval {
    PyObject_HEAD
    flowtuple_interval_t *c_data;
    flowtuple_record_t *record;
} FlowtupleInterval;

#define INTER_OBJ(self) (((FlowtupleInterval*)(self))->c_data)

PyObject *pyflowtuple_interval_from_rec(void *data);
int init_pyflowtuple_interval(PyObject *module);

#endif