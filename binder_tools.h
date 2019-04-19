#ifndef binder_tools
#define binder_tools

#include <python3.5/Python.h>
#include <vector>
#include <stdexcept>
// #include "PyUtils.h"

template <class Obj>
PyObject * vectorToList(std::vector<Obj> &data){
	PyObject * listObj = PyList_New(data.size());
	if (listObj==NULL){
		throw std::logic_error("Unable to allocate memory for Python list");
	}
	PyObject *dat;
	for (int i = 0; i < data.size();i++){
		dat = PyFloat_FromDouble((double)data[i]);
		if (dat==NULL) {
			Py_DECREF(listObj);
			throw std::logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, dat);
	}
	return listObj;
}

#endif