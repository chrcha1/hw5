#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#include "schedwork.h"

using namespace std;

#endif

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, int m, int d, int k,int n, int i, int j, DailySchedule& sched);
bool isValid(const AvailabilityMatrix& avail, unsigned int worker, int m, int n, int d, DailySchedule& sched);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // initializing  sched matrix w INVALID_ID vals
    for(size_t i = 0; i < avail.size(); i++)
    {
        vector<Worker_T> row(dailyNeed, INVALID_ID);
        sched.push_back(row);
    }

    int n = avail.size();
    int k = avail[0].size();

    return scheduleHelper(avail, maxShifts, dailyNeed, k, n, 0, 0, sched);
}

// recursively schedules workers
bool scheduleHelper(const AvailabilityMatrix& avail, int m, int d, int k,int n, int i, int j, DailySchedule& sched)
{
    if (i == n) return true; // all days scheduled
    if (j == d) return scheduleHelper(avail, m, d, k, n, i + 1, 0, sched); // move to next day

    for (int currWorker = 0; currWorker < k; currWorker++)
    {
        sched[i][j] = (Worker_T) currWorker;
        if (isValid(avail, currWorker, m, n, d, sched))
        {
            if (scheduleHelper(avail, m, d, k, n, i, j + 1, sched)) return true;
        }
    }

    // if no valid worker found---backtrack
    sched[i][j] = INVALID_ID;
    return false;
}

// check if curr sched valid
bool isValid(const AvailabilityMatrix& avail, unsigned int worker, int m, int n, int d, DailySchedule& sched)
{
    int numShifts = 0;
    int numShiftsPerDay = 0;

    for(int i = 0; i < n; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (sched[i][j] == worker)
            {
                if (!avail[i][worker]) return false; // worker not available
                numShifts++;
                numShiftsPerDay++;
            }
        }

        if (numShiftsPerDay > 1) return false; // >1 shift per day
        numShiftsPerDay = 0;
    }

    return numShifts <= m; // are worker's shifts within limit
		//yeeee
}