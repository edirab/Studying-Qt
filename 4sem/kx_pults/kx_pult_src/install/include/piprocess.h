/*! \file piprocess.h
* \brief Process
*/
/*
	PIP - Platform Independent Primitives
	Process
	Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PIPROCESS_H
#define PIPROCESS_H

#include "pithread.h"
#include "pifile.h"

/// events:
/// execStarted(PIString program)
/// execFinished(PIString program, int exit_code)
///
/// handlers:
/// bool exec(const PIString & program)
/// bool exec(const PIString & program, const PIString & arg1)
/// bool exec(const PIString & program, const PIString & arg1, const PIString & arg2)
/// bool exec(const PIString & program, const PIString & arg1, const PIString & arg2, const PIString & arg3)
/// bool exec(const PIString & program, const PIStringList & args)
/// void terminate()
/// bool waitForFinish(int timeout_msecs = 60000)
class PIP_EXPORT PIProcess: private PIThread
{
	PIOBJECT_SUBCLASS(PIProcess, PIThread)
public:
	PIProcess();
	virtual ~PIProcess();

	int exitCode() const {return exit_code;}
	int pID() const;

	void setGrabInput(bool yes) {g_in = yes;}
	void setGrabOutput(bool yes) {g_out = yes;}
	void setGrabError(bool yes) {g_err = yes;}
	void setInputFile(const PIString & path) {f_in.setPath(path);}
	void setOutputFile(const PIString & path) {f_out.setPath(path);}
	void setErrorFile(const PIString & path) {f_err.setPath(path);}
	void unsetInputFile() {f_in.setPath("");}
	void unsetOutputFile() {f_out.setPath("");}
	void unsetErrorFile() {f_err.setPath("");}
	PIString workingDirectory() const {return wd;}
	void setWorkingDirectory(const PIString & path) {wd = path;}
	void resetWorkingDirectory() {wd.clear();}
	PIByteArray readOutput() {f_out.open(PIIODevice::ReadOnly); return f_out.readAll();}
	PIByteArray readError() {f_err.open(PIIODevice::ReadOnly); return f_err.readAll();}

	PIStringList environment() {return env;}
	void clearEnvironment() {env.clear();}
	void removeEnvironmentVariable(const PIString & variable);
	void setEnvironmentVariable(const PIString & variable, const PIString & value);

	EVENT_HANDLER1(void, exec, const PIString & , program) {args.clear(); args << program; exec_();}
	EVENT_HANDLER2(void, exec, const PIString & , program, const PIString & , arg) {args.clear(); args << program << arg; exec_();}
	EVENT_HANDLER2(void, exec, const PIString & , program, const PIStringList & , args_) {args << program << args_; exec_();}
	EVENT_HANDLER(void, terminate);
	EVENT_HANDLER(bool, waitForFinish) {return waitForFinish(60000);}
	EVENT_HANDLER1(bool, waitForFinish, int, timeout_msecs) {return PIThread::waitForFinish(timeout_msecs);}

	EVENT1(execStarted, PIString, program)
	EVENT2(execFinished, PIString, program, int, exit_code)

	static void execIndependent(const PIString & program) {execIndependent(program, PIStringList());}
	static void execIndependent(const PIString & program, const PIString & arg) {execIndependent(program, PIStringList() << arg);}
	static void execIndependent(const PIString & program, const PIStringList & args_);

	static PIStringList currentEnvironment();
	static int currentPID();

private:
	virtual void run();
	void exec_();
	void startProc(bool detached);

	PIStringList args, env;
	PIString wd;
	PIByteArray out;
	PIFile f_in, f_out, f_err;
	bool g_in, g_out, g_err, t_in, t_out, t_err;
#ifdef WINDOWS
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
#else
	pid_t pid;
#endif
	FILE * tf_in, * tf_out, * tf_err;
	int exit_code, sz;
	bool is_exec;

};

#endif // PIPROCESS_H
