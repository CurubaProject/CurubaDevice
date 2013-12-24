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
#ifndef COMMUN_H
#define COMMUN_H

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMERB_0 3

#define STATE_ON			(1)
#define STATE_OFF			(2)
#define STATE_NOLOAD		(3)

#define TYPE_DIMMER 1
#define TYPE_OUTLET 2

#define DEVICE_1 1
#define DEVICE_2 2

#define TRUE 	(1)
#define FALSE	(0)

#define PAYLOAD_INFO_REQUEST		(10)
#define PAYLOAD_INFO_RESPONSE		(20)
#define PAYLOAD_CONTROL_REQUEST		(30)
#define PAYLOAD_CONTROL_RESPONSE	(40)
#define PAYLOAD_CONFIG_REQUEST		(50)
#define PAYLOAD_CONFIG_RESPONSE		(60)
#define PAYLOAD_HEARTBEAT_REQUEST 	(70)
#define PAYLOAD_HEARTBEAT_RESPONSE  (80)

#define STATUS_ACTIVE 		(1)
#define STATUS_INACTIVE 	(2)
#define STATUS_OVERRIDE 	(3)

#define MAXTABADC 			(308)

#define LED_STATE_OFF                   (0)
#define LED_STATE_UNCONNECTED           (1)
#define LED_STATE_CONNECTED             (2)
#define LED_STATE_CONFIGURING           (3)

#endif
