/*
	Get CPU Usage with NtQuerySystemInformation()

	>=Win7 Work
*/
#include <stdio.h>
#include <windows.h>

#define NTSTATUS					ULONG
#define NT_SUCCESS(Status)			((NTSTATUS)(Status) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_SUCCESS				((NTSTATUS)0x00000000L)

#define SystemBasicInformation			0
#define SystemPerformanceInformation	2
#define SystemTimeInformation			3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

typedef struct
{
    DWORD dwUnknown1;
    ULONG uKeMaximumIncrement;
    ULONG uPageSize;
    ULONG uMmNumberOfPhysicalPages;
    ULONG uMmLowestPhysicalPage;
    ULONG uMmHighestPhysicalPage;
    ULONG uAllocationGranularity;
    PVOID pLowestUserAddress;
    PVOID pMmHighestUserAddress;
    ULONG uKeActiveProcessors;
    BYTE bKeNumberProcessors;
    BYTE bUnknown2;
    WORD wUnknown3;
} SYSTEM_BASIC_INFORMATION;

// New Length: 336 Bytes
typedef struct _SYSTEM_PERFORMANCE_INFORMATION
{
	LARGE_INTEGER  IdleTime;                    //CPU空闲时间；
	LARGE_INTEGER  ReadTransferCount;           //I/O读操作数目；
	LARGE_INTEGER  WriteTransferCount;          //I/O写操作数目；
	LARGE_INTEGER  OtherTransferCount;          //I/O其他操作数目；
	ULONG          ReadOperationCount;          //I/O读数据数目；
	ULONG          WriteOperationCount;         //I/O写数据数目；
	ULONG          OtherOperationCount;         //I/O其他操作数据数目；
	ULONG          AvailablePages;              //可获得的页数目；
	ULONG          TotalCommittedPages;         //总共提交页数目；
	ULONG          TotalCommitLimit;            //已提交页数目；
	ULONG          PeakCommitment;              //页提交峰值；
	ULONG          PageFaults;                  //页故障数目；
	ULONG          WriteCopyFaults;             //Copy-On-Write故障数目；
	ULONG          TransitionFaults;            //软页故障数目；
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //需求0故障数；
	ULONG          PagesRead;                   //读页数目；
	ULONG          PageReadIos;                 //读页I/O操作数；
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //已写页文件页数；
	ULONG          PagefilePageWriteIos;        //已写页文件操作数；
	ULONG          MappedFilePagesWritten;      //已写映射文件页数；
	ULONG          MappedFileWriteIos;          //已写映射文件操作数；
	ULONG          PagedPoolUsage;              //分页池使用；
	ULONG          NonPagedPoolUsage;           //非分页池使用；
	ULONG          PagedPoolAllocs;             //分页池分配情况；
	ULONG          PagedPoolFrees;              //分页池释放情况；
	ULONG          NonPagedPoolAllocs;          //非分页池分配情况；
	ULONG          NonPagedPoolFress;           //非分页池释放情况；
	ULONG          TotalFreeSystemPtes;         //系统页表项释放总数；
	ULONG          SystemCodePage;              //操作系统代码页数；
	ULONG          TotalSystemDriverPages;      //可分页驱动程序页数；
	ULONG          TotalSystemCodePages;        //操作系统代码页总数；
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                   
	ULONG          MmSystemCachePage;          //系统缓存页数；
	ULONG          PagedPoolPage;              //分页池页数；
	ULONG          SystemDriverPage;           //可分页驱动页数；
	ULONG          FastReadNoWait;             //异步快速读数目；
	ULONG          FastReadWait;               //同步快速读数目；
	ULONG          FastReadResourceMiss;       //快速读资源冲突数；
	ULONG          FastReadNotPossible;        //快速读失败数；
	ULONG          FastMdlReadNoWait;          //异步MDL快速读数目；
	ULONG          FastMdlReadWait;            //同步MDL快速读数目；
	ULONG          FastMdlReadResourceMiss;    //MDL读资源冲突数；
	ULONG          FastMdlReadNotPossible;     //MDL读失败数；
	ULONG          MapDataNoWait;              //异步映射数据次数；
	ULONG          MapDataWait;                //同步映射数据次数；
	ULONG          MapDataNoWaitMiss;          //异步映射数据冲突次数；
	ULONG          MapDataWaitMiss;            //同步映射数据冲突次数；
	ULONG          PinMappedDataCount;         //牵制映射数据数目；
	ULONG          PinReadNoWait;              //牵制异步读数目；
	ULONG          PinReadWait;                //牵制同步读数目；
	ULONG          PinReadNoWaitMiss;          //牵制异步读冲突数目；
	ULONG          PinReadWaitMiss;            //牵制同步读冲突数目；
	ULONG          CopyReadNoWait;             //异步拷贝读次数；
	ULONG          CopyReadWait;               //同步拷贝读次数；
	ULONG          CopyReadNoWaitMiss;         //异步拷贝读故障次数；
	ULONG          CopyReadWaitMiss;           //同步拷贝读故障次数；
	ULONG          MdlReadNoWait;              //异步MDL读次数；
	ULONG          MdlReadWait;                //同步MDL读次数；
	ULONG          MdlReadNoWaitMiss;          //异步MDL读故障次数；
	ULONG          MdlReadWaitMiss;            //同步MDL读故障次数；
	ULONG          ReadAheadIos;               //向前读操作数目；
	ULONG          LazyWriteIos;               //LAZY写操作数目；
	ULONG          LazyWritePages;             //LAZY写页文件数目；
	ULONG          DataFlushes;                //缓存刷新次数；
	ULONG          DataPages;                  //缓存刷新页数；
	ULONG          ContextSwitches;            //环境切换数目；
	ULONG          FirstLevelTbFills;          //第一层缓冲区填充次数；
	ULONG          SecondLevelTbFills;         //第二层缓冲区填充次数；
	ULONG          SystemCall;                 //系统调用次数；
/*
guga: SYSTEM_PERFORMANCE_INFORMATION Structure
http://masm32.com/board/index.php?topic=3402.0
*/
	// Below are only available in Windows 8, windows web server 8 or above
	UINT64         CcTotalDirtyPages;
	UINT64         CcDirtyPageThreshold;
	INT64          ResidentAvailablePages;
} SYSTEM_PERFORMANCE_INFORMATION,*PSYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG uCurrentTimeZoneId;
    DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;
 
 
// ntdll!NtQuerySystemInformation (NT specific!)
//
// The function copies the system information of the
// specified type into a buffer
//
// NTSYSAPI
// NTSTATUS
// NTAPI
// NtQuerySystemInformation(
// IN UINT SystemInformationClass,  // information type
// OUT PVOID SystemInformation,     // pointer to buffer
// IN ULONG SystemInformationLength,// buffer size in bytes
// OUT PULONG ReturnLength OPTIONAL // pointer to a 32-bit
// // variable that receives
// // the number of bytes
// // written to the buffer
// );
typedef LONG (WINAPI *lpNtQuerySystemInformation)(UINT,PVOID,ULONG,PULONG);
lpNtQuerySystemInformation fNtQuerySystemInformation;

int GetAllCPUUsage(void)
{
	int nResult = -1;

    NTSTATUS Status;
    double dbIdleTime;
    double dbSystemTime;
    LARGE_INTEGER liOldIdleTime   = {0, 0};
    LARGE_INTEGER liOldSystemTime = {0, 0};
    SYSTEM_TIME_INFORMATION			SysTimeInfo;
    SYSTEM_BASIC_INFORMATION		SysBaseInfo;
    SYSTEM_PERFORMANCE_INFORMATION	SysPerfInfo;

	__try
	{
		fNtQuerySystemInformation = (lpNtQuerySystemInformation)GetProcAddress(GetModuleHandle("NTDLL"), "NtQuerySystemInformation");
		if (fNtQuerySystemInformation == NULL)
			__leave;

		// get number of processors in the system
		Status = fNtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL);
		if (Status != STATUS_SUCCESS)
			__leave;
 
 		while (1)
		{
			// get new system time
			Status = fNtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),NULL);
			if (Status != STATUS_SUCCESS)
				__leave;

			// get new CPU's idle time
			Status = fNtQuerySystemInformation(SystemPerformanceInformation, &SysPerfInfo, sizeof(SysPerfInfo), NULL);
			if (Status != STATUS_SUCCESS)
				__leave;

			// if it's a first call - skip it
			if (liOldIdleTime.QuadPart != 0)
			{
				// CurrentValue = NewValue - OldValue
				dbIdleTime = Li2Double(SysPerfInfo.IdleTime) - Li2Double(liOldIdleTime);
				dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);
	 
				// CurrentCpuIdle = IdleTime / SystemTime
				dbIdleTime = dbIdleTime / dbSystemTime;
	 
				// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors
				dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;
	 
	 			nResult = (UINT)dbIdleTime;
				//printf("[i] CPU Usage: %3d%%\n", (UINT)dbIdleTime);

				break;
			}
	 
			// store new CPU's idle and system time
			liOldIdleTime = SysPerfInfo.IdleTime;
			liOldSystemTime = SysTimeInfo.liKeSystemTime;
	 
			// wait one second
			Sleep(1000);
		}
	}
	__finally
	{
	}

	return nResult;
}

int main(int argc, char *argv[])
{
	while (1)
	{
		printf("[i] CPU Usage: %3d%%\n", GetAllCPUUsage());
	}

	return 0;
}
