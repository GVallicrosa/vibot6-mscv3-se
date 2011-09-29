/***************************************************************************
 Stopwatch.h
 -------------------
 update               : 2003-01-28
 copyright            : (C) 2002-2003 by Micha�l ROY
 email                : michaelroy@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _STOPWATCH_
#define _STOPWATCH_

#include <iostream>
#include <iomanip>
#include <time.h>

//--
//
// Stopwatch
//
//--
// Implement a stopwatch in order to measure process time.
class Stopwatch
{

public:

  // Default constructor
  inline
  Stopwatch() :
    on(false), inter(0), total(0)
  {
  }

  // Destructor
  inline
  ~Stopwatch()
  {
  }

  // Return activity of stopwatch
  inline bool
  Running() const
  {
    return on;
  }

  // Stop stopwatch & initialize data
  inline void
  Reset()
  {
    on = false;
    inter = 0;
    total = 0;
  }

  // Active stopwatch
  inline void
  Start()
  {
    if (on == false)
      {
        last = clock();
        on = true;
      }
  }

  // Unactive stopwatch
  inline clock_t
  Stop()
  {
    if (on == true)
      {
        now = clock();
        inter = now - last;
        last = now;
        total += inter;
        on = false;
      }
    return total;
  }

  // Return total time
  inline clock_t
  GetTotal()
  {
    if (on)
      {
        now = clock();
        inter = now - last;
        last = now;
        total += inter;
      }
    return total;
  }

  // Return intermediate time
  inline clock_t
  GetInter()
  {
    if (on)
      {
        now = clock();
        inter = now - last;
        last = now;
        total += inter;
      }
    return inter;
  }

  // Print intermediate elapsed time
  inline void
  PrintInter(std::ostream& os = std::cout) const
  {
    os << inter / CLOCKS_PER_SEC << '.' << std::setw(2) << std::setfill('0')
        << (int) ((float) inter / CLOCKS_PER_SEC * 100) % 100 << std::setfill(
        ' ') << "s";
  }

  // Print total elapsed time
  inline void
  PrintTotal(std::ostream& os = std::cout) const
  {
    os << total / CLOCKS_PER_SEC << '.' << std::setw(2) << std::setfill('0')
        << (int) ((float) total / CLOCKS_PER_SEC * 100) % 100 << std::setfill(
        ' ') << "s";
  }

protected:

  bool on; // Activity
  clock_t now; // Current time
  clock_t last; // Wall clock of start watch
  clock_t inter; // Intermediate elapsed time
  clock_t total; // Total elapsed time
};

#endif // _STOPWATCH_
