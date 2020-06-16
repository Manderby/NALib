
// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"


extern int16  na_GlobalTimeShift;
extern NABool na_GlobalSummerTime;


#define NA_DATETIME_FLAG_SUMMERTIME 0x01

#define NA_DATETIME_GREGORIAN_400_YEAR_PERIOD naMakei64WithLo(400)
#define NA_DATETIME_GREGORIAN_100_YEAR_PERIOD naMakei64WithLo(100)
#define NA_DATETIME_GREGORIAN_4_YEAR_PERIOD   naMakei64WithLo(4)

#define NA_DATETIME_YEAR_1582                 naMakei64WithLo(1582)
#define NA_DATETIME_YEAR_1958                 naMakei64WithLo(1958)


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
#define NA_SECONDS_IN_NORMAL_YEAR        naMakei64WithLo(NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_LEAP_YEAR          naMakei64WithLo(NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR   + NA_SECONDS_JUL_TO_DEC)
#define NA_SECONDS_IN_4_YEAR_PERIOD      naMakei64WithLo(NA_SECONDS_JAN_TO_JUN_IN_LEAP_YEAR + 3 * NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR + 4 * NA_SECONDS_JUL_TO_DEC)

//#define NA_SECONDS_IN_100_YEAR_PERIOD \
//  (76LL * NA_SECONDS_IN_NORMAL_YEAR + 24LL * NA_SECONDS_IN_LEAP_YEAR)
// NA_SECONDS_IN_100_YEAR_PERIOD:        3155673600
#define NA_SECONDS_IN_100_YEAR_PERIOD_HI  0x0
#define NA_SECONDS_IN_100_YEAR_PERIOD_LO  0xbc17c200
#define NA_SECONDS_IN_100_YEAR_PERIOD     naCastu64Toi64(naMakeu64(NA_SECONDS_IN_100_YEAR_PERIOD_HI, NA_SECONDS_IN_100_YEAR_PERIOD_LO))

//#define NA_SECONDS_IN_400_YEAR_PERIOD \
//  (4LL * NA_SECONDS_IN_100_YEAR_PERIOD + NA_SECONDS_PER_DAY)
// NA_SECONDS_IN_400_YEAR_PERIOD:       12622780800
#define NA_SECONDS_IN_400_YEAR_PERIOD_HI  0x00000002
#define NA_SECONDS_IN_400_YEAR_PERIOD_LO  0xf0605980
#define NA_SECONDS_IN_400_YEAR_PERIOD     naCastu64Toi64(naMakeu64(NA_SECONDS_IN_400_YEAR_PERIOD_HI, NA_SECONDS_IN_400_YEAR_PERIOD_LO))


struct NADateTime{
  NAi64  siSecond;     // SI-second number starting Jan 1st 1958, 00:00 + 00:00
  int32    nanoSecond;      // nanosecond number in range [0, 999999999]
  int16    shift;     // time shift in minutes (positive and negative)
  uint8    errorNum;  // error number in case invalid values were given.
  uint8    flags;     // Various flags.
};


NA_IDEF NABool naIsLeapYearJulian(NAi64 year){
  return naEquali64(naModi64(year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD), NA_ZERO_i64);
}


NA_IDEF NABool naIsLeapYearGregorian(NAi64 year){
  if(naEquali64(naModi64(year, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD), NA_ZERO_i64)){return NA_TRUE;}
  if(naEquali64(naModi64(year, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD), NA_ZERO_i64)){return NA_FALSE;}
  if(naEquali64(naModi64(year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD), NA_ZERO_i64)){return NA_TRUE;}
  return NA_FALSE;
}


NA_IDEF NABool naIsLeapYear(NAi64 year){
  if(naSmallerEquali64(year, NA_DATETIME_YEAR_1582)){
    return naIsLeapYearJulian(year);
  }else{
    return naIsLeapYearGregorian(year);
  }
}



NA_IDEF NADateTime naMakeDateTime(NAi64 year, int32 mon, int32 day, int32 hour, int32 min, int32 sec){
  NADateTimeStruct dts;
  dts.year = year;
  dts.mon = mon - 1;
  dts.day = day - 1;
  dts.hour = hour;
  dts.min = min;
  dts.sec = sec;
  dts.nanoSecond = 0;
  dts.shift = na_GlobalTimeShift;
  dts.errorNum = NA_DATETIME_ERROR_NONE;
  dts.flags = (uint8)na_GlobalSummerTime;
  return naMakeDateTimeWithDateTimeStruct(&dts);
}



NA_IDEF NADateTime naMakeDateTimeWithNALibSecondNumber(NAi64 secondNumber){
  NADateTime dateTime;
  dateTime.siSecond = secondNumber;
  dateTime.nanoSecond = 0;
  dateTime.shift = 0;
  dateTime.errorNum = NA_DATETIME_ERROR_NONE;
  dateTime.flags = 0;
  return dateTime;
}




NA_IDEF void naExtractDateTimeUTCInformation(const NADateTime* dateTime,
                             NADateTimeStruct* dts,
                          NADateTimeAttribute* dta){
  NADateTime utcdateTime = *dateTime;
  naSetDateTimeZone(&utcdateTime, 0, NA_FALSE);
  naExtractDateTimeInformation(&utcdateTime, dts, dta);
}



NA_IDEF void naSetDateTimeZone( NADateTime* dateTime,
                              int16 newShift,
                             NABool summerTime){
  dateTime->errorNum = NA_DATETIME_ERROR_NONE;
  dateTime->shift = newShift;
  if(summerTime){
    dateTime->flags |= NA_DATETIME_FLAG_SUMMERTIME;
  }else{
    dateTime->flags &= ~NA_DATETIME_FLAG_SUMMERTIME;
  }
}



NA_IDEF void naCorrectDateTimeZone( NADateTime* dateTime,
                                  int16 newShift,
                                 NABool summerTime){
  dateTime->errorNum = NA_DATETIME_ERROR_NONE;
  dateTime->siSecond = naSubi64(dateTime->siSecond, naMakei64WithLo(dateTime->shift * (int32)NA_SECONDS_PER_MINUTE));
  dateTime->siSecond = naAddi64(dateTime->siSecond, naMakei64WithLo(newShift * (int32)NA_SECONDS_PER_MINUTE));
  naSetDateTimeZone(dateTime, newShift, summerTime);
}



NA_DEF double naGetDateTimeDifference(const NADateTime* end, const NADateTime* begin){
  double sign = 1.;
  double diffSeconds;
  double diffNanoSeconds;
  if(naSmalleri64(end->siSecond, begin->siSecond)){
    sign = -1;
    diffSeconds = naCasti64ToDouble(naSubi64(begin->siSecond, end->siSecond));
  }else{
    diffSeconds = naCasti64ToDouble(naSubi64(end->siSecond, begin->siSecond));
  }
  diffNanoSeconds = ((double)end->nanoSecond - (double)begin->nanoSecond) / 1e9;
  return sign * (diffSeconds + diffNanoSeconds);
}



NA_IDEF void naAddDateTimeDifference(NADateTime* dateTime, double difference){
  NAi64 fullSeconds = naMakei64WithDouble(difference);
  int32 nanoSeconds = (int32)((difference - naCasti64ToDouble(fullSeconds)) * 1e9);
  dateTime->errorNum = NA_DATETIME_ERROR_NONE;
  if(difference < 0){
    dateTime->nanoSecond += nanoSeconds;
    if(dateTime->nanoSecond < 0){naDeci64(fullSeconds); dateTime->nanoSecond += 1000000000;}
    dateTime->siSecond = naAddi64(dateTime->siSecond, fullSeconds);
  }else{
    dateTime->nanoSecond += nanoSeconds;
    if(dateTime->nanoSecond > 999999999){naInci64(fullSeconds); dateTime->nanoSecond -= 1000000000;}
    dateTime->siSecond = naAddi64(dateTime->siSecond, fullSeconds);
  }
}



NA_IDEF NABool naHasDateTimeSummerTime(const NADateTime* dateTime){
  return (dateTime->flags & NA_DATETIME_FLAG_SUMMERTIME) ? NA_TRUE : NA_FALSE;
}



NA_IDEF void naSetDateTimeSummertime(NADateTime* dateTime, NABool summerTime){
  dateTime->errorNum = NA_DATETIME_ERROR_NONE;
  if(summerTime){
    if((dateTime->flags & NA_DATETIME_FLAG_SUMMERTIME) == 0){
      dateTime->flags |= NA_DATETIME_FLAG_SUMMERTIME;
      dateTime->shift += NA_MINUTES_PER_HOUR;
    }
  }else{
    if((dateTime->flags & NA_DATETIME_FLAG_SUMMERTIME) != 0){
      dateTime->flags &= ~NA_DATETIME_FLAG_SUMMERTIME;
      dateTime->shift -= NA_MINUTES_PER_HOUR;
    }
  }
}



NA_IDEF void naSetGlobalTimeShift(int16 shiftMinutes, NABool summerTime){
  na_GlobalTimeShift = shiftMinutes;
  na_GlobalSummerTime = summerTime;
}



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
