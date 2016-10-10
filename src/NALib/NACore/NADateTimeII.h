
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"


extern int16  na_globaltimeshift;
extern NABool na_globalsummertime;


#define NA_DATETIME_FLAG_SUMMERTIME 0x01


struct NADateTime{
  int64  sisec;     // SI-second number starting Jan 1st 1958, 00:00 + 00:00
  int32  nsec;      // nanosecond number in range [0, 999999999]
  int16  shift;     // time shift in minutes (positive and negative)
  uint8  errornum;  // error number in case invalid values were given.
  uint8  flags;     // Various flags.
};


NA_IDEF NABool naIsLeapYearJulian(int64 year){
  return !(year % 4);
}


NA_IDEF NABool naIsLeapYearGregorian(int64 year){
  if(!(year % 400)){return NA_TRUE;}
  if(!(year % 100)){return NA_FALSE;}
  if(!(year %   4)){return NA_TRUE;}
  return NA_FALSE;
}


NA_IDEF NABool naIsLeapYear(int64 year){
  if(year <= 1582){
    return naIsLeapYearJulian(year);
  }else{
    return naIsLeapYearGregorian(year);
  }
}



NA_IDEF NADateTime naMakeDateTimeWithNALibSecondNumber(uint64 secondnumber){
  NADateTime datetime;
  datetime.sisec = secondnumber;
  datetime.nsec = 0;
  datetime.shift = 0;
  datetime.errornum = NA_DATETIME_ERROR_NONE;
  datetime.flags = 0;
  return datetime;
}


NA_IDEF NADateTime naMakeDateTimeWithValues(int64 year, int32 mon, int32 day, int32 hour, int32 min, int32 sec){
  NADateTimeStruct dts = {year, mon - 1, day - 1, hour, min, sec, 0, na_globaltimeshift, NA_DATETIME_ERROR_NONE, (uint8)na_globalsummertime};
  return naMakeDateTimeWithDateTimeStruct(&dts);
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
  datetime->sisec -= (datetime->shift * NA_SECONDS_PER_MINUTE);
  datetime->sisec += (newshift * NA_SECONDS_PER_MINUTE);
  naSetDateTimeZone(datetime, newshift, summertime);
}



NA_DEF double naGetDateTimeDiff(const NADateTime* end, const NADateTime* start){
  double sign = 1.;
  double diffsecs;
  double diffnsecs;
  if(end->sisec < start->sisec){
    sign = -1;
    diffsecs = (double)(start->sisec - end->sisec);
  }else{
    diffsecs = (double)(end->sisec - start->sisec);
  }
  diffnsecs = ((double)end->nsec - (double)start->nsec) / 1e9;
  return sign * (diffsecs + diffnsecs);
}



NA_IDEF void naAddDateTimeDifference(NADateTime* datetime, double difference){
  int64 fullsecs = (int64)difference;
  int32 nanosecs = (int32)((difference - (double)fullsecs) * 1e9);
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if(difference < 0){
    datetime->nsec += nanosecs;
    if(datetime->nsec < 0){fullsecs--; datetime->nsec += 1000000000;}
    datetime->sisec += fullsecs;
  }else{
    datetime->nsec += nanosecs;
    if(datetime->nsec > 999999999){fullsecs++; datetime->nsec -= 1000000000;}
    datetime->sisec += fullsecs;
  }
}



NA_IDEF NABool naHasDateTimeSummerTime(const NADateTime* datetime){
  return (datetime->flags & NA_DATETIME_FLAG_SUMMERTIME) ? NA_TRUE : NA_FALSE;
}



NA_IDEF void naSetDateTimeSummertime(NADateTime* datetime, NABool summertime){
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if(summertime){
    if(datetime->flags & NA_DATETIME_FLAG_SUMMERTIME){return;}
    datetime->flags |= NA_DATETIME_FLAG_SUMMERTIME;
    datetime->shift += NA_MINUTES_PER_HOUR;
  }else{
    if(!(datetime->flags & NA_DATETIME_FLAG_SUMMERTIME)){return;}
    datetime->flags &= ~NA_DATETIME_FLAG_SUMMERTIME;
    datetime->shift -= NA_MINUTES_PER_HOUR;
  }
}



NA_IDEF void naSetGlobalTimeShift(int16 shiftminutes, NABool summertime){
  na_globaltimeshift = shiftminutes;
  na_globalsummertime = summertime;
}









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
