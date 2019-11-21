#include  "stdafx.h"
#include "Windows.h"
#include "iostream"
using namespace std;

class SpinLock
{
	volatile LONG m_state;

public:
	SpinLock()
	{
		m_state = true;
	}

public:
	void Acquire()
	{
		while (InterlockedExchange(&m_state, false) == false)
		{
			//YieldProcessor(/*__nop();*/);
			//Sleep(0);
			//SwitchToThread();
		}
	}

	void Release()
	{
		InterlockedExchange(&m_state, true);
	}

};

LONGLONG g_llSharedValue=0;

DWORD WINAPI SpinLockProc(PVOID pParam)
{
	SpinLock* pLock = (SpinLock*)pParam;
	DWORD dwThrId = GetCurrentThreadId();

	try
	{
		pLock->Acquire();
		g_llSharedValue++;
		cout << " SubT_After \t " << dwThrId << " \t : " << g_llSharedValue << endl;
			pLock->Release();

		Sleep(1000);
	}
	catch (HRESULT hr)
	{
		printf(" !!!! thread %d stopped by error: %08x\n", dwThrId,hr);
	}

	return 0;
}

#define MAX_THR_CNT 50

void _tmain()
{
	SpinLock lock;

	HANDLE arhThrs[MAX_THR_CNT];
	for (int i = 0; i < MAX_THR_CNT; i++)
	{
		DWORD dwTheaID = 0;
		arhThrs[i] = CreateThread(NULL, 0, SpinLockProc, &lock, 0, &dwTheaID);
	}

	getchar();

	for (int i = 0; i < MAX_THR_CNT; i++)
		CloseHandle(arhThrs[i]);

	cout << "=== Simulationtermiated ====== " << endl;
	
}