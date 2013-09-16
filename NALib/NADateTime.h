
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_NA_DATETIME_DEFINED
#define NA_NA_DATETIME_DEFINED

#include "NASystem.h"
#include "NAString.h"


// Puts the current thread on hold for the specified amount of time.
// S = Seconds, M = Milliseconds, U = Microseconds.
static int naSleepU(NAInt usecs);
static int naSleepM(NAInt msecs);
static int naSleepS(NAInt  secs);


// If not defined, the Asc and Bin formats will use 00:00 and wintertime as the
// default timezone setting.
typedef enum{
  NA_DATETIME_FORMAT_APACHE,                   // 01/Apr/234567:22:37:51 -0130
//  NA_DATETIME_FORMAT_PM_MEASUREMENT,           // "01/04/234567:22:37"
//  NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT,  // 234567-04-01T22:37:51-01:30
//  NA_DATETIME_FORMAT_CONDENSEDDATE,            // 2345670401
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
  int64 year;   // year number in astronomic numbering (neg, 0 and pos)
  int32 mon;    // month number in [0, 11].   Beware the 0-index!
  int32 day;    // day number in [0, 30].     Beware the 0-index!
  int32 hour;   // hour number in [0, 23]
  int32 min;    // minute number in [0, 59]
  int32 sec;    // second number in [0, 69] (may contain leap seconds)
  int16 shift;  // time shift in minutes (positive or negative)
  uint16 flags; // Various flags.
} NADateTimeStruct;

typedef struct{
  int32  yearsign;     // either +1 or -1
  int32  dayofyear;    // days since start of year, 0-indexed (Beware 1582!)
  NABool isleapyear;   // true if current year is a leap year
  int32  daysinmonth;  // number of days in this month (beware October 1582!)
  int32  weekday;      // 0 = Monday, 1 = Tuesday, ... 6 = Sunday
  int32  shiftsign;    // either +1 or -1
  int32  shifthour;    // positive shift hour number in [0, 12]
  int32  shiftmin;     // positive shift minute number in [0, 59]
} DateTimeAttribute;



//// Returns the month number (0-indexed) of an english month abbreviation.
//// For example: "Nov" -> 10
int32 naGetMonthNumberFromEnglishAbbreviation(const NAString* str);

// Takes the current system clock and stores it in the given datetimestruct.
static void naFillDateTimeStructWithNow(NADateTimeStruct* datetimestruct);

// Returns true if the given year number is a leap year.
static NABool naIsLeapYearJulian      (int64 year);
static NABool naIsLeapYearGregorian   (int64 year);




typedef struct NADateTime NADateTime;
struct NADateTime{
  int64  sisecs; // The number of SI-seconds since Jan 1st 1958, 00:00 + 00:00
  uint32 nsecs;  // nanoseconds in range [0, 999999999]
  int16  shift;  // time shift in minutes
  uint16 flags;  // Various flags.
};



// Creates a new NADateTime struct. No initialization is performed whatsoever.
NADateTime* naCreateDateTime(NADateTime* datetime);

// Creates a new NADateTime struct with the current system clock.
NADateTime* naCreateDateTimeWithNow(NADateTime* datetime);

// Create a new NADateTime struct with the values provided.
NADateTime* naCreateDateTimeWithDateTimeStruct( NADateTime* datetime,
                                    const NADateTimeStruct* datetimestruct);

// Creates a new NADateTime struct with the given values. This function is a
// convenience function which is easy to use and implement.
//
// The mon and day values are expected 1-indexed (1 is January or the first
// day of the month respectively). This is in contrast to the NADateTimeStruct
// where mon and day always are expressed 0-indexed. The timezone is set to
// the global setting, see below. If you want to set the timezone manually or
// provide any additional information, use the constructor with the
// NADateTimeStruct as an argument instead, it is much more powerful.
NADateTime* naCreateDateTimeWithValues( NADateTime* datetime,
                                              int64 year,
                                              int32 mon,
                                              int32 day,
                                              int32 hour,
                                              int32 min,
                                              int32 sec);


// Creates a new NADateTime struct from a given string with a given format.
NADateTime* naCreateDateTimeFromString(  NADateTime* datetime,
                                     const NAString* string,
                                 NAAscDateTimeFormat format);

// Creates a new NADateTime struct from a given data block with a given format.
NADateTime* naCreateDateTimeFromPointer(  NADateTime* datetime,
                                          const void* data,
                                  NABinDateTimeFormat format);

//// Copy Constructor.
//NADateTime(const NADateTime& datetime);
//
//// Create a new NADateTime object with the raw values.
//NADateTime(int64 sisecs, int16 shift, uint16 flags);
//
//
//// While the init methods read values of a given format, the output methods
//// write the values in the given format.
//void output(Byte* data, NABinDateTimeFormat format) const;
//
//// Returns a string in the given format;
//String getString(NAAscDateTimeFormat format = NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT) const;
//
//int64 getSISeconds() const;
//
//// Returns other time and date structures needed for compatibility with
//// the systems native date and time implementation.
////
//// Warning: The tm_zone entry of the tm-struct is not yet working.
////  void getSystemTM(struct tm* systemtimestruct) const;
////  void getSystemTimeSpec(struct timespec* systemtimespec) const;
//
//void setSISeconds(int64 newsecs);
//void addSISeconds(int64 addsecs);
//int16 getShift() const;
//uint16 getFlags() const;
//
//// Fills the given struct with date and time values, splitted into all of its
//// components. Returns a DateTimeAttribute with further information about
//// this date.
//DateTimeAttribute getNADateTimeStruct(NADateTimeStruct* datetimestruct) const;
//// Same thing but expressed in timezone 00:00 wintertime
//DateTimeAttribute getNADateTimeStructUTC(NADateTimeStruct* datetimestruct) const;
//
//// Alters timezone shift and summertime flag WITHOUT changing the real clock.
//// For example: 2011-04-01T22:37:51+06:00
//// becomes      2011-04-02T06:07:51-01:30
//void setZone(int16 shiftminutes, NABool summertime);
//// Alters timezone shift and summertime flag WITH changing the real clock.
//// For example: 2011-04-01T22:37:51+06:00
//// becomes      2011-04-01T22:37:51-01:30
//// Note that if you are uncertain whether to use setZone or correctZone, this
//// method is probably the wrong one. This method is only available for
//// DateTimes which accidentally were stored with an incorrect time shift.
//void correctZone(int16 shiftminutes, NABool summertime);
//
//// Returns NA_TRUE if the date has summertime.
//NABool hasSummertime() const;
//
//
//// ////////////////////////////
//// STATIC METHODS
//// ////////////////////////////
//
//// The NADateTime class stores a global timezone shift and a daylight
//// saving time flag. These global settings are automatically used for
//// constructors, input- and output methods where no explicit or implicit
//// information about the shift is available.
//
//// Sets the time shift and daylight saving flag to the settings of the
//// local machine.
//static void setGlobalTimeShiftToLocalSetting();
//// Sets a custom time shift and daylight saving flag.
//static void setGlobalTimeShift(int16 shiftminutes, NABool summertime);
//
//// Returns a human readable string of a second counter (including decimals)
//// For example: NADateTime::secondsToString(12345678.12345678, 6)
//// returns the string "142d 21:21:18.123456"
//static String secondsToString(Real realseconds, uint8 decimaldigits);
//
//
//// Returns true if the given year is a leap year. The first method will
//// return the julian information for years <= 1582 and the gregorian
//// information for years > 1582.
//static NABool isLeapYear            (int64 year);
//
//
//
//// ////////////////////////////
//// LEAP SECOND CORRECTION
//// ////////////////////////////
//
//// Leap second information is published only up to a certain date. For
//// highest accuracy, store the first uncertain second number of the current
//// library version with your data.
//static int64 getFirstUncertainSecondNumber();
//// When the library upgrades, check for that value again and if higher,
//// correct all NADateTime values using the correctLeapSeconds method. This
//// method will correct dates only if needed or possible.
//void correctLeapSeconds(int32 leapsecondcorrectionconstant);
//// The correction method needs a constant as an input to identify the last
//// valid date. Use the getLeapSecondCorrectionConstant method to get this
//// constant (once per upgrade).
//static int32 getLeapSecondCorrectionConstant(int64 olduncertainsecondnumber);
//// The return value of this method also indicates whether a correction is
//// needed or not. A value >= 0 means, that a correction is needed.
//// Otherwise, no correction will be made. The following constants are used:
//#define NO_CORRECTION_NEEDED              -1
//#define INVALID_UNCERTAIN_SECOND_NUMBER   -2
//#define NEW_LIBRARY_IS_OLDER_THAN_BEFORE  -3
//// Don't forget to store the new uncertain second number with your data after
//// you corrected all NADateTime values!
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//// ///////////////////////////////////////////////////////////////////////
//// Inline Implementations: See readme file for more expanation.
//// ///////////////////////////////////////////////////////////////////////
//
//
//
//#if NA_SYSTEM == NA_SYSTEM_WINDOWS
//#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
//  #include <unistd.h>
//#endif
//
//
//static NA_INLINE int naSleepU(NAInt usecs){
//  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
//    Sleep((DWORD)(usecs/1000));
//    return 0;
//  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
//    return usleep((useconds_t)(usecs));
//  #endif
//}
//
//static NA_INLINE int naSleepM(NAInt msecs){
//  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
//    Sleep((DWORD)(msecs));
//    return 0;
//  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
//    return usleep((useconds_t)(msecs*1000LL));
//  #endif
//}
//
//static NA_INLINE int naSleepS(NAInt secs){
//  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
//    Sleep((DWORD)(secs*1000));
//    return 0;
//  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
//    return usleep((useconds_t)(secs*1000000LL));
//  #endif
//}
//
//

#endif // NA_NA_DATETIME_DEFINED


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
