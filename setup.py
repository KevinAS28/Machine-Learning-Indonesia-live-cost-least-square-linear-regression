from distutils.core import setup, Extension
name = "MlIdlcCore"
setup(name=name, #in python we will import this mymodule
	version="1.0", ext_modules = [Extension(name, ["ML_binder.cpp"])])