/* Copyright (C) 2026 tonybanters (tony@tonybtw.com)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   The GNU General Public License is contained in the file LICENSE.
*/

#ifndef SCENIC_SLICE_H
#define SCENIC_SLICE_H

#include <stddef.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#define SLICE_FROM_ARRAY(a) { .data = (a), .len = ARRAY_LEN(a) }
#define SLICE_EMPTY { .data = NULL, .len = 0 }

#endif
