#define WIN32_LEAN_AND_MEAN 

#pragma warning(disable : 4996)

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <WinSock2.h>
#include <time.h>

#if !defined(_Wp64)
#define DWORD_PTR DWORD
#define LONG_PTR LONG
#define INT_PTR INT
#endif


typedef struct donutBin
{
	int donutsInBin;
	int donutsSold;
	int MaxDonuts;
	int which;

	HANDLE SemFilledSpots;
	HANDLE SemEmptySpots;
	HANDLE MutexLockFront;

}donutBin_rec;

typedef struct donutLine
{
	int front;
	int back;
	int count;
	int which;

	int line[6];

	HANDLE MutexlockLine;
}donutLine_rec;

typedef struct bakerDefine
{
	BOOL bakerAlive;
	int which;
	int donutsToBake;
	int bakedDonuts;
	int timeToBake;

	int typeBaked[4];

	HANDLE hBaker;
}bakerDefine_rec;

typedef struct lineManagerDefine
{
	HANDLE donutAvailableEvent;
	HANDLE workDoneEvent;
	HANDLE managerAliveMutex;
	BOOL managerAlive;
	HANDLE hManager;
}lineManagerDefine_rec;

typedef struct workerDefine
{
	HANDLE workerGoEvent;
	HANDLE hWorker;
	int typeSold[4];
	int totalDonutSold;
}workerDefine_rec;

void WINAPI baker(int who);

void WINAPI worker(int who);

void WINAPI manager(int who);

HANDLE bakerAliveMutex;
HANDLE hcurDonuts;
HANDLE MutexTotalSold;

int bakerAliveCount = 2;
int managerAliveCount = 4;
int binSize = 5;
int prevCount;
int curDonutsInBin = 0;
int TotalSold = 0;

bakerDefine_rec bakerArr[2];
donutBin_rec binArr[4];
lineManagerDefine_rec managerArr[4];
donutLine_rec lineArr[4];
workerDefine_rec workerArr[6];

int _tmain(int argc, LPTSTR argv[])
{
	bakerAliveMutex = CreateMutex(NULL, FALSE, NULL);
	hcurDonuts = CreateMutex(NULL, FALSE, NULL);
	MutexTotalSold = CreateMutex(NULL, FALSE, NULL);
	
	printf("Enter the amount of donuts to bake for baker #1: ");
	scanf("%d", &bakerArr[0].donutsToBake);

	printf("Enter the amount of donuts to bake for baker #2: ");
	scanf("%d", &bakerArr[1].donutsToBake);

	printf("Enter the amount of time it takes to bake 1 donut (Milliseconds): ");
	scanf("%d", &bakerArr[0].timeToBake);
	bakerArr[1].timeToBake = bakerArr[0].timeToBake;
	printf("\n");
	
	// Set up first baker
	bakerArr[0].which = 0;
	

	// Set up second baker
	bakerArr[1].which = 1;


	// Spawn 2 bakers
	bakerArr[0].hBaker = (HANDLE)_beginthreadex(NULL, 0, baker, 0, NULL, NULL);
	bakerArr[1].hBaker = (HANDLE)_beginthreadex(NULL, 0, baker, 1, NULL, NULL);


	// Set up 4 donut bins
	for (int i = 0; i < 4; i++)
	{
		binArr[i].MaxDonuts = binSize;
		binArr[i].SemEmptySpots = CreateSemaphore(NULL, binSize, binSize, NULL);
		binArr[i].SemFilledSpots = CreateSemaphore(NULL, 0, binSize, NULL);
		binArr[i].MutexLockFront = CreateMutex(NULL, FALSE, NULL);
		binArr[i].which = i;
	}

	// Set up 4 donut lines
	for (int i = 0; i < 4; i++)
	{
		lineArr[i].MutexlockLine = CreateMutex(NULL, FALSE, NULL);

	}

	// Set up 4 line managers
	for (int i = 0; i < 4; i++)
	{
		
		managerArr[i].donutAvailableEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// CreateEvent(Security,True -> door stays open, initial state of door, name)
		managerArr[i].managerAliveMutex = CreateMutex(NULL, FALSE, NULL);
		managerArr[i].managerAlive = TRUE;
		managerArr[i].hManager = (HANDLE)_beginthreadex(NULL, 0, manager, i, NULL, NULL);
	
	}

	// Set up 6 workers
	for (int i = 0; i < 6; i++)
	{
		workerArr[i].workerGoEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		workerArr[i].hWorker = (HANDLE)_beginthreadex(NULL, 0, worker, i, NULL, NULL);
	}

	WaitForSingleObject(bakerArr[0].hBaker, INFINITE);
	WaitForSingleObject(bakerArr[1].hBaker, INFINITE);
	WaitForSingleObject(managerArr[0].hManager, INFINITE);
	WaitForSingleObject(managerArr[1].hManager, INFINITE);
	WaitForSingleObject(managerArr[2].hManager, INFINITE);
	WaitForSingleObject(managerArr[3].hManager, INFINITE);
	WaitForSingleObject(workerArr[0].hWorker, INFINITE);
	WaitForSingleObject(workerArr[1].hWorker, INFINITE);
	WaitForSingleObject(workerArr[2].hWorker, INFINITE);
	WaitForSingleObject(workerArr[3].hWorker, INFINITE);
	WaitForSingleObject(workerArr[4].hWorker, INFINITE);
	WaitForSingleObject(workerArr[5].hWorker, INFINITE);

	for (int i = 0; i < 2; i++)
	{
		printf("Baker #%d\n", i+1);
		printf("Made %d donuts\n", bakerArr[i].donutsToBake);
		for (int x = 0; x < 4; x++)
		{
			printf("%d type #%d\n", bakerArr[i].typeBaked[x], x+1);
		}
		printf("\n");
	}

	for (int i = 0; i < 4; i++)
	{
		printf("Bin #%d\n", i + 1);
		printf("%d donuts\n", binArr[i].donutsSold);
		printf("\n");
	}

	for (int i = 0; i < 6; i++)
	{
		printf("Worker #%d\n", i + 1);
		printf("Sold %d donuts\n", workerArr[i].totalDonutSold);
		printf("\n");
		for (int x = 0; x < 4; x++)
		{
			printf("%d type #%d\n", workerArr[i].typeSold[x], x + 1);
		}
		printf("\n");
	}
	
	system("pause");
	return 0;
}

void WINAPI baker(int who)
{
	
	srand(&who);

	//printf("%d",who);
	while (bakerArr[who].bakedDonuts < bakerArr[who].donutsToBake)
	{
		int randomDonut = rand() % 4;

		// Check if there's an open spot for random donut
		if (!WaitForSingleObject(binArr[randomDonut].SemEmptySpots, 10))
		{
			// Bake donut
			Sleep(bakerArr[who].timeToBake);
			// Lock down the bin of the random donut
			WaitForSingleObject(binArr[randomDonut].MutexLockFront, INFINITE);

			binArr[randomDonut].donutsInBin++;

			// Unlock the bin of the random donut
			ReleaseMutex(binArr[randomDonut].MutexLockFront);

			// Release donuts in bin
			ReleaseSemaphore(binArr[randomDonut].SemFilledSpots, 1, &prevCount);

			// Update your donut counts for report (Bakers data)
			bakerArr[who].bakedDonuts++;	// Total donuts baked
			bakerArr[who].typeBaked[randomDonut]++;	// Each donuts baked

			// lock down total donuts in all bins
			WaitForSingleObject(hcurDonuts, INFINITE);
			curDonutsInBin++;
			ReleaseMutex(hcurDonuts);

			// Signal Line Manager of that donut: Donut Available Event
			SetEvent(managerArr[randomDonut].donutAvailableEvent);
		}
	}

	// Lock Baker Alive Count (Mutex)
	WaitForSingleObject(bakerAliveMutex, INFINITE);

	bakerAliveCount--;

	if (bakerAliveCount == 0)
	{
		// Signal ALL line managers donut available (Event)
		for (int i = 0; i < 4; i++)
		{
			SetEvent(managerArr[i].donutAvailableEvent);
		}
	}

	ReleaseMutex(bakerAliveMutex);

	return 0;
}

void WINAPI worker(int who)
{
	srand(&who);
	int randomDonut;
	while (managerAliveCount > 0)
	{
	

		randomDonut = rand() % 4;

		if (managerArr[randomDonut].managerAlive == TRUE)
		{
			// Add worker to the random donut queue
			WaitForSingleObject(lineArr[randomDonut].MutexlockLine, INFINITE);
			if (managerArr[randomDonut].managerAlive == TRUE)
			{
				lineArr[randomDonut].line[lineArr[randomDonut].back] = who;
				lineArr[randomDonut].back = (lineArr[randomDonut].back + 1) % 6;
				lineArr[randomDonut].count++;
				// Wait for worker go event
				ReleaseMutex(lineArr[randomDonut].MutexlockLine);
				WaitForSingleObject(workerArr[who].workerGoEvent, INFINITE);

				WaitForSingleObject(binArr[randomDonut].MutexLockFront, INFINITE);
				if (binArr[randomDonut].donutsInBin > 0)
				{
					if (!WaitForSingleObject(binArr[randomDonut].SemFilledSpots, INFINITE))
					{
						// lock bin
						
						binArr[randomDonut].donutsInBin--;
						binArr[randomDonut].donutsSold++;
						ReleaseMutex(binArr[randomDonut].MutexLockFront);

						// Update worker data
						workerArr[who].typeSold[randomDonut]++;

						WaitForSingleObject(MutexTotalSold, INFINITE);
						TotalSold++;
						ReleaseMutex(MutexTotalSold);

						// Release EmptySpotInBin (counting Semaphore)
						ReleaseSemaphore(binArr[randomDonut].SemEmptySpots, 1, &prevCount);

						// Signal Line Manager that work is done (event)
						SetEvent(managerArr[randomDonut].workDoneEvent);
					}
				}
				else
				{
					ReleaseMutex(binArr[randomDonut].MutexLockFront);
				}
			}
			else
			{
				SetEvent(managerArr[randomDonut].workDoneEvent);
				ReleaseMutex(lineArr[randomDonut].MutexlockLine);
			}
			

		
		}

		
	}

	workerArr[who].totalDonutSold = workerArr[who].typeSold[0] + workerArr[who].typeSold[1] + workerArr[who].typeSold[2] + workerArr[who].typeSold[3];
	return;
}

void WINAPI manager(int who)
{
	while (bakerAliveCount > 0 || binArr[who].donutsInBin > 0)
	{
		// Wait for Donut Available (Event)
		WaitForSingleObject(managerArr[who].donutAvailableEvent, INFINITE);
		
		// If worker is in line
		if (lineArr[who].count > 0)
		{
			// Dequeue worker and lock line
			WaitForSingleObject(lineArr[who].MutexlockLine, INFINITE);
			int curWorker = lineArr[who].line[lineArr[who].front];
			lineArr[who].line[lineArr[who].front] = -1;
			lineArr[who].count--;
			lineArr[who].front = (lineArr[who].front+1)%6;
			ReleaseMutex(lineArr[who].MutexlockLine);

			//Signal worker to go (event)
			SetEvent(workerArr[curWorker].workerGoEvent);

			// Wait for worker done event
			WaitForSingleObject(managerArr[who].workDoneEvent, INFINITE);

			//If this manager's bin still has donuts
			if (binArr[who].donutsInBin > 0)
			{
				// Signal event donut available
				SetEvent(managerArr[who].donutAvailableEvent);
			}

		}
		else
		{
			SetEvent(managerArr[who].donutAvailableEvent);
		}
	}
	// Lock line manager alive count
	WaitForSingleObject(managerArr[who].managerAliveMutex, INFINITE);
	managerAliveCount--;
	managerArr[who].managerAlive = FALSE;
	ReleaseMutex(managerArr[who].managerAliveMutex);

	// While line is not empty
	WaitForSingleObject(lineArr[who].MutexlockLine, INFINITE);
	while (lineArr[who].count > 0)
	{
		// Dequeue worker and signal worker to go event

		int curWorker = lineArr[who].line[lineArr[who].front];
		lineArr[who].count--;
		lineArr[who].front = (lineArr[who].front + 1) % 6;
		SetEvent(workerArr[curWorker].workerGoEvent);
		
	}
	ReleaseMutex(lineArr[who].MutexlockLine);

	// Manager goes home
	return;
}