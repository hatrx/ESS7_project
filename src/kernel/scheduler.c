#include <stm32f4xx_hal.h>
#include <stdbool.h>

#include "kernel/scheduler.h"

#include "../xml_data.h"

#include "arinc/statics.h"
#include "arinc/window.h"
#include "arinc/queuing_port.h"

uint8_t indexActivePartition = 0;

static uint16_t timings1[4] = {1000, 1000, 1000, 3000};
window_t schedule[MAX_NUM_WINDOWS];
static uint32_t timings2 = 0;

void scheduler_partitionScheduler(void)
{
	const uint32_t nb_partitions = sizeof(partitions) / sizeof(partition_t);
	if(timings2 <= HAL_GetTick()){
		indexActivePartition = (indexActivePartition + 1) % nb_partitions;
		timings2 = HAL_GetTick() + timings1[indexActivePartition];

		curr_partition_id = partitions[indexActivePartition].IDENTIFIER;
	}
}

process_t* scheduler_processScheduler(partition_t *part)
{
	/* Set state of currently running process to READY. */
	process_t *curr_process = &part->processes[part->index_running_process];
	curr_process->PROCESS_STATE = READY;

	/* Get list of processes in partition. */
	process_t *processes = part->processes;

	/* Find a READY partition as default candidate for execution. */
	process_t *priority_process;
	for (size_t i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++) {
		if (processes[i].PROCESS_STATE == READY) {
			priority_process = &processes[i];
		}
	}

	/* Attempt to find a better candidate by comparing to default. */
	for (size_t i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++) {
		if (processes[i].PROCESS_STATE != READY)
			continue;

		if (processes[i].CURRENT_PRIORITY < priority_process->CURRENT_PRIORITY)
			continue;

		if (processes[i].tickStamp < priority_process->tickStamp) {
			priority_process = &processes[i];
			part->index_running_process = i;
		}
	}

	/* Set state of process to RUNNING. */
	priority_process->PROCESS_STATE = RUNNING;

	/* Return pointer to highest priority candidate process */
	return priority_process;
}

void scheduler_buildSchedule(void)
{
	bool doubleBreak = false;
	int i, j, windowIndex = 0, numTotalWindows = 0, numGeneratedWindows = 0;
	float desiredStartTime = 0.0, earliestNextStartTime = 50000;
	partition_schedule* tmpPSchedule;
	window_schedule* tmpWSchedule;
	partition_t* tmpPartition;

	// The goal: Create an ordered list of windows. Windows should be ordered
	// by the time inside a frame in which they run.
	// The problem: Our source data (from the XML) is not ordered.
	// The solution: Search through all the partitions and all the windows to
	// find the next. When we have the next, insert it into the list. 
	// We know the start time and the duration of all windows. With this it 
	// should be easy to find the next.
	// If we cannot find a window that start at
	// (window-1).startTime + (window-1).duration, we assume it's an implicit
	// idle, and we generate an idle window for that.

	// Count the total number of windows first
	for(i = 0; i < sizeof(partition_schedules) / sizeof(partition_schedule); i++ )
	{
		for(j = 0; j < sizeof(tmpPSchedule->window_arr) / sizeof(window_schedule); j++)
		{
			numTotalWindows++;
		}
	}

	while(numGeneratedWindows < numTotalWindows)
	{
		// Reset pointer
		tmpWSchedule = NULL;

		// Iterate through all partition schedules
		for(i = 0, doubleBreak = false; i < sizeof(partition_schedules) / sizeof(partition_schedule); i++ )
		{
			if(doubleBreak == true) break;
			tmpPSchedule = &partition_schedules[i];

			// Iterate through all windows in the partition
			for(j = 0; j < sizeof(tmpPSchedule->window_arr) / sizeof(window_schedule); j++)
			{
				if(tmpPSchedule->window_arr[j].windowstartseconds == desiredStartTime)
				{
					tmpWSchedule = &tmpPSchedule->window_arr[j];
					doubleBreak = true;
					break;
				}
				else
				{
					if(earliestNextStartTime < tmpPSchedule->window_arr[j].windowstartseconds)
					{
						earliestNextStartTime = tmpPSchedule->window_arr[j].windowstartseconds;
					}
				}
			}
		}

		if(tmpWSchedule != NULL) // A matching window was found
		{
			// Find the partition by ID
			tmpPartition = NULL;
			for(i = 0; i < sizeof(partitions) / sizeof(partition_t); i++)
			{
				if (partitions[i].IDENTIFIER == tmpPSchedule->partitionidentifier)
				{
					tmpPartition = &partitions[i];
					break;
				}
			}

			// Create a window
			schedule[windowIndex++] = (window_t) {
				.partition = tmpPartition,
				.frameStartTime = desiredStartTime,
				.duration = tmpWSchedule->windowdurationseconds,
			};
			desiredStartTime = desiredStartTime + tmpWSchedule->windowdurationseconds;
		}
		else // Generate an idle window
		{
			schedule[windowIndex++] = (window_t) {
				.partition = &partitions[0],
				.frameStartTime = desiredStartTime,
				.duration = earliestNextStartTime - desiredStartTime,
			};
			desiredStartTime = earliestNextStartTime;
			earliestNextStartTime = 50000;
			numTotalWindows++;
		}

		numGeneratedWindows++;
	}
}