/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MLog.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2012 Skaiware
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#ifndef _M_LOG
#define _M_LOG

#include <ostream>
#include <iostream>
#include <sstream>
#include <fstream>
// to do : add syslog support

#ifdef __CYGWIN__
//#include <syslog.h>
#endif

/* Log function compatible with the syslog standard, usefull for filtering logs, and logging networkly (todo).
	0	Emergency	emerg (panic)	System is unusable.	A "panic" condition usually affecting multiple apps/servers/sites. At this level it would usually notify all tech staff on call.
	1	Alert	alert	Action must be taken immediately.	Should be corrected immediately, therefore notify staff who can fix the problem. An example would be the loss of a primary ISP connection.
	2	Critical	crit	Critical conditions.	Should be corrected immediately, but indicates failure in a primary system, an example is a loss of a backup ISP connection.
	3	Error	err (error)	Error conditions.	Non-urgent failures, these should be relayed to developers or admins; each item must be resolved within a given time.
	4	Warning	warning (warn)	Warning conditions.	Warning messages, not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full - each item must be resolved within a given time.
	5	Notice	notice	Normal but significant condition.	Events that are unusual but not error conditions - might be summarized in an email to developers or admins to spot potential problems - no immediate action required.
	6	Informational	info	Informational messages.	Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required.
	7	Debug	debug	Debug-level messages.	Info useful to developers for debugging the application, not useful during operations.
*/
// For the moment let s just simply log if the message has a severity lower than the env variable
// to do : add __FUNCTION__ or __func__, __line__, __file__...
#define MLOG(severity, USERMESSAGE) { MLog::m_stringstream.str(std::string("")); MLog::m_stringstream<<USERMESSAGE; MLog::m_string=MLog::m_stringstream.str(); MLog::log(severity, __FUNCTION__, __FILE__, __LINE__); }

class M_ENGINE_EXPORT MLog
{
	static MLog* m_instance; // singleton
	static std::fstream m_logfstream;
	static int m_desired_loglevel;
	// private constructor
	MLog();
	~MLog();
	public:
		static std::stringstream m_stringstream;
		static std::string m_string;
		static void log(int severity, const char* func, const char* fil, const int &line_no);
};

#endif
