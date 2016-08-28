/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *
 *-----------------------------------------------------------------------------
 */

#include "kbhit.h"
#ifndef WIN32

void
_init_keyboard ()
{
  tcgetattr (0, &initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 0;
  tcsetattr (0, TCSANOW, &new_settings);
}

void
_close_keyboard ()
{
  tcsetattr (0, TCSANOW, &initial_settings);
}

int
_kbhit ()
{
  unsigned char ch;
  int nread;


  if (peek_character != -1)
    return 1;
  new_settings.c_cc[VMIN] = 0;
  tcsetattr (0, TCSANOW, &new_settings);
  nread = read (0, &ch, 1);
  new_settings.c_cc[VMIN] = 1;
  tcsetattr (0, TCSANOW, &new_settings);
  if (nread == 1)
    {
      peek_character = ch;
      return 1;
    }
  return 0;
}

int
_getch ()
{
  char ch;

  if (peek_character != -1)
    {
      ch = peek_character;
      peek_character = -1;
      return ch;
    }
  read (0, &ch, 1);
  return ch;
}
#endif



char
TimedInput (int t)
{
  char output = NULL;
  time_t s = time (0);

#ifndef WIN32
  _init_keyboard ();
#endif // WIN32

  while ((int) (time (0) - s) < t)
    {
      if (_kbhit ())
	{
	  output = (char) _getch ();

#ifndef WIN32
	  _close_keyboard ();
#endif // WIN32

	  return output;
	}
    }

#ifndef WIN32
  _close_keyboard ();
#endif // WIN32
  return output;

}
