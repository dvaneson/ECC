#include <Python.h>
#include "ecc.h"

static PyObject* py_point_add(PyObject* self, PyObject* args)
{
    point p1, p2, sum;
    ecurve E;

    if(!PyArg_ParseTuple(args, "(iip)(iip)(iii)", &p1.x, &p1.y, &p1.at_inf, &p2.x, &p2.y, &p2.at_inf, &E.p, &E.a, &E.b))
      return NULL;

    point_add(&p1, &p2, &sum, &E);
    return Py_BuildValue("(iiO)", sum.x, sum.y, sum.at_inf ? Py_True: Py_False);
}

static PyObject* py_double_and_add(PyObject* self, PyObject* args)
{
    point p1, sum;
    ecurve E;
    int num = 1;

    if(!PyArg_ParseTuple(args, "(iip)(iii)|i", &p1.x, &p1.y, &p1.at_inf, &E.p, &E.a, &E.b, &num))
      return NULL;

    double_and_add(&p1, &sum, &E, num);
    return Py_BuildValue("(iiO)", sum.x, sum.y, sum.at_inf ? Py_True: Py_False);
}

static PyObject* py_point_str(PyObject* self, PyObject* args)
{
    point p1;

    if(!PyArg_ParseTuple(args, "(iip)", &p1.x, &p1.y, &p1.at_inf))
      return NULL;

    return Py_BuildValue("s", point_str(&p1));
}


// Module's Function Definition struct
static PyMethodDef eccMethods[] = {
    { "point_add", py_point_add, METH_VARARGS, "Compute point addition on a given Elliptic Curve" },
    { "double_and_add", py_double_and_add, METH_VARARGS, "Compute the multiplication of a point on a given Elliptic Curve" },
    { "point_str", py_point_str, METH_VARARGS, "String representation of a point on an Elliptic Curve" },
    { NULL, NULL, 0, NULL }
};

// Module Definition struct
static struct PyModuleDef eccModule = {
    PyModuleDef_HEAD_INIT,
    "ecc_module",
    "Elliptic Curve Cryptography Module",
    -1,
    eccMethods
};

// Initializes module using our above struct
PyMODINIT_FUNC PyInit_ecc_module(void)
{
    return PyModule_Create(&eccModule);
}
