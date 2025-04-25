#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool createSchedule(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftCount,
    size_t day,
    size_t slot
);

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

    size_t dayCounter = avail.size();
    size_t workerCounter = avail[0].size();

    sched.resize(dayCounter);
    std::vector<int> shiftCount(workerCounter, 0);

    return createSchedule(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);

}

bool createSchedule(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftCount,
    size_t day,
    size_t slot
)
{

    // Base case: at the end of the matrix 
    if(day == avail.size()){
        return true;
    }

    else{
        // Need to loop through each worker
        for(Worker_T worker = 0; worker < avail[day].size(); ++worker){
            // Need to check for 3 things
            bool workerAvailable = avail[day][worker] == 1;
            bool workerHasShiftsRemaining = shiftCount[worker] < maxShifts;
            bool workerNotAlreadyScheduled = std::find(sched[day].begin(), sched[day].end(), worker) == sched[day].end();
            
            // Adding to the schedule & recursing if the 3 conditions are met
            if(workerAvailable && workerHasShiftsRemaining && workerNotAlreadyScheduled){
                sched[day].push_back(worker);
                shiftCount[worker]++;

                // Now we recurse
                if(slot + 1 < dailyNeed){
                    if(createSchedule(avail, dailyNeed, maxShifts, sched, shiftCount, day, slot + 1)){
                        return true;
                    }
                }
                else{
                    if(createSchedule(avail, dailyNeed, maxShifts, sched, shiftCount, day + 1, 0)){
                        return true;
                    }
                }

                // Backtracking
                sched[day].pop_back();
                shiftCount[worker]--;
            }
        }
        return false;
    }

}

