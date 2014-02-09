// ------------------------------------------------------------------------------------------------
// ----------------- Curuba Device ----------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Copyright (C) 2013 Mathieu B�langer (mathieu.b.belanger@usherbrooke.ca)
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this Program, or any covered work, by linking or combining it
// with "CC3000 Host Driver Implementation" (or a modified version of that
// library), containing parts covered by the terms of Texas Instruments
// Incorporated's BSD style license, the licensors of this Program grant you
// additional permission to convey the resulting work. {Corresponding Source
// for a non-source form of such a combination shall include the source code
// for the parts of "CC3000 Host Driver Implementation" used as well as that
// of the covered work.}
// ------------------------------------------------------------------------------------------------
#include "timer.h"

#include "util.h"
#include "eventManager.h"
#include "deviceInfoState.h"

#define NUMBER_OF_TIMER 5
#define DELAY5SEC						(40)
#define DELAY2SEC						(20)

/* Private Function */
Timer* selectTimer(Event event);
/* END Private Function */

//This implementation have been chosen because of it's simplicity
Timer listTimer[NUMBER_OF_TIMER];

void initTimer()
{
	static const Timer heartbeat = {0,0,EVENT_HEARTBEAT_TIMEOUT,0};
	static const Timer infoRequest = {0,0,EVENT_INFOREQUEST_TIMEOUT,0};
	static const Timer ping = {0,0,EVENT_PING_TIMEOUT,0};
	static const Timer socket = {0,0,EVENT_SOCKET_CONNECTION_TIMEOUT,0};
	static const Timer wifi = {DELAY5SEC,0,EVENT_WIFI_CONNECTION_TIMEOUT,0};

	listTimer[0] = heartbeat;
	listTimer[1] = infoRequest;
	listTimer[2] = ping;
	listTimer[3] = socket;
	listTimer[4] = wifi;
}

void startTimer(Event event)
{
	Timer* ptrListTimer = selectTimer(event);

	if (ptrListTimer != 0)
	{
		ptrListTimer->enabled = 1;
		ptrListTimer->startTime = getDeviceInfoState()->timeCounter;
	}
}

void stopTimer(Event event)
{
	Timer* ptrListTimer = selectTimer(event);

	if (ptrListTimer != 0)
	{
		ptrListTimer->enabled = 0;
	}
}

Timer* selectTimer(Event event)
{
	Timer* ptrListTimer = 0;

	switch (event)
	{
	case EVENT_HEARTBEAT_TIMEOUT:
		ptrListTimer = listTimer;
		break;
	case EVENT_INFOREQUEST_TIMEOUT:
		ptrListTimer = listTimer + 1;
		break;
	case EVENT_PING_TIMEOUT:
		ptrListTimer = listTimer + 2;
		break;
	case EVENT_SOCKET_CONNECTION_TIMEOUT:
		ptrListTimer = listTimer + 3;
		break;
	case EVENT_WIFI_CONNECTION_TIMEOUT:
		ptrListTimer = listTimer + 4;
		break;
	}

	return ptrListTimer;
}

void timerOut()
{
	Timer* ptrListTimer  = listTimer;
	unsigned int index;
	for (index = NUMBER_OF_TIMER; index--;  )
	{
		if ( ptrListTimer->enabled && (getTimeElapsed(ptrListTimer->startTime, getDeviceInfoState()->timeCounter) >= ptrListTimer->timerLimit) )
		{
			notify(ptrListTimer->event);
		}
		ptrListTimer++;
	}
}
