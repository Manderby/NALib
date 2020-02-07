
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"


extern int16  na_globaltimeshift;
extern NABool na_globalsummertime;


#define NA_DATETIME_FLAG_SUMMERTIME 0x01

#define NA_DATETIME_GREGORIAN_400_YEAR_PERIOD naMakeInt64WithLo(400)
#define NA_DATETIME_GREGORIAN_100_YEAR_PERIOD naMakeInt64WithLo(100)
#define NA_DATETIME_GREGORIAN_4_YEAR_PERIOD   naMakeInt64WithLo(4)

#define NA_DATETIME_YEAR_1582                 naMakeInt64WithLo(1582)
#define NA_DATETIME_YEAR_1958                 naMakeInt64WithLo(1958)


// The following macros evaluate to the following numbers:
// NA_SECONDS_PER_HOUR:                        3600
// NA_SECONDS_PER_DAY:                        86400
// NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR:   15638400
// NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR:     15724800
// NA_SECONDS_JUL_TO_DEC:                  15897600
// NA_SECONDS_IN_NORMAL_YEAR:              31536000
// NA_SECONDS_IN_LEAP_YEAR:                31622400
// NA_SECONDS_IN_4_YEAR_PERIOD:           126230400

#define NA_SECONDS_PER_MINUTE (60)
#define NA_MINUTES_PER_HOUR   (60)
#define NA_SECONDS_PER_HOUR   (NA_SECONDS_PER_MINUTE * NA_MINUTES_PER_HOUR)
#define NA_HOURS_PER_DAY      (24)
#define NA_SECONDS_PER_DAY    (NA_SECONDS_PER_HOUR * NA_HOURS_PER_DAY)
#define NA_MONTHS_PER_YEAR    (12)
#define NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR  (NA_SECONDS_PER_DAY * (31+28+31+30+31+30))
#define NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR    (NA_SECONDS_PER_DAY * (31+29+31+30+31+30))
#define NA_SECONDS_JUL_TO_DEC            (NA_SECONDS_PER_DAY * (31+31+30+31+30+31))
#define NA_SECONDS_IN_NORMAL_YEAR        naMakeInt64WithLo(NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_LEAP_YEAR          naMakeInt64WithLo(NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR   + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_4_YEAR_PERIOD      naMakeInt64WithLo(NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR + 3 * NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR + 4 * NA_SECONDS_JUL_TO_DEC)

//#define NA_SECONDS_IN_100_YEAR_PERIOD \
//  (76LL * NA_SECONDS_IN_NORMAL_YEAR + 24LL * NA_SECONDS_IN_LEAP_YEAR)
// NA_SECONDS_IN_100_YEAR_PERIOD:        3155673600
#define NA_SECONDS_IN_100_YEAR_PERIOD_HI  0x0
#define NA_SECONDS_IN_100_YEAR_PERIOD_LO  0xbc17c200
#define NA_SECONDS_IN_100_YEAR_PERIOD     naCastUInt64ToInt64(naMakeUInt64(NA_SECONDS_IN_100_YEAR_PERIOD_HI, NA_SECONDS_IN_100_YEAR_PERIOD_LO))

//#define NA_SECONDS_IN_400_YEAR_PERIOD \
//  (4LL * NA_SECONDS_IN_100_YEAR_PERIOD + NA_SECONDS_PER_DAY)
// NA_SECONDS_IN_400_YEAR_PERIOD:       12622780800
#define NA_SECONDS_IN_400_YEAR_PERIOD_HI  0x00000002
#define NA_SECONDS_IN_400_YEAR_PERIOD_LO  0xf0605980
#define NA_SECONDS_IN_400_YEAR_PERIOD     naCastUInt64ToInt64(naMakeUInt64(NA_SECONDS_IN_400_YEAR_PERIOD_HI, NA_SECONDS_IN_400_YEAR_PERIOD_LO))


struct NADateTime{
  int64  sisec;     // SI-second number starting Jan 1st 1958, 00:00 + 00:00
  int32  nsec;      // nanosecond number in range [0, 999999999]
  int16  shift;     // time shift in minutes (positive and negative)
  uint8  errornum;  // error number in case invalid values were given.
  uint8  flags;     // Various flags.
};


NA_IDEF NABool naIsLeapYearJulian(int64 year){
  return naEqualInt64(naModInt64(year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD), NA_ZERO_64);
}


NA_IDEF NABool naIsLeapYearGregorian(int64 year){
  if(naEqualInt64(naModInt64(year, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD), NA_ZERO_64)){return NA_TRUE;}
  if(naEqualInt64(naModInt64(year, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD), NA_ZERO_64)){return NA_FALSE;}
  if(naEqualInt64(naModInt64(year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD), NA_ZERO_64)){return NA_TRUE;}
  return NA_FALSE;
}


NA_IDEF NABool naIsLeapYear(int64 year){
  if(naSmallerEqualInt64(year, NA_DATETIME_YEAR_1582)){
    return naIsLeapYearJulian(year);
  }else{
    return naIsLeapYearGregorian(year);
  }
}



NA_IDEF NADateTime naMakeDateTime(int64 year, int32 mon, int32 day, int32 hour, int32 min, int32 sec){
  NADateTimeStruct dts;
  dts.year = year;
  dts.mon = mon - 1;
  dts.day = day - 1;
  dts.hour = hour;
  dts.min = min;
  dts.sec = sec;
  dts.nsec = 0;
  dts.shift = na_globaltimeshift;
  dts.errornum = NA_DATETIME_ERROR_NONE;
  dts.flags = (uint8)na_globalsummertime;
  return naMakeDateTimeWithDateTimeStruct(&dts);
}



NA_IDEF NADateTime naMakeDateTimeWithNALibSecondNumber(int64 secondnumber){
  NADateTime datetime;
  datetime.sisec = secondnumber;
  datetime.nsec = 0;
  datetime.shift = 0;
  datetime.errornum = NA_DATETIME_ERROR_NONE;
  datetime.flags = 0;
  return datetime;
}




NA_IDEF void naExtractDateTimeUTCInformation(const NADateTime* datetime,
                             NADateTimeStruct* dts,
                          NADateTimeAttribute* dta){
  NADateTime utcdatetime = *datetime;
  naSetDateTimeZone(&utcdatetime, 0, NA_FALSE);
  naExtractDateTimeInformation(&utcdatetime, dts, dta);
}



NA_IDEF void naSetDateTimeZone( NADateTime* datetime,
                              int16 newshift,
                             NABool summertime){
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  datetime->shift = newshift;
  if(summertime){
    datetime->flags |= NA_DATETIME_FLAG_SUMMERTIME;
  }else{
    datetime->flags &= ~NA_DATETIME_FLAG_SUMMERTIME;
  }
}



NA_IDEF void naCorrectDateTimeZone( NADateTime* datetime,
                                  int16 newshift,
                                 NABool summertime){
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  datetime->sisec = naSubInt64(datetime->sisec, naMakeInt64WithLo(datetime->shift * (int32)NA_SECONDS_PER_MINUTE));
  datetime->sisec = naAddInt64(datetime->sisec, naMakeInt64WithLo(newshift * (int32)NA_SECONDS_PER_MINUTE));
  naSetDateTimeZone(datetime, newshift, summertime);
}



NA_DEF double naGetDateTimeDifference(const NADateTime* end, const NADateTime* begin){
  double sign = 1.;
  double diffsecs;
  double diffnsecs;
  if(naSmallerInt64(end->sisec, begin->sisec)){
    sign = -1;
    diffsecs = naCastInt64ToDouble(naSubInt64(begin->sisec, end->sisec));
  }else{
    diffsecs = naCastInt64ToDouble(naSubInt64(end->sisec, begin->sisec));
  }
  diffnsecs = ((double)end->nsec - (double)begin->nsec) / 1e9;
  return sign * (diffsecs + diffnsecs);
}



NA_IDEF void naAddDateTimeDifference(NADateTime* datetime, double difference){
  int64 fullsecs = naMakeInt64WithDouble(difference);
  int32 nanosecs = (int32)((difference - naCastInt64ToDouble(fullsecs)) * 1e9);
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if(difference < 0){
    datetime->nsec += nanosecs;
    if(datetime->nsec < 0){naDecInt64(fullsecs); datetime->nsec += 1000000000;}
    datetime->sisec = naAddInt64(datetime->sisec, fullsecs);
  }else{
    datetime->nsec += nanosecs;
    if(datetime->nsec > 999999999){naIncInt64(fullsecs); datetime->nsec -= 1000000000;}
    datetime->sisec = naAddInt64(datetime->sisec, fullsecs);
  }
}



NA_IDEF NABool naHasDateTimeSummerTime(const NADateTime* datetime){
  return (datetime->flags & NA_DATETIME_FLAG_SUMMERTIME) ? NA_TRUE : NA_FALSE;
}



NA_IDEF void naSetDateTimeSummertime(NADateTime* datetime, NABool summertime){
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if(summertime){
    if((datetime->flags & NA_DATETIME_FLAG_SUMMERTIME) == 0){
      datetime->flags |= NA_DATETIME_FLAG_SUMMERTIME;
      datetime->shift += NA_MINUTES_PER_HOUR;
    }
  }else{
    if((datetime->flags & NA_DATETIME_FLAG_SUMMERTIME) != 0){
      datetime->flags &= ~NA_DATETIME_FLAG_SUMMERTIME;
      datetime->shift -= NA_MINUTES_PER_HOUR;
    }
  }
}



NA_IDEF void naSetGlobalTimeShift(int16 shiftminutes, NABool summertime){
  na_globaltimeshift = shiftminutes;
  na_globalsummertime = summertime;
}









// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
