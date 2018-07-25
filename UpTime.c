//**************************************************************************** 
// Version: V1.0 
// Coder: WinEggDrop 
// Date Release: NULL 
// Purpose: To View System UP Time 
// Test PlatForm: Win 2K Pro And Server SP4 
// Compiled On: VC++ 6.0 
// Others: 
//        Using Performance Counter To Get The System UP Time.The 
//        GetTickCount() API Only Returns DWORD Value(After 49.3 Days, 
//        DWORD Valud Will Encounter An Overflow.Two Approach For Performance 
//        Counter Coding.One Is The Performance Counter API.The Other Is 
//        Registry API.Performance Counter API Is Far More Easy Than Registry 
//        Approach In Many Aspects.For More Information,Read MSDN About The 
//        Performance Counter 
//**************************************************************************** 
#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <tchar.h> 
#include <pdh.h> 

#pragma comment(lib,"pdh.lib") 

// Function ProtoType Declaration 
//------------------------------------------------------------------------------------------------------ 
BOOL GetSystemUptime(UINT *EslapsedDay,UINT *EslapsedHour,UINT *EslapsedMin); 
//------------------------------------------------------------------------------------------------------ 
// End Of Fucntion ProtoType Declaration 

int main () 
{ 
UINT EslapsedDay  = 0; 
UINT EslapsedHour = 0 ; 
UINT EslapsedMin  = 0; 
if (GetSystemUptime(&EslapsedDay,&EslapsedHour,&EslapsedMin))        // We Get The UP Time Successfully 
{ 
     printf("UpTime: %d Day %d Hour %d Minute\n",EslapsedDay,EslapsedHour,EslapsedMin);        // Display It 
} 
return 0; 
}// End Of Main() 


//------------------------------------------------------------------------------------ 
// Purpose: To Get The System UP Time 
// Return Type: BOOLEAN 
// Parameters:  
//            Out: UINT *EslapsedDay    --> Store The System UP Day 
//            Out: UINT *EslapsedHour   --> Store The System UP Hour 
//            Out: UINT *EslapsedMin    --> Store The System UP Minute 
//------------------------------------------------------------------------------------ 
BOOL GetSystemUptime(UINT *EslapsedDay,UINT *EslapsedHour,UINT *EslapsedMin) 
{ 
HQUERY          hQuery; 
char            *szPathBuffer  = "\\System\\System Up Time"; 
HCOUNTER        hCounter; 
PDH_FMT_COUNTERVALUE   fmtValue; 
DWORD           ctrType; 
BOOL            Flag = FALSE; 

if (PdhOpenQuery(0, 0, &hQuery) != ERROR_SUCCESS)        // Open The Performance Counter Query 
{ 
     return FALSE;        // If Fail,Then Run False 
} 

if (PdhAddCounter(hQuery,szPathBuffer, 0, &hCounter) != ERROR_SUCCESS)        // Add Performance Counter To Get System UP Time 
{ 
       goto CleanUP;        // If Fail,Then Clean The Performance Counter Handle And Return 
} 

if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS)        // Get The Data 
{ 
     goto CleanUP;                // If Fail,Then Clean The Performance Counter Handle And Return 
} 

if (PdhGetFormattedCounterValue(hCounter,PDH_FMT_LARGE,&ctrType,&fmtValue) != ERROR_SUCCESS)        // Convert The Data To 64 Bits Integet 
{ 
     goto CleanUP;        // If Fail,Then Clean The Performance Counter Handle And Return        
} 

// Some Calculation Below 
*EslapsedDay  = (UINT)(fmtValue.largeValue / 86400); 
*EslapsedHour = (UINT)((fmtValue.largeValue % 86400) / 3600); 
*EslapsedMin  = (UINT)(fmtValue.largeValue % 86400); 
*EslapsedMin  = (*EslapsedMin % 3600) / 60; 
Flag = TRUE; 

CleanUP:        // Close Counter Handle And Return To The Caller 
PdhCloseQuery (hQuery); 
return Flag; 
}// End Of GetSystemUptime 
// End Of File
