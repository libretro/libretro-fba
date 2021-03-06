// FB Alpha watchdog device module

#include "burnint.h"

static INT32 (*WatchdogReset)(INT32 clear_mem) = NULL;
static INT32 WatchdogFrames = 0;
static INT32 Watchdog = 0;
static INT32 WatchdogEnable = 0;

void BurnWatchogWrite()
{
	Watchdog = 0;
	WatchdogEnable = 1;
}

UINT8 BurnWatchdogRead()
{
	Watchdog = 0;
	WatchdogEnable = 1;

	return 0;
}

void BurnWatchdogReset()
{
	Watchdog = 0;
	WatchdogEnable = 0;
}

void BurnWatchdogInit(INT32 (*reset)(INT32 clear_mem), INT32 frames)
{
	if (reset == NULL) {
#if defined FBA_DEBUG
		bprintf (PRINT_ERROR, _T("Error: BurnWatchdogInit called with no reset!\n"));
#endif
		return;
	}

	if (frames == 0) {
#if defined FBA_DEBUG
		bprintf (PRINT_ERROR, _T("Error: BurnWatchdogInit called with 0 frames (%d)!\n"), frames);
#endif
		frames = 180; // default
	}

	WatchdogReset = reset;
	WatchdogFrames = frames;
}

void BurnWatchdogUpdate()
{
	if (WatchdogFrames == -1) {
		return;
	}

	if (WatchdogEnable) {
		Watchdog++;
	}

	if (Watchdog >= WatchdogFrames) {
		if (WatchdogReset != NULL) {
			WatchdogReset(0);
#if defined FBA_DEBUG
			bprintf (0, _T("BurnWatchdogUpdate - Watchdog triggered!\n"));
#endif
			
		}
	}

	Watchdog = 0;
}

INT32 BurnWatchdogScan(INT32 nAction)
{
	if (nAction & ACB_VOLATILE)
	{
		SCAN_VAR(WatchdogEnable);
		SCAN_VAR(Watchdog);
	}

	return 0;
}
