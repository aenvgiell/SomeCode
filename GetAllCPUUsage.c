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
	LARGE_INTEGER  IdleTime;                    //CPU����ʱ�䣻
	LARGE_INTEGER  ReadTransferCount;           //I/O��������Ŀ��
	LARGE_INTEGER  WriteTransferCount;          //I/Oд������Ŀ��
	LARGE_INTEGER  OtherTransferCount;          //I/O����������Ŀ��
	ULONG          ReadOperationCount;          //I/O��������Ŀ��
	ULONG          WriteOperationCount;         //I/Oд������Ŀ��
	ULONG          OtherOperationCount;         //I/O��������������Ŀ��
	ULONG          AvailablePages;              //�ɻ�õ�ҳ��Ŀ��
	ULONG          TotalCommittedPages;         //�ܹ��ύҳ��Ŀ��
	ULONG          TotalCommitLimit;            //���ύҳ��Ŀ��
	ULONG          PeakCommitment;              //ҳ�ύ��ֵ��
	ULONG          PageFaults;                  //ҳ������Ŀ��
	ULONG          WriteCopyFaults;             //Copy-On-Write������Ŀ��
	ULONG          TransitionFaults;            //��ҳ������Ŀ��
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //����0��������
	ULONG          PagesRead;                   //��ҳ��Ŀ��
	ULONG          PageReadIos;                 //��ҳI/O��������
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //��дҳ�ļ�ҳ����
	ULONG          PagefilePageWriteIos;        //��дҳ�ļ���������
	ULONG          MappedFilePagesWritten;      //��дӳ���ļ�ҳ����
	ULONG          MappedFileWriteIos;          //��дӳ���ļ���������
	ULONG          PagedPoolUsage;              //��ҳ��ʹ�ã�
	ULONG          NonPagedPoolUsage;           //�Ƿ�ҳ��ʹ�ã�
	ULONG          PagedPoolAllocs;             //��ҳ�ط��������
	ULONG          PagedPoolFrees;              //��ҳ���ͷ������
	ULONG          NonPagedPoolAllocs;          //�Ƿ�ҳ�ط��������
	ULONG          NonPagedPoolFress;           //�Ƿ�ҳ���ͷ������
	ULONG          TotalFreeSystemPtes;         //ϵͳҳ�����ͷ�������
	ULONG          SystemCodePage;              //����ϵͳ����ҳ����
	ULONG          TotalSystemDriverPages;      //�ɷ�ҳ��������ҳ����
	ULONG          TotalSystemCodePages;        //����ϵͳ����ҳ������
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                   
	ULONG          MmSystemCachePage;          //ϵͳ����ҳ����
	ULONG          PagedPoolPage;              //��ҳ��ҳ����
	ULONG          SystemDriverPage;           //�ɷ�ҳ����ҳ����
	ULONG          FastReadNoWait;             //�첽���ٶ���Ŀ��
	ULONG          FastReadWait;               //ͬ�����ٶ���Ŀ��
	ULONG          FastReadResourceMiss;       //���ٶ���Դ��ͻ����
	ULONG          FastReadNotPossible;        //���ٶ�ʧ������
	ULONG          FastMdlReadNoWait;          //�첽MDL���ٶ���Ŀ��
	ULONG          FastMdlReadWait;            //ͬ��MDL���ٶ���Ŀ��
	ULONG          FastMdlReadResourceMiss;    //MDL����Դ��ͻ����
	ULONG          FastMdlReadNotPossible;     //MDL��ʧ������
	ULONG          MapDataNoWait;              //�첽ӳ�����ݴ�����
	ULONG          MapDataWait;                //ͬ��ӳ�����ݴ�����
	ULONG          MapDataNoWaitMiss;          //�첽ӳ�����ݳ�ͻ������
	ULONG          MapDataWaitMiss;            //ͬ��ӳ�����ݳ�ͻ������
	ULONG          PinMappedDataCount;         //ǣ��ӳ��������Ŀ��
	ULONG          PinReadNoWait;              //ǣ���첽����Ŀ��
	ULONG          PinReadWait;                //ǣ��ͬ������Ŀ��
	ULONG          PinReadNoWaitMiss;          //ǣ���첽����ͻ��Ŀ��
	ULONG          PinReadWaitMiss;            //ǣ��ͬ������ͻ��Ŀ��
	ULONG          CopyReadNoWait;             //�첽������������
	ULONG          CopyReadWait;               //ͬ��������������
	ULONG          CopyReadNoWaitMiss;         //�첽���������ϴ�����
	ULONG          CopyReadWaitMiss;           //ͬ�����������ϴ�����
	ULONG          MdlReadNoWait;              //�첽MDL��������
	ULONG          MdlReadWait;                //ͬ��MDL��������
	ULONG          MdlReadNoWaitMiss;          //�첽MDL�����ϴ�����
	ULONG          MdlReadWaitMiss;            //ͬ��MDL�����ϴ�����
	ULONG          ReadAheadIos;               //��ǰ��������Ŀ��
	ULONG          LazyWriteIos;               //LAZYд������Ŀ��
	ULONG          LazyWritePages;             //LAZYдҳ�ļ���Ŀ��
	ULONG          DataFlushes;                //����ˢ�´�����
	ULONG          DataPages;                  //����ˢ��ҳ����
	ULONG          ContextSwitches;            //�����л���Ŀ��
	ULONG          FirstLevelTbFills;          //��һ�㻺������������
	ULONG          SecondLevelTbFills;         //�ڶ��㻺������������
	ULONG          SystemCall;                 //ϵͳ���ô�����
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
