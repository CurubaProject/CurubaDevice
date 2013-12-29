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
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#define EVENT_WIFI_CONNECTED 0x0001
#define EVENT_WIFI_DISCONNECTED 0x0002
#define EVENT_WIFI_CONNECTION_TIMEOUT 0x0004

#define EVENT_SOCKET_CONNECTED 0x0008
#define EVENT_SOCKET_DISCONNECTED 0x0010
#define EVENT_SOCKET_CONNECTION_TIMEOUT 0x0020

#define EVENT_HEARTBEAT_RECEIVED 0x0040
#define EVENT_HEARTBEAT_READYTOSEND 0x0080
#define EVENT_HEARTBEAT_TIMEOUT 0x0100

#define EVENT_PING_RESPONSE 0x0200
#define EVENT_PING_TIMEOUT 0x0400

#define EVENT_INFOREQUEST_RECEIVED 0x0800
#define EVENT_INFOREQUEST_TIMEOUT 0x1000

#define EVENT_PACKETS_RECEIVED 0x2000

#define EVENT_PAYLOAD_RECEIVED 0x4000
#define EVENT_PAYLOAD_TOSEND 0x8000

/* OVERWRITABLE FUNCTION*/
void do_event_wifi_connected();
void do_event_wifi_disconnected();

void do_event_socket_connected();
void do_event_socket_disconnected();

void do_event_heartbeat_received();
void do_event_hearbeat_readyToSend();

void do_event_inforequest_received();

void do_event_packetsReceived();

void do_event_payloadReceived();
void do_event_payloadToSend();
/**/

void notify(unsigned int newEvent);
void doEvent();

#endif
