#include <PDH.h>

// CPU π”√¬ 
LONG GetCPUUsage(void)
{
	HQUERY          hQuery;
	char            CPUUsage[]  = "\\Processor(_Total)\\% Processor Time";
	HCOUNTER        hCounter;
	PDH_FMT_COUNTERVALUE  fmtValue;
	DWORD          ctrType;

	if (PdhOpenQuery(0, 0, &hQuery) == ERROR_SUCCESS)
	{
		if (PdhAddCounter(hQuery, CPUUsage, 0, &hCounter) == ERROR_SUCCESS)
		{
			if (PdhCollectQueryData(hQuery) == ERROR_SUCCESS)
			{
				Sleep(100);
				if (PdhCollectQueryData(hQuery) == ERROR_SUCCESS)
				{
					if (PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, &ctrType, &fmtValue) == ERROR_SUCCESS)
					{
						PdhCloseQuery(hQuery);
						return fmtValue.longValue;
					}
				}
			}
		}
		PdhCloseQuery(hQuery);
	}

	return -1;
}
