#pragma once
#include <Windows.h>

extern HANDLE moboThread;

// CPU Cores
extern HANDLE core0Thread;
extern HANDLE core1Thread;
extern HANDLE core2Thread;
extern HANDLE core3Thread;
extern HANDLE core4Thread;
extern HANDLE core5Thread;
extern HANDLE core6Thread;
extern HANDLE core7Thread;

// Components
extern HANDLE usbAThread;
extern HANDLE usbCThread;
extern HANDLE blue0Thread1;
extern HANDLE blue0Thread2;
extern HANDLE blue1Thread1;
extern HANDLE blue1Thread2;

extern HANDLE rtcThread;
extern HANDLE pitThread1;
extern HANDLE pitThread2;
extern HANDLE pitThread3;
extern HANDLE pitThread4;

extern HANDLE storageThreadIn;
extern HANDLE storageThreadOut;
extern HANDLE gpuThreadIn;
extern HANDLE gpuThreadOut;
extern HANDLE audioThreadIn;
extern HANDLE audioThreadOut;

LPTHREAD_START_ROUTINE moboThreadFunc();

const LPTHREAD_START_ROUTINE mobo = moboThreadFunc();