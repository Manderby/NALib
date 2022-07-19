
#ifndef NA_DATETIME_INCLUDED
#define NA_DATETIME_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "NABase.h"
#include "NAString.h"
#include "NABuffer.h"


#if NA_OS == NA_OS_WINDOWS
  #include "Windows.h"
  typedef TIME_ZONE_INFORMATION NATimeZone;
#elif NA_OS == NA_OS_MAC_OS_X
  typedef struct timezone NATimeZone;
#endif




// If not defined, the Asc and Bin formats will use 00:00 and wintertime as the
// default timeZone setting.
typedef enum{
  NA_DATETIME_FORMAT_APACHE,                   // 01/Apr/234567:22:37:51 -0130
  NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT,  // 234567-04-01T22:37:51-01:30
  NA_DATETIME_FORMAT_CONDENSEDDATE,            // 2345670401
  NA_DATETIME_FORMAT_NATURAL                   // 234567-04-01 22:37:51
} NAAscDateTimeFormat;

typedef enum{
  NA_DATETIME_FORMAT_ICC_PROFILE,
  NA_DATETIME_FORMAT_PNG
} NABinDateTimeFormat;

typedef enum{
  NA_DATETIME_ERROR_NONE = 0,
  NA_DATETIME_ERROR_JULIAN_GREGORIAN_CHASM,
  NA_DATETIME_ERROR_INVALID_MONTH_NUMBER,
  NA_DATETIME_ERROR_INVALID_DAY_NUMBER,
  NA_DATETIME_ERROR_INVALID_HOUR_NUMBER,
  NA_DATETIME_ERROR_INVALID_MINUTE_NUMBER,
  NA_DATETIME_ERROR_INVALID_SECOND_NUMBER,
  NA_DATETIME_ERROR_COUNT
} NADateTimeError;


// This is the fundamental time struct of NALib. It uses precisely 128 Bits and stores
// both date, time and a nanosecond timer.
//
// Note that NADateTime is a pure pod and does not require a call to naClearXXX.
//
// The full type definition is in the file "NADateTimeII.h"
typedef struct NADateTime NADateTime;


// These are the structs accessible to the programmer. He can both construct
// date and time and readout information thereof.
typedef struct NADateTimeStruct NADateTimeStruct;
struct NADateTimeStruct{
  NAi64 year;      // year number in astronomic numbering (-, 0 and +)
  int32   mon;       // month number in [0, 11].   Beware the 0-index!
  int32   day;       // day number in [0, 30].     Beware the 0-index!
  int32   hour;      // hour number in [0, 23]
  int32   min;       // minute number in [0, 59]
  int32   sec;       // second number in [0, 69] (may contain leap seconds)
  int32   nanoSecond;      // Nanosecond number in [0, 999999999]
  int16   shift;     // time shift in minutes (positive or negative)
  uint8   errorNum;  // The errorNum when constructed from a stamp.
  uint8   flags;     // Various flags.
};

typedef struct NADateTimeAttribute NADateTimeAttribute;
struct NADateTimeAttribute{
  int32  yearsign;     // either +1 or -1
  int32  dayOfYear;    // days since start of year, 0-indexed (Beware 1582!)
  int32  isLeapYear;   // 1 if current year is a leap year, 0 otherwise
  int32  daysinmonth;  // number of days in this month (beware October 1582!)
  int32  weekday;      // 0 = Monday, 1 = Tuesday, ... 6 = Sunday
  int32  shiftsign;    // either +1 or -1
  int32  shifthour;    // positive shift hour number in [0, 14]
  int32  shiftmin;     // positive shift minute number in [0, 59]
};



// Returns the month number (0-indexed) of an english month abbreviation.
// For example: "Nov" -> 10
NA_API int32 naGetMonthNumberWithEnglishAbbreviation(const NAString* str);
// Reads Strings like "November", abbreviations like "Nov" and numbers like "11"
// and returns the 0-indexed month number. So, an input like "11" returns 10.
// Comparison is done case-insensitive.
NA_API int32 naGetMonthNumberFromUTF8CStringLiteral(const NAUTF8Char* str);

// Returns true if the given year number is a leap year.
NA_IAPI NABool naIsLeapYearJulian      (NAi64 year);
NA_IAPI NABool naIsLeapYearGregorian   (NAi64 year);
// The following function returns the julian information for years <= 1582 and
// the gregorian information for years > 1582.
NA_IAPI NABool naIsLeapYear            (NAi64 year);



// Creates a new NADateTime struct with the given values. This function is a
// convenience function which is easy to use and implement.
//
// The mon and day values are expected 1-indexed (1 is January or the first
// day of the month respectively). This is in contrast to the NADateTimeStruct
// where mon and day always are expressed 0-indexed. The timeZone is set to
// the global setting, see below. If you want to set the timeZone manually or
// provide any additional information, use the constructor with the
// NADateTimeStruct as an argument instead, it is much more powerful.
NA_IAPI NADateTime naMakeDateTime(  NAi64 year,
                                    int32 mon,
                                    int32 day,
                                    int32 hour,
                                    int32 min,
                                    int32 sec);

// Returns an NADateTime struct with the current system clock including the
// systems timeZone. Note that an NADateTime does not require naClearXXX upon
// deletion!
//
// Want to implement a nanosecond-timer? This is the function for you. Together
// with naGetDateTimeDiff.
NA_API NADateTime naMakeDateTimeNow(void);

// Returns an NADateTime struct with the given second number interpreted like
// in NALib. Nanoseconds and Timeshift will be zero.
NA_IAPI NADateTime naMakeDateTimeWithNALibSecondNumber(NAi64 secondNumber);

// Create a new NADateTime struct with the values provided.
NA_API NADateTime naMakeDateTimeWithDateTimeStruct(const NADateTimeStruct* dts);

// Creates a new NADateTime struct from a given string with a given format.
NA_API NADateTime naMakeDateTimeFromString( const NAString* string,
                                          NAAscDateTimeFormat format);

// Creates a new NADateTime struct from a given buffer with a given format.
NA_API NADateTime naMakeDateTimeFromBuffer(  NABuffer* buffer,
                                        NABinDateTimeFormat format);

// Returns a pointer to a C-string containing an error message corresponding
// to the given error number.
NA_API const char* naGetDateTimeErrorString(uint8 errorNum);

// Creates a byte array with the given format.
//NA_API NAByteArray* naInitByteArrayFromDateTime( NAByteArray* bytearray,
//                                              const NADateTime* dateTime,
//                                            NABinDateTimeFormat format);

// Creates a string in the given format.
NA_API NAString* naNewStringWithDateTime(const NADateTime* dateTime,
                                   NAAscDateTimeFormat format);

// Converts between the systems time formats and NADateTime
NA_API struct tm naMakeTMfromDateTime   (const NADateTime* dateTime);
// Computes the time shift in minutes including daylight saving time, if applicable.
NA_API int16     naMakeShiftFromTimeZone(const NATimeZone* timeZone, NABool daylightSaving);
// if timeZone is a Null-Pointer, the global timeZone settings are used.

#if NA_OS == NA_OS_WINDOWS
  NA_API NADateTime naMakeDateTimeFromFileTime(
    const FILETIME* fileTime,
    const NATimeZone* timeZone,
    NABool daylightSaving);
#elif NA_OS == NA_OS_MAC_OS_X
  NA_API struct timespec naMakeTimeSpecFromDateTime(
    const NADateTime* dateTime,
    NABool daylightSaving);
  NA_API struct timeval  naMakeTimeValFromDateTime(
    const NADateTime* dateTime);
  NA_API NATimeZone naMakeTimeZoneFromDateTime(
    const NADateTime* dateTime);
  NA_API NADateTime naMakeDateTimeFromTimeSpec(
    const struct timespec* timeSpec,
    const NATimeZone* timeZone);
  NA_API NADateTime naMakeDateTimeFromTimeVal(
    const struct timeval* timeVal,
    const NATimeZone* timeZone);
#endif


// Fills the given struct with date and time values, splitted into all of the
// components of the given dateTime. Fills an NADateTimeAttribute with further
// information about this date if desired. The attribute can be Null.
NA_API void naExtractDateTimeInformation(const NADateTime* dateTime,
                                         NADateTimeStruct* dts,
                                      NADateTimeAttribute* dta);
// Same thing but expressed in timeZone 00:00 wintertime
NA_IAPI void naExtractDateTimeUTCInformation(const NADateTime* dateTime,
                                             NADateTimeStruct* dts,
                                          NADateTimeAttribute* dta);

// Alters timeZone shift and daylightSavingTime flag WITHOUT changing the real clock.
// For example: 2011-04-01T22:37:51+06:00
// becomes      2011-04-02T06:07:51-01:30
NA_IAPI void naSetDateTimeZone(NADateTime* dateTime,
                                     int16 newShift,
                                    NABool daylightSavingTime);
// Alters timeZone shift and daylightSavingTime flag WITH changing the real clock.
// For example: 2011-04-01T22:37:51+06:00
// becomes      2011-04-01T22:37:51-01:30
// Note that if you are uncertain whether to use setZone or correctZone, this
// method is probably the wrong one. This method is only available for
// DateTimes which accidentally were stored with an incorrect time shift.
NA_IAPI void naCorrectDateTimeZone(NADateTime* dateTime,
                                         int16 newShift,
                                        NABool daylightSavingTime);

// Returns the difference in time. The returned value is in seconds. The
// returned value can be negative if end is before begin.
NA_IAPI double naGetDateTimeDifference( const NADateTime* end,
                                        const NADateTime* begin);
// Adds the given difference to the dateTime.
NA_IAPI void naAddDateTimeDifference(NADateTime* dateTime, double difference);

// Returns a human readable string of a second counter (including decimals)
// For example: naNewStringFromSecondDifference(12345678.12345678, 6)
// creates the string "142d 21:21:18.123456"
NA_API NAString* naNewStringFromSecondDifference(       double difference,
                                                         uint8 decimalDigits);

// Returns NA_TRUE if the date has daylightSavingTime.
NA_IAPI NABool naHasDateTimeDaylightSavingTime(const NADateTime* dateTime);
NA_IAPI void naSetDateTimeDaylightSavingTime(NADateTime* dateTime, NABool daylightSavingTime);


// Global timeZone settings
// The NADateTime.c file defines a global timeZone shift and a daylight
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
NA_IAPI void naSetGlobalTimeShift(int16 shiftMinutes, NABool daylightSavingTime);
// Sets the time shift and daylight saving flag to the system settings of the
// local machine.
NA_API void naSetGlobalTimeShiftToSystemSettings(void);






// ////////////////////////////
// LEAP SECOND CORRECTION
//
// Leap second information is published only up to a certain date. When you
// store dates of future events for which the leap seconds are not know yet,
// you must correct the dates when the leap second information has updated.
//
// To do so, you just store a specific identifier together with your data: The
// number of the first uncertain second of this library.
NA_API NAi64 naGetFirstUncertainSecondNumber(void);

// When the library updates, you can check if the new library has a new
// uncertain second number. You do this by giving the uncertain second number
// which you stored with your data to the following function:
NA_API NAInt naGetLeapSecondCorrectionConstant(NAi64 oldUncertainSecondNumber);
// The return value of this function is a number needed for an internal
// structure not visible to the programmer. A value >= 0 means, that a
// correction of your data might be required. If the value is < 0, no
// correction is necessary or can/will be made. The following macros depict
// further information about the negative constant:
#define NA_DATETIME_NO_CORRECTION_NEEDED             -1
#define NA_DATETIME_INVALID_UNCERTAIN_SECOND_NUMBER  -2
#define NA_DATETIME_NEW_LIBRARY_IS_OLDER_THAN_BEFORE -3

// To correct you data, go through all dates and send them to the following
// function together with the akquired constant from above. This function will
// correct dates only if needed or possible.
NA_API void naCorrectDateTimeForLeapSeconds(NADateTime* dateTime,
                                          NAInt leapSecondCorrectionConstant);

// When all your data is converted to the new library and you want to store
// the corrected dates, don't forget to store the NEW uncertain second number
// with your data! Otherwise it will be corrected again in the next run!




// Inline implementations are in a separate file:
#include "Core/NADateTimeII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_DATETIME_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
