#pragma once
#ifndef JC_LOCK_H
#define JC_LOCK_H 201901251017
#include <windows.h>	//CRITICAL_SECTION

class JCCriticalSection
{
private:
	CRITICAL_SECTION _lock;
public:
	JCCriticalSection()
	{
		InitializeCriticalSection(&_lock);
	}
	~JCCriticalSection()
	{
		DeleteCriticalSection(&_lock);
	}

	void Lock()
	{
		EnterCriticalSection(&_lock);
	}
	void Unlock()
	{
		LeaveCriticalSection(&_lock);
	}
};

class JSScoppedLock
{
private:
	JCCriticalSection& _cs;
public:
	JSScoppedLock(JCCriticalSection& cs) : _cs(cs)
	{
		_cs.Lock();
	}
	~JSScoppedLock()
	{
		_cs.Unlock();
	}
};
#endif
