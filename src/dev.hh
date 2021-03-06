//-*- mode: c++; indent-tabs-mode: t; -*-
//
// Class: dev Copyright (c) 2018 Manuel Cano
// Author: manutalcual@gmail.com
// Date: Tue Sep 18 14:33:22 2018
// Time-stamp: <>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//   02110-1301	USA
//
#ifndef dev_hh
#define dev_hh
#include "common.hh"

bool get_enum_monitors (HMONITOR mon, HDC hdc, LPRECT rect, LPARAM data);

class dev
{
public:
	dev ()
		: _monitors (0),
		  _right (0),
		  _bottom (0)
	{
		read ();
	}
	void read ()
	{
		RECT desktop;
		//_monitors = GetSystemMetrics(SM_CMONITORS);
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect (hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		_top = GetSystemMetrics(SM_XVIRTUALSCREEN); //desktop.top;
		_left = GetSystemMetrics(SM_YVIRTUALSCREEN); //desktop.left;
		_top = GetSystemMetrics(SM_CXMAXTRACK);
		_left = GetSystemMetrics(SM_CYMAXTRACK);
		_right = desktop.right;
		_bottom = desktop.bottom;
		// SM_CYVIRTUALSCREEN
		// SM_CXVIRTUALSCREEN
		_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		_monitors = 0;
#if 0
		DWORD           DispNum = 0;
		DISPLAY_DEVICE  DisplayDevice;

		DisplayDevice.cb = sizeof(DISPLAY_DEVICE);
		while (EnumDisplayDevicesA(NULL, DispNum, &DisplayDevice, 0)) {
			++DispNum;
			logp (sys::e_debug, "Display name: '" << DisplayDevice.DeviceName << "'");
		}
#endif
		enum_monitors ();
		logp (sys::e_debug, "Metrics [read]: width: " << _width
			  << ", height: " << _height
			  << ", top: " << _top
			  << ", left: " << _left
			  << ", right: " << _right
			  << ", bottom: " << _bottom
			  << ", monitors: " << _monitors);
	}
	void print ()
	{
		logp (sys::e_debug, "Metrics [print]: width: " << _width
			  << ", height: " << _height
			  << ", right: " << _right
			  << ", bottom: " << _bottom
			  << ", monitors: " << _monitors);
	}
	size_t width ()
	{
		return _width;
	}
	size_t height ()
	{
		return _height;
	}
	size_t monitors ()
	{
		return _monitors;
	}
	void operator = (const dev & d)
	{
		_monitors = d._monitors;
		_width = d._width;
		_height = d._height;
		_right = d._right;
		_bottom = d._bottom;
	}
	bool operator == (const dev & d)
	{
		return _monitors == d._monitors;
	}
	bool operator != (const dev & d)
	{
		return _monitors != d._monitors;
	}
 	bool operator < (const dev & d)
	{
		return _monitors < d._monitors;
	}
 	bool operator <= (const dev & d)
	{
		return _monitors <= d._monitors;
	}
 	bool operator > (const dev & d)
	{
		return _monitors > d._monitors and (_width > d._width or _height > d._height);
	}
 	bool operator >= (const dev & d)
	{
		return _monitors >= d._monitors;
	}
	void enum_monitors ()
	{
		EnumDisplayMonitors(
			NULL,
			NULL,
			(MONITORENUMPROC)get_enum_monitors,
			(LPARAM) this
			);
	}
	void add_monitor_count ()
	{
		++_monitors;
	}
	void update_monitors (HMONITOR hmon)
	{
		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hmon, &mi);
		logp (sys::e_debug, "Monitor data: "
			  << mi.rcWork.top << ", "
			  << mi.rcWork.left << ", "
			  << mi.rcWork.right << ", "
			  << mi.rcWork.bottom);
		if (mi.rcWork.left < _left)
			_left = mi.rcWork.left;
		if (mi.rcWork.top < _top)
			_top = mi.rcWork.top;
		if (mi.rcWork.right > _right)
			_right = mi.rcWork.right;
		if (mi.rcWork.bottom > _bottom)
			_bottom = mi.rcWork.bottom;
	}
//private:
	int _monitors;
	int _top;
	int _left;
	int _width;
	int _height;
	int _right;
	int _bottom;

};


#endif // dev_hh
