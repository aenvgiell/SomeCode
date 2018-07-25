# SomeCode

GetCPUUsage.h 	  Get CPU Usage with PDH by WinEggDrop


UpTime.c 	        Get System UP Time with PDH by WinEggDrop

GetAllCPUUsage.c 	Get CPU Usage with NtQuerySystemInformation FIXED!!! (>=Win7 Working)


使用PDH获取系统启动时间或CPU使用率，兼容性有问题，

\\System\\System Up Time

\\Processor(_Total)\\% Processor Time

在老外的系统上(巴西/墨西哥/西班牙/...)，这些字符串会变的。

CPU

//PDH

GetSystemTimes();

NtQuerySystemInformation();


UPTime

//PDH

//GetTickCount();

QueryPerformanceFrequency() + QueryPerformanceCounter()
