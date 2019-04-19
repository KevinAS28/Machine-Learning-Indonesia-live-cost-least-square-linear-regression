#include <python3.5/Python.h>
#include <vector>
#include <stdexcept>
// #include "PyUtils.h"
#include "binder_tools.h"
#include "ML.h"
#include <string>
#include <stdexcept>

//Module name: "MlIdlcCore"
std::string module = "MlIdlcCore";

PyObject * xtotal = NULL; //python float
PyObject * y = NULL; //python float
PyObject * b = NULL; //python float
PyObject * xdata = NULL; //python list of float
PyObject * ydata = NULL; //python list of float

void setDouble(PyObject * (&to), double from){
	to = PyFloat_FromDouble(from);
	if (to==NULL){
		throw std::logic_error("Unable to set double");
	}
}

PyObject * getXTotal(PyObject * self){
	if (xtotal==NULL){
		std::cout << "You must run startThink() first\n";
		return Py_BuildValue("b", false);
	}
	return xtotal;
}

PyObject * getY(PyObject * self){
	if (y==NULL){
		std::cout << "You must run startThink() first\n";
		return Py_BuildValue("b", false);
	}	
	return y;
}

PyObject * getB(PyObject * self){
	if (b==NULL){
		std::cout << "You must run startThink() first\n";
		return Py_BuildValue("b", false);
	}	
	return b;
}

PyObject * getXData(PyObject * self){
	if (xdata==NULL){
		std::cout << "You must run startThink() first\n";
		return Py_BuildValue("b", false);
	}	
	return xdata;
}

PyObject * getYData(PyObject * self){
	if (ydata==NULL){
		std::cout << "You must run startThink() first\n";
		return Py_BuildValue("b", false);
	}	
	return ydata;
}

PyObject * startThink(PyObject * self){
	ReturnStruct result = Start();
	setDouble(xtotal, result.xtotal);
	setDouble(y, result.y);
	setDouble(b, result.b);
	ydata = vectorToList(result.ydata);
	xdata = vectorToList(result.xdata);
	return Py_BuildValue("b", true);
}
char * funcDocs = "";

PyMethodDef MlIdlcCore_funcs[] = {
	{	"startThink",
		(PyCFunction)startThink,
		METH_NOARGS,
		funcDocs},
	{	"getYData",
		(PyCFunction)getYData,
		METH_NOARGS,
		funcDocs},		
	{	"getXData",
		(PyCFunction)getXData,
		METH_NOARGS,
		funcDocs},		
	{	"getY",
		(PyCFunction)getY,
		METH_NOARGS,
		funcDocs},						
	{	"getB",
		(PyCFunction)getB,
		METH_NOARGS,
		funcDocs},					
	{	"getXTotal",
		(PyCFunction)getXTotal,
		METH_NOARGS,
		funcDocs},				
	{	NULL, NULL, 0, NULL}
};

std::string module_doc0 = "Machine Learning Core to predict Indonesian live cost\n\nstartThink() : asking the input and process the data\ngetXTotal() : the total of the x (input)\ngetY(): the cost result from prediction\ngetB(): the constant of y\ngetXData: list of x data\ngetYData(): list of y data";
const char * module_doc = module_doc0.c_str();

PyModuleDef MlIdlcCore_module = {
	PyModuleDef_HEAD_INIT,
	module.c_str(),
	module_doc,
	-1,
	MlIdlcCore_funcs,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC PyInit_MlIdlcCore(void) {
	return PyModule_Create(&MlIdlcCore_module);
}