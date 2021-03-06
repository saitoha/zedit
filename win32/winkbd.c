/* winkbd.c - keyboard input routines
 * Copyright (C) 2013 Sean MacLennan
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this project; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "z.h"
#include "keys.h"
#include "winkeys.h"

/* stack and vars for t[un]getkb / tkbrdy */
#define CSTACK 16 /* must be power of 2 */
static int cstack[CSTACK];
static int cptr = -1;
static int cpushed;	/* needed in shell.c */
static bool Pending;

static short convertKey(KEY_EVENT_RECORD *event)
{
	Byte key = virt[event->wVirtualKeyCode];
	if (event->dwControlKeyState == 0 || key == 0)
		return key;

	if (key >= 128)
		return key;

	if (event->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
		return M(toupper(key));

	if (event->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
		switch (key) {
		case ' ':
		case '\\':
			return 28;
		case ']':
			return 29;
		case '^':
			return 30;
		case '_':
		case '-':
			return 31;
		default:
			if (isalpha(key))
				return key - 'a' + 1;
			else
				return 0;
		}

	if (event->dwControlKeyState & (CAPSLOCK_ON | SHIFT_PRESSED)) {
		switch (key) {
		case '`': return '~';
		case '1': return '!';
		case '2': return '@';
		case '3': return '#';
		case '4': return '$';
		case '5': return '%';
		case '6': return '^';
		case '7': return '&';
		case '8': return '*';
		case '9': return '(';
		case '0': return ')';
		case '-': return '_';
		case '=': return '+';

		case '[': return '{';
		case ']': return '}';
		case '\\': return '|';

		case ';': return ':';
		case '\'': return '"';

		case ',': return '<';
		case '.': return '>';
		case '/': return '?';

		default:
			if (isalpha(key))
				return toupper(key);
		}
	}

	return key;
}

static _inline void do_mouse(MOUSE_EVENT_RECORD *mouse)
{
	if (mouse->dwEventFlags & MOUSE_WHEELED) {
		mouse_scroll(mouse->dwMousePosition.Y,
			     mouse->dwButtonState & 0x80000000);
		zrefresh();
	} else if (mouse->dwButtonState & 0xffff) {
		mouse_point(mouse->dwMousePosition.Y,
			    mouse->dwMousePosition.X,
			    mouse->dwButtonState & RIGHTMOST_BUTTON_PRESSED);
		zrefresh();
	}
}

int tgetcmd(void)
{
	Pending = false;

	cptr = (cptr + 1) & (CSTACK - 1);
	if (cpushed) {
		--cpushed;
		return cstack[cptr];
	}

	INPUT_RECORD input[CSTACK];
	DWORD i, n;
	int p = cptr;

again:
	if (!ReadConsoleInput(hstdin, input, CSTACK, &n))
		hang_up(1);	/* we lost connection */

	for (i = 0; i < n; ++i)
		switch (input[i].EventType) {
		case KEY_EVENT: /* 1 */
			if (input[i].Event.KeyEvent.bKeyDown == 1)
				break;
			cstack[p] = convertKey(&input[i].Event.KeyEvent);
			if (cstack[p]) {
				p = (p + 1) & (CSTACK - 1);
				++cpushed;
			}
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
			Rowmax = input[i].Event.WindowBufferSizeEvent.dwSize.Y;
			Colmax = input[i].Event.WindowBufferSizeEvent.dwSize.X;
			Zredisplay();		/* update the windows */
			zrefresh();		/* force a screen update */
			break;
		case MOUSE_EVENT:
			do_mouse(&input[i].Event.MouseEvent);
			break;
		}

	if (cpushed == 0)
		goto again; /* keep reading till we get a key */

	--cpushed;
	return cstack[cptr];
}

bool tkbrdy(void)
{
	if (cpushed || Pending)
		return true;

	return Pending = WaitForSingleObject(hstdin, 0) == WAIT_OBJECT_0;
}

bool tdelay(int ms)
{
	if (InPaw || cpushed || Pending)
		return false;

	return WaitForSingleObject(hstdin, ms) != WAIT_OBJECT_0;
}
