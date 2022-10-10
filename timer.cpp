// ===================================================================
// $Id$
//
// timer.cpp
//     Timing for running process
//
// Licence: the use and distribution of this file is severely limited, please
// see the file 'doc/Licence.txt'. Any non-authorized use can be prosecuted under
// International Law.
// REPLACEMENT_STRING
//
// Initial coding by Vlastimil Havran, 1998.
// Windows time measuring added by Jaroslav Krivanek, October 2003.

// GOLEM headers
#include "timer.h"



// standard headers
#include <ctime>


#ifndef _MSC_VER
//#ifdef __UNIX__

#include <sys/time.h>
#include <sys/resource.h>

#endif // __UNIX__

#ifdef _MSC_VER
#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>
#endif // _MSC_VER

#include <chrono>

// Static variable
bool
Timer::initTimingCalled = false;

// The following are not static's in the CTimer class to prevent
// #including <windows.h> into the "timer.h" header.
#ifdef  _MSC_VER
/// true if the performance timer works under windows
static BOOL hasHRTimer;
/// frequency of the performance timer under windows
static LARGE_INTEGER hrFreq;
#endif // MSC_VER

void
Timer::_initTiming()
{
#ifdef  _MSC_VER
  hasHRTimer = QueryPerformanceFrequency(&hrFreq);
#endif
  initTimingCalled = true;
}

void
Timer::Reset()
{
  // Real times in seconds
  auto epoch = std::chrono::time_point<std::chrono::system_clock>{};
  auto now   = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = now - epoch;
  lastRealTime = diff.count();

  lastUserTime   = 0.0;
  lastSystemTime = 0.0;

  realTime   = 0.0;
  userTime   = 0.0;
  systemTime = 0.0;

  countStop = 0;
  running = false;

#ifdef __UNIX__
  // Timing in OS UNIX
  begrusage.ru_utime.tv_usec = begrusage.ru_utime.tv_sec =
  begrusage.ru_stime.tv_usec = begrusage.ru_stime.tv_sec = 0L;
  endrusage = begrusage;
#endif // __UNIX__
}

void
Timer::_start() const
{
  if (running)
    return; // timer is already running

  // Measure the real time
  auto epoch = std::chrono::time_point<std::chrono::system_clock>{};
  auto now   = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = now - epoch;
  lastRealTime = diff.count();

  // Measure the real and system time
#ifdef __UNIX__
  // Measure under UNIX
  struct rusage begrusage;
  getrusage(RUSAGE_SELF, &begrusage);

  lastUserTime =
    (double)begrusage.ru_utime.tv_sec +
    1e-6 * begrusage.ru_utime.tv_usec;

  lastSystemTime =
    (double)begrusage.ru_stime.tv_sec +
    1e-6 * begrusage.ru_stime.tv_usec;
#endif // __UNIX__

#ifdef  _MSC_VER
  // Mesure under Windows
  if (hasHRTimer) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    lastUserTime = (double)counter.QuadPart / (double)hrFreq.QuadPart;
    lastSystemTime = 0;
  }
  else {
    static struct _timeb mtime;
    _ftime(&mtime);
    lastUserTime   = (double)mtime.time + 1e-3 * mtime.millitm;
    lastSystemTime = 0;
  }
#endif
  // Begin trace.
  running = true;
}

void
Timer::_stop() const
{
  if (!running)
    return; // timer is not running

  // Begin trace.
  auto epoch = std::chrono::time_point<std::chrono::system_clock>{};
  auto now   = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = now - epoch;
  realTime += diff.count() - lastRealTime;

#ifdef __UNIX__
  // timing in unix OS
  getrusage(RUSAGE_SELF, &endrusage);

  userTime += (double)endrusage.ru_utime.tv_sec +
    1e-6 * endrusage.ru_utime.tv_usec -
    lastUserTime;

  systemTime += (double)endrusage.ru_stime.tv_sec +
    1e-6 * endrusage.ru_stime.tv_usec -
    lastSystemTime;
#endif // __UNIX__

#ifdef  _MSC_VER
  // Mesure under Windows
  if (hasHRTimer) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    userTime += (double)counter.QuadPart / (double)hrFreq.QuadPart -
      lastUserTime;
    systemTime = 0;
   }
  else {
    static struct _timeb mtime;
    _ftime(&mtime);
    userTime   += (double)mtime.time + 1e-3 * mtime.millitm -
      lastUserTime;
    systemTime = 0;
  }
#endif

  running = false;
  countStop++;
}

// returns the real time measured by timer in seconds
double
Timer::RealTime() const
{
  if (running) {
    _stop();
    _start();
  }
  return realTime;
}

// returns the user time measured by timer in seconds
double
Timer::UserTime() const
{
  if (running) {
    _stop();
    _start();
  }
  return userTime;
}

// returns the user+system time measured by timer in seconds
double
Timer::SystemTime() const
{
  if (running) {
    _stop();
    _start();
  }
  return systemTime;
}
