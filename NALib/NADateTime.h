
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_DATETIME_INCLUDED
#define NA_DATETIME_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAString.h"


// Puts the current thread on hold for the specified amount of time.
// S = Seconds, M = Milliseconds, U = Microseconds.
NA_IAPI int naSleepU(NAInt usecs);
NA_IAPI int naSleepM(NAInt msecs);
NA_IAPI int naSleepS(NAInt  secs);

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef TIME_ZONE_INFORMATION NATimeZone;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  typedef struct timezone NATimeZone;
#endif

// If not defined, the Asc and Bin formats will use 00:00 and wintertime as the
// default timezone setting.
typedef enum{
  NA_DATETIME_FORMAT_APACHE,                   // 01/Apr/234567:22:37:51 -0130
  NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT,  // 234567-04-01T22:37:51-01:30
  NA_DATETIME_FORMAT_CONDENSEDDATE,            // 2345670401
  NA_DATETIME_FORMAT_NATURAL                   // 234567-04-01 22:37:51
} NAAscDateTimeFormat;

typedef enum{
  NA_DATETIME_FORMAT_ICC_PROFILE
} NABinDateTimeFormat;


#define NA_SECONDS_PER_MINUTE                (60LL)
#define NA_MINUTES_PER_HOUR                  (60LL)
#define NA_SECONDS_PER_HOUR                  (NA_SECONDS_PER_MINUTE * NA_MINUTES_PER_HOUR)
#define NA_HOURS_PER_DAY                     (24LL)
#define NA_SECONDS_PER_DAY                   (NA_SECONDS_PER_HOUR * NA_HOURS_PER_DAY)
#define NA_MONTHS_PER_YEAR                   (12LL)
#define NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR (NA_SECONDS_PER_DAY * (31LL+28LL+31LL+30LL+31LL+30LL))
#define NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR   (NA_SECONDS_PER_DAY * (31LL+29LL+31LL+30LL+31LL+30LL))
#define NA_SECONDS_JUL_TO_DEC                (NA_SECONDS_PER_DAY * (31LL+31LL+30LL+31LL+30LL+31LL))
#define NA_SECONDS_IN_NORMAL_YEAR            (NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_LEAP_YEAR              (NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR   + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_4_YEAR_PERIOD          (NA_SECONDS_IN_LEAP_YEAR + 3LL * NA_SECONDS_IN_NORMAL_YEAR)
#define NA_SECONDS_IN_100_YEAR_PERIOD        (76LL * NA_SECONDS_IN_NORMAL_YEAR + 24LL * NA_SECONDS_IN_LEAP_YEAR)
#define NA_SECONDS_IN_400_YEAR_PERIOD        (4LL * NA_SECONDS_IN_100_YEAR_PERIOD + NA_SECONDS_PER_DAY)

// The macros above evaluate to the following numbers:
// NA_SECONDS_PER_HOUR:                        3600
// NA_SECONDS_PER_DAY:                        86400
// NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR:   15638400
// NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR:     15724800
// NA_SECONDS_JUL_TO_DEC:                  15897600
// NA_SECONDS_IN_NORMAL_YEAR:              31536000
// NA_SECONDS_IN_LEAP_YEAR:                31622400
// NA_SECONDS_IN_4_YEAR_PERIOD:           126230400
// NA_SECONDS_IN_100_YEAR_PERIOD:        3155673600
// NA_SECONDS_IN_400_YEAR_PERIOD:       12622780800


#define NA_DATETIME_FLAG_SUMMERTIME 0x0001


typedef struct{
  int64  year;   // year number in astronomic numbering (neg, 0 and pos)
  int32  mon;    // month number in [0, 11].   Beware the 0-index!
  int32  day;    // day number in [0, 30].     Beware the 0-index!
  int32  hour;   // hour number in [0, 23]
  int32  min;    // minute number in [0, 59]
  int32  sec;    // second number in [0, 69] (may contain leap seconds)
  int32  nsec;   // Nanosecond number in [0, 999999999]
  int16  shift;  // time shift in minutes (positive or negative)
  uint16 flags;  // Various flags.
} NADateTimeStruct;

typedef struct{
  int32  yearsign;     // either +1 or -1
  int32  dayofyear;    // days since start of year, 0-indexed (Beware 1582!)
  NABool isleapyear;   // true if current year is a leap year
  int32  daysinmonth;  // number of days in this month (beware October 1582!)
  int32  weekday;      // 0 = Monday, 1 = Tuesday, ... 6 = Sunday
  int32  shiftsign;    // either +1 or -1
  int32  shifthour;    // positive shift hour number in [0, 14]
  int32  shiftmin;     // positive shift minute number in [0, 59]
} NADateTimeAttribute;



// Returns the month number (0-indexed) of an english month abbreviation.
// For example: "Nov" -> 10
int32 naGetMonthNumberWithEnglishAbbreviation(const NAString* str);

// Returns true if the given year number is a leap year.
NABool naIsLeapYearJulian      (int64 year);
NABool naIsLeapYearGregorian   (int64 year);
// The following function returns the julian information for years <= 1582 and
// the gregorian information for years > 1582.
NABool naIsLeapYear            (int64 year);



// Note that this type is not opaque.
typedef struct NADateTime NADateTime;
struct NADateTime{
  int64  sisec;  // SI-second number starting Jan 1st 1958, 00:00 + 00:00
  int32  nsec;   // nanosecond number in range [0, 999999999]
  int16  shift;  // time shift in minutes
  uint16 flags;  // Various flags.
};


// Returns an NADateTime struct with the current system clock including the
// systems timezone.
// Want to implement a nanosecond-timer? This is the function for you. Together
// with naGetDateTimeDiff.
NADateTime naMakeDateTimeNow();

// Create a new NADateTime struct with the values provided.
NADateTime naMakeDateTimeWithDateTimeStruct(const NADateTimeStruct* dts);

// Creates a new NADateTime struct with the given values. This function is a
// convenience function which is easy to use and implement.
//
// The mon and day values are expected 1-indexed (1 is January or the first
// day of the month respectively). This is in contrast to the NADateTimeStruct
// where mon and day always are expressed 0-indexed. The timezone is set to
// the global setting, see below. If you want to set the timezone manually or
// provide any additional information, use the constructor with the
// NADateTimeStruct as an argument instead, it is much more powerful.
NADateTime naMakeDateTimeWithValues(  NADateTime* datetime,
                                            int64 year,
                                            int32 mon,
                                            int32 day,
                                            int32 hour,
                                            int32 min,
                                            int32 sec);


// Creates a new NADateTime struct from a given string with a given format.
NADateTime naCreateDateTimeFromString(  NADateTime* datetime,
                                    const NAString* string,
                                NAAscDateTimeFormat format);

// Creates a new NADateTime struct from a given data block with a given format.
NADateTime naCreateDateTimeFromPointer(  NADateTime* datetime,
                                         const void* data,
                                 NABinDateTimeFormat format);


//// While the init methods read values of a given format, the output methods
//// write the values in the given format.
//void output(Byte* data, NABinDateTimeFormat format) const;
// todo

// Returns a string in the given format;
NAString* naCreateStringWithDateTime(NAString* string,
                             const NADateTime* datetime,
                           NAAscDateTimeFormat format);

// Converts between the systems time formats and NADateTime
struct tm       naMakeTMfromDateTime      (const NADateTime* datetime);
// Computes the time shift in minutes including summer time, if applicable.
int16           naMakeShiftFromTimeZone   (const NATimeZone* timezn);
// if timezn is a Null-Pointer, the global timezone settings are used.

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  NADateTime      naMakeDateTimeFromFileTime(const FILETIME* filetime,
                                             const NATimeZone* timezn);
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  struct timespec naMakeTimeSpecFromDateTime(const NADateTime* datetime);
  struct timeval  naMakeTimeValFromDateTime (const NADateTime* datetime);
  NATimeZone      naMakeTimeZoneFromDateTime(const NADateTime* datetime);
  NADateTime      naMakeDateTimeFromTimeSpec(const struct timespec* timesp,
                                             const NATimeZone* timezn);
  NADateTime      naMakeDateTimeFromTimeVal (const struct timeval* timevl,
                                             const NATimeZone* timezn);
#endif


// Fills the given struct with date and time values, splitted into all of the
// components of the given datetime. Fills an NADateTimeAttribute with further
// information about this date if desired. The attribute can be NULL.
void naExtractDateTimeInformation(const NADateTime* datetime,
                                  NADateTimeStruct* dts,
                               NADateTimeAttribute* dta);
// Same thing but expressed in timezone 00:00 wintertime
void naExtractDateTimeUTCInformation(const NADateTime* datetime,
                                     NADateTimeStruct* dts,
                                  NADateTimeAttribute* dta);

// Alters timezone shift and summertime flag WITHOUT changing the real clock.
// For example: 2011-04-01T22:37:51+06:00
// becomes      2011-04-02T06:07:51-01:30
void naSetDateTimeZone( NADateTime* datetime,
                              int16 newshift,
                             NABool summertime);
// Alters timezone shift and summertime flag WITH changing the real clock.
// For example: 2011-04-01T22:37:51+06:00
// becomes      2011-04-01T22:37:51-01:30
// Note that if you are uncertain whether to use setZone or correctZone, this
// method is probably the wrong one. This method is only available for
// DateTimes which accidentally were stored with an incorrect time shift.
void naCorrectDateTimeZone( NADateTime* datetime,
                                  int16 newshift,
                                 NABool summertime);

// Returns the difference in time. The returned value is in seconds.
double naGetDateTimeDiff(const NADateTime* end, const NADateTime* start);
// Adds the given difference to the datetime.
void naAddDateTimeDifference(NADateTime* datetime, double difference);

// Returns a human readable string of a second counter (including decimals)
// For example: naCreateStringFromSeconds(12345678.12345678, 6)
// creates the string "142d 21:21:18.123456"
NAString* naCreateStringFromSecondDifference(NAString* string,
                                                double difference,
                                                 uint8 decimaldigits);

// Returns NA_TRUE if the date has summertime.
NABool naHasDateTimeSummerTime(const NADateTime* datetime);
void naSetDateTimeSummertime(NADateTime* datetime, NABool summertime);


// Global timezone settings
// The NADateTime.c file defines a global timezone shift and a daylight
// saving time flag. These global settings are automatically used for
// constructors, input- and output methods where no explicit or implicit
// information about the shift is available.
//
// Warning: These global settings are initialized to +00:00 wintertime on
// program start!
//
// Use the following methods to manipulate the global settings:
//
// Sets a custom time shift and daylight saving flag.
void naSetGlobalTimeShift(int16 shiftminutes, NABool summertime);
// Sets the time shift and daylight saving flag to the system settings of the
// local machine. 
void naSetGlobalTimeShiftToSystemSettings();






// ////////////////////////////
// LEAP SECOND CORRECTION
//
// Leap second information is published only up to a certain date. When you
// store dates of future events for which the leap seconds are not know yet,
// you must correct the dates when the leap second information has updated.
//
// To do so, you just store a specific identifier together with your data: The
// number of the first uncertain second of this library.
int64 naGetFirstUncertainSecondNumber();

// When the library updates, you can check if the new library has a new
// uncertain second number. You do this by giving the uncertain second number
// which you stored with your data to the following function:
NAInt naGetLeapSecondCorrectionConstant(int64 olduncertainsecondnumber);
// The return value of this function is a number needed for an internal
// structure not visible to the programmer. A value >= 0 means, that a
// correction of your data might be required. If the value is < 0, no
// correction is necessary or can/will be made. The following macros depict
// further information about the negative constant:
#define NO_CORRECTION_NEEDED              -1
#define INVALID_UNCERTAIN_SECOND_NUMBER   -2
#define NEW_LIBRARY_IS_OLDER_THAN_BEFORE  -3

// To correct you data, go through all dates and send them to the following
// function together with the akquired constant from above. This function will
// correct dates only if needed or possible.
void naCorrectDateTimeForLeapSeconds(NADateTime* datetime,
                                           NAInt leapsecondcorrectionconstant);

// When all your data is converted to the new library and you want to store
// the corrected dates, don't forget to store the NEW uncertain second number
// with your data! Otherwise it will be corrected again in the next run!

























// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
#endif


NA_IAPI int naSleepU(NAInt usecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(usecs/1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(usecs));
  #endif
}

NA_IAPI int naSleepM(NAInt msecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(msecs));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(msecs*1000LL));
  #endif
}

NA_IAPI int naSleepS(NAInt secs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(secs*1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(secs*1000000LL));
  #endif
}



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_DATETIME_INCLUDED


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
