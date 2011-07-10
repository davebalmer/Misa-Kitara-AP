
#ifndef __CONTROLASSIGNMENTS_H__
#define __CONTROLASSIGNMENTS_H__

#define CONTROLASSIGNMENTS_SUCCESS 0
#define CONTROLASSIGNMENTS_TOOMANY 1
#define CONTROLASSIGNMENTS_SAMEPAR 2
#define CONTROLASSIGNMENTS_UNKNOWN 3

typedef struct __CONTROLASSIGNMENTS_INFO
{
	U8 temp;
} CONTROLASSIGNMENTS_INFO,*PCONTROLASSIGNMENTS_INFO;

typedef U8 (*COMMITINFORMATION)(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end);

U8 CreateControlAssignmentsScreen(void* pPara);
U8 DeleteControlAssignmentsScreen();
U8 TopControlAssignmentsScreen(WM_HWIN hPreWin);
U8 UpdateControlAssignmentsInfo(PCONTROLASSIGNMENTS_INFO pInfo);
U8 GetControlAssignmentsInfo(PCONTROLASSIGNMENTS_INFO pInfo);

U8 ControlAssignmentsAddItem(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end);
U8 ControlAssignmentsEditItem(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end);
U8 ControlAssignmentsDelItem(int pos);
U8 ControlAssignmentsDelAllItem();
U8 CommitInfomation(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end);

#endif // __CONTROLASSIGNMENTS_H__

