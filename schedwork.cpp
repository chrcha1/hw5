#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

#include "schedwork.h"

using namespace std;

static const Worker_T INVALID_WORKER = (unsigned int)-1;

// checking if worker assignment is valid
bool isWorkerValid(const AvailabilityMatrix& availability, const size_t dailyNeed, const size_t maxShifts, DailySchedule& schedule, int day, Worker_T worker, vector<int>& workerShifts)
{
    int duplicateCount = 0;
    for (int i = 0; i < dailyNeed; i++)
    {
        if (schedule[day][i] == worker) duplicateCount++;
        if (duplicateCount > 1) 
					return false;
    }

    if ((workerShifts[worker] > maxShifts) || !availability[day][worker] ) 
			return false;
    else 
			return true;
}

// Recursive helper function for scheduling
bool scheduleWorkers(const AvailabilityMatrix& availability, const size_t dailyNeed, const size_t maxShifts, DailySchedule& schedule, int day, int workerColumn, vector<int>& workerShifts)
{
    size_t workerCount = availability[0].size();
    if (day >= availability.size())
      return true;
    else
    {
        for (int worker = 0; worker < workerCount; worker++)
        {
            vector<Worker_T> daySchedule = schedule[day];
            if (find(daySchedule.begin(), daySchedule.end(), worker) == daySchedule.end()){
                schedule[day][workerColumn] = worker;
                workerShifts[worker]++;
                int tempWorkerColumn = workerColumn;
                int tempDay = day;
                if (isWorkerValid(availability, dailyNeed, maxShifts, schedule, day, worker, workerShifts))
                {
                    tempWorkerColumn++;
                    if (tempWorkerColumn >= dailyNeed)
                    {
                        tempDay++;
                        tempWorkerColumn = 0;
                    }
                    bool status = scheduleWorkers(availability, dailyNeed, maxShifts, schedule, tempDay, tempWorkerColumn, workerShifts);

                    if (status == true) 
											return true;
                }
                schedule[day][workerColumn] = INVALID_WORKER;
                workerShifts[worker]--;
            }
        }
        return false;
    }
}

// main function for scheduling
bool schedule(const AvailabilityMatrix& availability, const size_t dailyNeed, const size_t maxShifts, DailySchedule& schedule)
{
    if(availability.size() == 0){
        return false;
    }
    schedule.clear();

    vector<int> workerShifts(availability[0].size());
    std::fill(workerShifts.begin(), workerShifts.end(), 0);
    for (int day = 0; day < availability.size(); day++)
    {
        vector<Worker_T> dailySchedule(dailyNeed);
        std::fill(dailySchedule.begin(), dailySchedule.end(), INVALID_WORKER);
        schedule.push_back(dailySchedule);
    }

    return scheduleWorkers(availability, dailyNeed, maxShifts, schedule, 0, 0, workerShifts);
}
