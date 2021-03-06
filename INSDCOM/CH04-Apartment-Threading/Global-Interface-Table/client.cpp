 // client.cpp
#define _WIN32_DCOM
#include <conio.h>
#include <iostream>  // For cout
using namespace std;

#include "Component-with-GIT\component.h" // Generated by MIDL

void __stdcall MyThread(IStream* pScream)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if(FAILED(hr))
		cout << "CoInitializeEx failed" << endl;

	ISum* pSum;
	CoGetInterfaceAndReleaseStream(pScream, IID_ISum, (void**)&pSum);

	for(int count = 0; count < 10; count++)
	{
		int sum;
		hr = pSum->Sum(count, count, &sum);
		cout << "Client: Calling Sum(" << count << ", " << count << ") = " << sum << endl;
	}

	pSum->Release();

	CoUninitialize();
}

void main()
{
	cout << "Client: Calling CoInitialize()" << endl;
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
		cout << "CoInitializeEx failed" << endl;

	cout << "Client: Calling CoCreateInstance()" << endl;
	ISum* pSum;
	hr = CoCreateInstance(CLSID_InsideDCOM, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pSum);
	if(FAILED(hr))
		cout << "CoCreateInstance failed" << endl;

	IStream* pStream;
	CoMarshalInterThreadInterfaceInStream(IID_ISum, pSum, &pStream);

	DWORD threadid;
	HANDLE thread_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MyThread, (void*)pStream, 0, &threadid);

	int sum;
	hr = pSum->Sum(2, 3, &sum);
	if(SUCCEEDED(hr))
		cout << "Client: Calling Sum(2, 3) = " << sum << endl;

	WaitForSingleObject(thread_handle, INFINITE);

	Sleep(500); // To avoid thread console print intermix.
	cout << endl;
	cout << "Chj: == Press any key to go on doing final Release(). ==" << endl;
	cout << "     == You can take the chance to inspect this process with a debugger. ==" << endl;
	_getch();

	hr = pSum->Release();
	cout << "Client: Calling pSum->Release() reference count = " << hr << endl;

	cout << "Client: Calling CoUninitialize()" << endl;
	CoUninitialize();
}