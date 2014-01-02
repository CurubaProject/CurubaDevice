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
#ifdef __TESTDEBUG__

#include "interuptDeviceControl.h"

Callback _callback_readCurrent = 0;
Callback _callback_hearbeatTimer = 0;
Callback _callback_dimmerTimer = 0;
Callback _callback_ledTimer = 0;

void initReadCurrent(Callback callback_readCurrent)
{
	_callback_readCurrent = callback_readCurrent;
}

void initTIMER0(void){}

void initHearbeatTimer(Callback callback_hearbeatTimer)
{
	_callback_hearbeatTimer = callback_hearbeatTimer;
}

void initDimmerTimer(Callback callback_dimmerTimer)
{
	_callback_dimmerTimer = callback_dimmerTimer;
}

void initLedTimer(Callback callback_ledTimer)
{
	_callback_ledTimer = callback_ledTimer;
}

#endif
