Сборка:

If you have installed one of the supported compilers, set up the compiler for C++ engine applications using the mex command. When provided with an option to select a compiler, select an installed compiler that the MATLAB® Engine API for C++ supports.

	mex -setup -client engine C++

Build your C++ engine program using the MATLAB mex command.

	mex -client engine MyEngineCode.cpp 
	


