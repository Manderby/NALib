
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NADateTime.h"
#include "NAString.h"
#include "NABinaryData.h"
#include "NAEndianness.h"
#include "NAMathOperators.h"
#include "ctype.h"

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_s(storage, tme);}
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <sys/time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_r(tme, storage);}
#else
  #warning "System undefined"
#endif

// The global timezone setting.
int16  na_globaltimeshift = 0;
NABool na_globalsummertime = NA_FALSE;

const char* na_monthenglishnames[12] = {"January", "February", "March", "April", "Mai", "June", "July", "August", "September", "October", "November", "December"};
const char* na_monthenglishabbreviationnames[12] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const char* na_datetime_errorstrings[NA_DATETIME_ERROR_COUNT] = {
  "No Error",
  "Date lies between Julian and Gregorian calendar.",
  "Invalid month number (0 is January).",
  "Invalid day number (0 is first day of month)",
  "Invalid hour number.",
  "Invalid minute number.",
  "Invalid second number.",
};



// stores the day index of the first day of each month both for a normal and
// a leap year. This structure speeds up the implementation quite a bit
int32 na_cumulativemonthstartdays[2 * 13] = { 0,  0,
                                              31, 31,
                                              59, 60,
                                              90, 91,
                                              120, 121,
                                              151, 152,
                                              181, 182,
                                              212, 213,
                                              243, 244,
                                              273, 274,
                                              304, 305,
                                              334, 335,
                                              365, 366};

typedef enum{
  NA_CALENDAR_SYSTEM_JULIAN,
  NA_CALENDAR_SYSTEM_GREGORIAN,
  NA_CALENDAR_SYSTEM_GREGORIAN_WITH_LEAP_SECONDS
} NACalendarSystem;

typedef enum{
  NA_START_JANUARY_FIRST,
  NA_START_JULY_FIRST,
  NA_POSITIVE_LEAP_SECONDS_JUNE,
  NA_POSITIVE_LEAP_SECONDS_DECEMBER,
} NALeapSecondCorrectionIndicator;

typedef struct{
  int64 startgregsec;
  int64 startsisec;
  int16 gregyear;
  NALeapSecondCorrectionIndicator indicator;
} NATAIPeriod;

// Leap second information:
#define NA_NUMBER_OF_TAI_PERIODS 97

// This table stores all leap second entries since 1958. Every year has at
// least 1 entry. Every entry defines, what the number of its first second is.
// Additionally, it is defined, what the seconds in that period are. They can
// be regular seconds denoting a start date, or they can be lead seconds. This
// structure is future-proof for at least some centuries, as it will be able to
// map leap seconds even if they are introduced every second. The same goes for
// negative leap seconds. But there have not been introduced any so far.
NATAIPeriod naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS] = {
  {         0,          0, 1958, NA_START_JANUARY_FIRST},           // [0]
  {  31536000,   31536000, 1959, NA_START_JANUARY_FIRST},
  {  63072000,   63072000, 1960, NA_START_JANUARY_FIRST},
  {  94694400,   94694400, 1961, NA_START_JANUARY_FIRST},
  { 126230400,  126230400, 1962, NA_START_JANUARY_FIRST},
  { 157766400,  157766400, 1963, NA_START_JANUARY_FIRST},
  { 189302400,  189302400, 1964, NA_START_JANUARY_FIRST},
  { 220924800,  220924800, 1965, NA_START_JANUARY_FIRST},
  { 252460800,  252460800, 1966, NA_START_JANUARY_FIRST},
  { 283996800,  283996800, 1967, NA_START_JANUARY_FIRST},
  { 315532800,  315532800, 1968, NA_START_JANUARY_FIRST},           // [10]
  { 347155200,  347155200, 1969, NA_START_JANUARY_FIRST},
  { 378691200,  378691200, 1970, NA_START_JANUARY_FIRST},
  { 410227200,  410227200, 1971, NA_START_JANUARY_FIRST},
  { 441763200,  441763200, 1971, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 10
  { 441763200,  441763210, 1972, NA_START_JANUARY_FIRST},
  { 457488000,  457488010, 1972, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  { 457488000,  457488011, 1972, NA_START_JULY_FIRST},
  { 473385600,  473385611, 1972, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 473385600,  473385612, 1973, NA_START_JANUARY_FIRST},
  { 504921600,  504921612, 1973, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [20] + 1
  { 504921600,  504921613, 1974, NA_START_JANUARY_FIRST},
  { 536457600,  536457613, 1974, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 536457600,  536457614, 1975, NA_START_JANUARY_FIRST},
  { 567993600,  567993614, 1975, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 567993600,  567993615, 1976, NA_START_JANUARY_FIRST},
  { 599616000,  599616015, 1976, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 599616000,  599616016, 1977, NA_START_JANUARY_FIRST},
  { 631152000,  631152016, 1977, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 631152000,  631152017, 1978, NA_START_JANUARY_FIRST},
  { 662688000,  662688017, 1978, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [30] + 1
  { 662688000,  662688018, 1979, NA_START_JANUARY_FIRST},
  { 694224000,  694224018, 1979, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 694224000,  694224019, 1980, NA_START_JANUARY_FIRST},
  { 725846400,  725846419, 1981, NA_START_JANUARY_FIRST},
  { 741484800,  741484819, 1981, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  { 741484800,  741484820, 1981, NA_START_JULY_FIRST},
  { 757382400,  757382420, 1982, NA_START_JANUARY_FIRST},
  { 773020800,  773020820, 1982, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  { 773020800,  773020821, 1982, NA_START_JULY_FIRST},
  { 788918400,  788918421, 1983, NA_START_JANUARY_FIRST},           // [40]
  { 804556800,  804556821, 1983, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  { 804556800,  804556822, 1983, NA_START_JULY_FIRST},
  { 820454400,  820454422, 1984, NA_START_JANUARY_FIRST},
  { 852076800,  852076822, 1985, NA_START_JANUARY_FIRST},
  { 867715200,  867715222, 1985, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  { 867715200,  867715223, 1985, NA_START_JULY_FIRST},
  { 883612800,  883612823, 1986, NA_START_JANUARY_FIRST},
  { 915148800,  915148823, 1987, NA_START_JANUARY_FIRST},
  { 946684800,  946684823, 1987, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  { 946684800,  946684824, 1988, NA_START_JANUARY_FIRST},           // [50]
  { 978307200,  978307224, 1989, NA_START_JANUARY_FIRST},
  {1009843200, 1009843224, 1989, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {1009843200, 1009843225, 1990, NA_START_JANUARY_FIRST},
  {1041379200, 1041379225, 1990, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {1041379200, 1041379226, 1991, NA_START_JANUARY_FIRST},
  {1072915200, 1072915226, 1992, NA_START_JANUARY_FIRST},
  {1088640000, 1088640026, 1992, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {1088640000, 1088640027, 1992, NA_START_JULY_FIRST},
  {1104537600, 1104537627, 1993, NA_START_JANUARY_FIRST},
  {1120176000, 1120176027, 1993, NA_POSITIVE_LEAP_SECONDS_JUNE},    // [60] + 1
  {1120176000, 1120176028, 1993, NA_START_JULY_FIRST},
  {1136073600, 1136073628, 1994, NA_START_JANUARY_FIRST},
  {1151712000, 1151712028, 1994, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {1151712000, 1151712029, 1994, NA_START_JULY_FIRST},
  {1167609600, 1167609629, 1995, NA_START_JANUARY_FIRST},
  {1199145600, 1199145629, 1995, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {1199145600, 1199145630, 1996, NA_START_JANUARY_FIRST},
  {1230768000, 1230768030, 1997, NA_START_JANUARY_FIRST},
  {1246406400, 1246406430, 1997, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {1246406400, 1246406431, 1997, NA_START_JULY_FIRST},              // [70]
  {1262304000, 1262304031, 1998, NA_START_JANUARY_FIRST},
  {1293840000, 1293840031, 1998, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {1293840000, 1293840032, 1999, NA_START_JANUARY_FIRST},
  {1325376000, 1325376032, 2000, NA_START_JANUARY_FIRST},
  {1356998400, 1356998432, 2001, NA_START_JANUARY_FIRST},
  {1388534400, 1388534432, 2002, NA_START_JANUARY_FIRST},
  {1420070400, 1420070432, 2003, NA_START_JANUARY_FIRST},
  {1451606400, 1451606432, 2004, NA_START_JANUARY_FIRST},
  {1483228800, 1483228832, 2005, NA_START_JANUARY_FIRST},
  {1514764800, 1514764832, 2005, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [80] + 1
  {1514764800, 1514764833, 2006, NA_START_JANUARY_FIRST},
  {1546300800, 1546300833, 2007, NA_START_JANUARY_FIRST},
  {1577836800, 1577836833, 2008, NA_START_JANUARY_FIRST},
  {1609459200, 1609459233, 2008, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {1609459200, 1609459234, 2009, NA_START_JANUARY_FIRST},
  {1640995200, 1640995234, 2010, NA_START_JANUARY_FIRST},
  {1672531200, 1672531234, 2011, NA_START_JANUARY_FIRST},
  {1704067200, 1704067234, 2012, NA_START_JANUARY_FIRST},
  {1719792000, 1719792034, 2012, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {1719792000, 1719792035, 2012, NA_START_JULY_FIRST},              // [90]
  {1735689600, 1735689635, 2013, NA_START_JANUARY_FIRST},
  {1767225600, 1767225635, 2014, NA_START_JANUARY_FIRST},
  {1798761600, 1798761635, 2015, NA_START_JANUARY_FIRST},
  {1814400000, 1814400035, 2015, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {1814400000, 1814400036, 2015, NA_START_JULY_FIRST},
  {1830297600, 1830297636, 2015, NA_START_JANUARY_FIRST},
  // the last entry is the first date with unknown future leap seconds.
  // everything up and including that date is known.
};


// TAI period starts at January 1st 1958
// Gregorian time period starts on Friday, October 15th 1582
// Julian time period ends with Thursday, October 4th 1582 (inclusive).

#define NA_START_GREGORIAN_PERIOD            (- NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 4LL * NA_SECONDS_IN_4_YEAR_PERIOD - NA_SECONDS_IN_NORMAL_YEAR - (31LL+30LL+31LL-14LL) * NA_SECONDS_PER_DAY)
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering. Equals year 1 BC.
// This assumes the planned leap years by cesar.
#define NA_YEAR_ZERO                         (NA_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+4LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 395LL * NA_SECONDS_IN_4_YEAR_PERIOD)
#define NA_YEAR_ZERO_OF_GREGORIAN_PERIOD     (NA_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+14LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 19LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 3LL * NA_SECONDS_IN_400_YEAR_PERIOD)
// the UNIX system time has no leap seconds and is nulled in 1970
#define NA_GREG_SECONDS_TILL_BEGIN_1970    (3LL * NA_SECONDS_IN_4_YEAR_PERIOD)
#define NA_GREG_SECONDS_SINCE_BEGIN_1601   (-NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 2LL * NA_SECONDS_IN_100_YEAR_PERIOD - 24LL * NA_SECONDS_IN_4_YEAR_PERIOD - 3LL * NA_SECONDS_IN_NORMAL_YEAR)


// Prototypes:
NAUInt naGetTAIPeriodIndexForSISecond(int64 sisecond);
NAUInt naGetLatestTAIPeriodIndexForGregorianSecond(int64 gregsecond);
NADateTime naMakeDateTimeNow();
void naSetGlobalTimeShiftToSystemSettings();
int64 naGetFirstUncertainSecondNumber();




NA_DEF NAUInt naGetTAIPeriodIndexForSISecond(int64 sisecond){
  NAUInt l, r, m;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 3].startsisec <= sisecond){
    if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 2].startsisec <= sisecond){
      if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec <= sisecond){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_NUMBER_OF_TAI_PERIODS - 1;
      }
      return NA_NUMBER_OF_TAI_PERIODS - 2;
    }
    return NA_NUMBER_OF_TAI_PERIODS - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(sisecond < 0){return -1;}
  // In all other cases, perform a binary search in all TAI periods.
  l = 0;
  r = NA_NUMBER_OF_TAI_PERIODS - 4;
  while(l != r){  // binary search
    m = (l+r)/2;
    if(naTAIPeriods[m + 1].startsisec <= sisecond){l = m + 1;}else{r = m;}
  }
  // l or r now define the index of the latest NATAIPeriod.
  return r;
}



NA_DEF NAUInt naGetLatestTAIPeriodIndexForGregorianSecond(int64 gregsecond){
  NAUInt l, r, m;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 3].startgregsec <= gregsecond){
    if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 2].startgregsec <= gregsecond){
      if(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startgregsec <= gregsecond){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_NUMBER_OF_TAI_PERIODS - 1;
      }
      return NA_NUMBER_OF_TAI_PERIODS - 2;
    }
    return NA_NUMBER_OF_TAI_PERIODS - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(gregsecond < 0){return -1;}
  // In all other cases, perform a binary search in all TAI periods.
  l = 0;
  r = NA_NUMBER_OF_TAI_PERIODS - 4;
  while(l != r){  // binary search
    m = (l+r)/2;
    if(naTAIPeriods[m + 1].startgregsec <= gregsecond){l = m + 1;}else{r = m;}
  }
  // l or r now define the index of the latest NATAIPeriod.
  return r;
}



NA_DEF int32 naGetMonthNumberWithEnglishAbbreviation(const NAString* str){
  int32 i;
  for(i=0; i<NA_MONTHS_PER_YEAR; i++){
    if(naEqualStringToUTF8CStringLiteral(str, na_monthenglishabbreviationnames[i])){return i;}
  }
  #ifndef NDEBUG
    naError("naGetMonthNumberWithEnglishAbbreviation", "Month abbreviation unknown. Returning -1.");
  #endif
  return -1;
}


NA_DEF int32 naGetMonthNumberFromUTF8CStringLiteral(const NAUTF8Char* str){
  int32 i;
  NAUInt len = naStrlen(str);
  if(!len){return -1;}
  for(i=0; i<NA_MONTHS_PER_YEAR; i++){
    if(naEqualUTF8CStringLiteralsCaseInsensitive(str, na_monthenglishnames[i])){return i;}
  }
  for(i=0; i<NA_MONTHS_PER_YEAR; i++){
    if(naEqualUTF8CStringLiteralsCaseInsensitive(str, na_monthenglishabbreviationnames[i])){return i;}
  }
  if(isdigit(str[0])){
    int32 returnint;
    NAString* numberstring = naNewStringWithUTF8CStringLiteral(str);
    returnint = naGetStringInt32(numberstring) - 1;
    naDelete(numberstring);
    return returnint;
  }
  #ifndef NDEBUG
    naError("naGetMonthNumberWithEnglishAbbreviation", "Month abbreviation unknown. Returning -1.");
  #endif
  return -1;
}


NA_DEF NABool naIsLeapYearJulian(int64 year){
  return !(year % 4);
}


NA_DEF NABool naIsLeapYearGregorian(int64 year){
  if(!(year % 400)){return NA_TRUE;}
  if(!(year % 100)){return NA_FALSE;}
  if(!(year %   4)){return NA_TRUE;}
  return NA_FALSE;
}


NA_DEF NABool naIsLeapYear(int64 year){
  if(year <= 1582){
    return naIsLeapYearJulian(year);
  }else{
    return naIsLeapYearGregorian(year);
  }
}





NA_DEF NADateTime naMakeDateTimeNow(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    FILETIME filetime;
    NATimeZone timezone;
    GetSystemTimeAsFileTime(&filetime);
    GetTimeZoneInformation(&timezone);
    return naMakeDateTimeFromFileTime(&filetime, &timezone);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    struct timeval curtime;
    NATimeZone curtimezone;
    gettimeofday(&curtime, &curtimezone);
    return naMakeDateTimeFromTimeVal(&curtime, &curtimezone);
  #endif
}


NA_API NADateTime naMakeDateTimeWithNALibSecondNumber(uint64 secondnumber){
  NADateTime datetime;
  datetime.sisec = secondnumber;
  datetime.nsec = 0;
  datetime.shift = 0;
  datetime.errornum = NA_DATETIME_ERROR_NONE;
  datetime.flags = 0;
  return datetime;
}



NA_DEF NADateTime naMakeDateTimeWithDateTimeStruct(const NADateTimeStruct* dts){
  NADateTime datetime;

  int64 remainingyears = dts->year;
  int64 years400;
  int64 years100;
  int64 years4;
  NABool isleap;
  NACalendarSystem calendarsystem;

  datetime.errornum = NA_DATETIME_ERROR_NONE;

  if((dts->year == 1582) && (dts->mon == 9) && (dts->day > 3) && (dts->day < 14)){
    datetime.errornum = NA_DATETIME_ERROR_JULIAN_GREGORIAN_CHASM;
  }
  if((dts->year < 1582) || ((dts->year == 1582) && ((dts->mon < 9) || ((dts->mon == 9) && (dts->day < 14))))){
    // julian system
    datetime.sisec = NA_YEAR_ZERO;
    isleap = naIsLeapYearJulian(dts->year);
    calendarsystem = NA_CALENDAR_SYSTEM_JULIAN;
  }else{
    // gregorian system
    datetime.sisec = NA_YEAR_ZERO_OF_GREGORIAN_PERIOD;
    years400 = (remainingyears / 400);
    datetime.sisec += years400 * NA_SECONDS_IN_400_YEAR_PERIOD;
    remainingyears -= (years400 * 400);
    years100 = (remainingyears / 100);
    datetime.sisec += years100 * NA_SECONDS_IN_100_YEAR_PERIOD;
    remainingyears -= (years100 * 100);
    isleap = naIsLeapYearGregorian(dts->year);
    if(dts->year < 1958){
      calendarsystem = NA_CALENDAR_SYSTEM_GREGORIAN;
    }else{
      calendarsystem = NA_CALENDAR_SYSTEM_GREGORIAN_WITH_LEAP_SECONDS;
    }
  }
  years4 = (remainingyears / 4);
  if(remainingyears < 0){years4--;}
  datetime.sisec += years4 * NA_SECONDS_IN_4_YEAR_PERIOD;
  remainingyears -= (years4 * 4);
  if(remainingyears){
    datetime.sisec += NA_SECONDS_IN_LEAP_YEAR; remainingyears--;
    datetime.sisec += remainingyears * NA_SECONDS_IN_NORMAL_YEAR;
  }
  if((dts->mon < 0) || (dts->mon > 11)){datetime.errornum = NA_DATETIME_ERROR_INVALID_MONTH_NUMBER;}
  datetime.sisec += na_cumulativemonthstartdays[2 * dts->mon + isleap] * NA_SECONDS_PER_DAY;
  if((dts->day < 0) || ((na_cumulativemonthstartdays[2 * dts->mon + isleap] + dts->day) >= na_cumulativemonthstartdays[2 * (dts->mon + 1) + isleap])){datetime.errornum = NA_DATETIME_ERROR_INVALID_DAY_NUMBER;}
  datetime.sisec += dts->day * NA_SECONDS_PER_DAY;
  if((dts->hour < 0) || (dts->hour > 23)){datetime.errornum = NA_DATETIME_ERROR_INVALID_HOUR_NUMBER;}
  datetime.sisec += dts->hour * NA_SECONDS_PER_HOUR;
  if((dts->min < 0) || (dts->min > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_MINUTE_NUMBER;}
  datetime.sisec += dts->min * NA_SECONDS_PER_MINUTE;
  if(calendarsystem == NA_CALENDAR_SYSTEM_GREGORIAN_WITH_LEAP_SECONDS){
    if(datetime.sisec >= naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec){
      if((dts->sec < 0) || (dts->sec > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
      datetime.sisec += naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec - naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startgregsec;
    }else{
      NAUInt r = naGetLatestTAIPeriodIndexForGregorianSecond(datetime.sisec);
      // r now defines the index of the NATAIPeriod
      datetime.sisec += naTAIPeriods[r].startsisec - naTAIPeriods[r].startgregsec;
      datetime.sisec += dts->sec;
      if((r < NA_NUMBER_OF_TAI_PERIODS-1) && (datetime.sisec >= naTAIPeriods[r+1].startsisec)){
        if((naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE) || (naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER)){
          if(datetime.sisec >= naTAIPeriods[r+2].startsisec){
            // The leap seconds are overflown
            datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;
          }
        }else{
          // The period overflown has no leap seconds
          datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;
        }
      }else{
        if((dts->sec < 0) || (dts->sec > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
      }
      datetime.sisec -= dts->sec;
    }
  }else{
    if((dts->sec < 0) || (dts->sec > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
  }
  datetime.sisec += dts->sec;
  datetime.sisec -= (dts->shift * NA_SECONDS_PER_MINUTE);
  datetime.nsec  = dts->nsec;
  datetime.shift = dts->shift;
  datetime.flags = dts->flags;

  return datetime;
}



NA_DEF NADateTime naMakeDateTimeWithValues(int64 year, int32 mon, int32 day, int32 hour, int32 min, int32 sec){
  NADateTimeStruct dts = {year, mon - 1, day - 1, hour, min, sec, 0, na_globaltimeshift, NA_DATETIME_ERROR_NONE, na_globalsummertime};
  return naMakeDateTimeWithDateTimeStruct(&dts);
}



NA_DEF NADateTime naMakeDateTimeFromString(const NAString* string, NAAscDateTimeFormat format){
  NAString* str;
  NADateTimeStruct dts;
  NAString* token;
  int16 int16value;
  
  dts.nsec = 0;
  
  str = naNewStringExtraction(string, 0, -1);
  token = naNewString();

  switch(format){
  case NA_DATETIME_FORMAT_APACHE:
    dts.day = naParseStringInt32(str, NA_TRUE);
    token = naParseStringTokenWithDelimiter(str, '/');
    dts.mon = naGetMonthNumberWithEnglishAbbreviation(token);
    naDelete(token);
    dts.year = naParseStringInt64(str, NA_TRUE);
    
    dts.hour = naParseStringInt32(str, NA_TRUE);
    dts.min = naParseStringInt32(str, NA_TRUE);
    dts.sec = naParseStringInt32(str, NA_TRUE);

    // The remaining string contains the time shift
    int16value = naGetStringInt16(str);
    dts.shift = (int16value / 100) * NA_MINUTES_PER_HOUR;
    dts.shift += int16value % 100;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT:
    dts.year = naParseStringInt64(str, NA_TRUE);
    dts.mon = naParseStringInt32(str, NA_TRUE) - 1;
    dts.day = naParseStringInt32(str, NA_TRUE) - 1;

    dts.hour = naParseStringInt32(str, NA_TRUE);
    dts.min = naParseStringInt32(str, NA_TRUE);
    dts.sec = naParseStringInt32(str, NA_FALSE   );

    dts.shift = naParseStringInt16(str, NA_TRUE) * NA_MINUTES_PER_HOUR;
    if(dts.shift < 0){
      dts.shift -= naGetStringInt16(str);
    }else{
      dts.shift += naGetStringInt16(str);
    }
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_CONDENSEDDATE:
    token = naNewStringExtraction(str, 0, -5);
    dts.year = naGetStringInt64(token);
    naDelete(token);
    token = naNewStringExtraction(str, -4, -3);
    dts.mon = naGetStringInt32(token);
    naDelete(token);
    token = naNewStringExtraction(str, -2, -1);
    dts.day = naGetStringInt32(token);
    naDelete(token);
    dts.hour = 0;
    dts.min = 0;
    dts.sec = 0;
    dts.shift = 0;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_NATURAL:
    dts.year = naParseStringInt64(str, NA_TRUE);
    dts.mon = naParseStringInt32(str, NA_TRUE);
    dts.day = naParseStringInt32(str, NA_TRUE);

    dts.hour = naParseStringInt32(str, NA_TRUE);
    dts.min = naParseStringInt32(str, NA_TRUE);
    dts.sec = naGetStringInt32(str);

    dts.shift = 0;
    dts.flags = 0;
    break;
  }
  
  naDelete(token);
  return naMakeDateTimeWithDateTimeStruct(&dts);
}




NA_DEF NADateTime naMakeDateTimeFromPointer(const void* data, NABinDateTimeFormat format){
  NADateTimeStruct dts;
  uint16 valueu16;

  #ifndef NDEBUG
    if(!data){
      NADateTime dummytime = naMakeDateTimeNow();
      naCrash("naMakeDateTimeFromPointer", "data is Null-Pointer.");
      return dummytime;
    }
  #endif

  switch(format){
  case NA_DATETIME_FORMAT_ICC_PROFILE:
    // ICC section 5.1.1, page 4, dateTimeNumber
    naCpy16(&valueu16, ((NAByte*)data) + 0);
    dts.year = valueu16;
    naCpy16(&valueu16, ((NAByte*)data) + 2);
    dts.mon = valueu16;
    naCpy16(&valueu16, ((NAByte*)data) + 4);
    dts.day = valueu16;
    naCpy16(&valueu16, ((NAByte*)data) + 6);
    dts.hour = valueu16;
    naCpy16(&valueu16, ((NAByte*)data) + 8);
    dts.min = valueu16;
    naCpy16(&valueu16, ((NAByte*)data) + 10);
    dts.sec = valueu16;
    dts.shift = 0;
    dts.flags = 0;
    break;
  }
  return naMakeDateTimeWithDateTimeStruct(&dts);
}



NA_API const char* naGetDateTimeErrorString(uint8 errornum){
  if(errornum >= NA_DATETIME_ERROR_COUNT){
    #ifndef NDEBUG
      naError("naGetDateTimeErrorString", "Error number invalid");
    #endif
    return NA_NULL;
  }
  return na_datetime_errorstrings[errornum];
}



NA_DEF NAByteArray* naInitByteArrayFromDateTime( NAByteArray* bytearray, const NADateTime* datetime, NABinDateTimeFormat format){
  // Declaration before Implementation. Needed for C90
  uint16 valueu16;
  NAByte* ptr;
  NADateTimeStruct dts;
  NADateTimeAttribute dta;
  naExtractDateTimeInformation(datetime, &dts, &dta);

  switch(format){
  case NA_DATETIME_FORMAT_ICC_PROFILE:    
    // ICC section 5.1.1, page 4, dateTimeNumber
    bytearray = naInitByteArrayWithSize(bytearray, 12);
    ptr = naGetByteArrayMutablePointer(bytearray);
    
    valueu16 = (uint16)dts.year;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[ 0]), &valueu16);
    valueu16 = (uint16)dts.mon;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[ 2]), &valueu16);
    valueu16 = (uint16)dts.day;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[ 4]), &valueu16);
    valueu16 = (uint16)dts.hour;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[ 6]), &valueu16);
    valueu16 = (uint16)dts.min;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[ 8]), &valueu16);
    valueu16 = (uint16)dts.sec;
    naConvertNativeBig16(&valueu16);
    naCpy16(&(ptr[10]), &valueu16);
    break;
  }

  return bytearray;
}



NA_DEF NAString* naNewStringWithDateTime(const NADateTime* datetime,
                           NAAscDateTimeFormat format){
  NAString* string;
  NADateTimeStruct dts;
  NADateTimeAttribute dta;
  naExtractDateTimeInformation(datetime, &dts, &dta);
  
  switch(format){
  case NA_DATETIME_FORMAT_APACHE:
    string = naNewStringWithFormat("%02d/%s/%lld:%02d:%02d:%02d %c%02d%02d",
                dts.day + 1,
                na_monthenglishabbreviationnames[dts.mon],
                dts.year,
                dts.hour,
                dts.min,
                dts.sec,
                ',' - dta.shiftsign,
                dta.shifthour,
                dta.shiftmin);
    break;
  case NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT:
    string = naNewStringWithFormat("%lld-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                dts.year,
                dts.mon + 1,
                dts.day + 1,
                dts.hour,
                dts.min,
                dts.sec,
                ',' - dta.shiftsign,
                dta.shifthour,
                dta.shiftmin);
    break;
  case NA_DATETIME_FORMAT_CONDENSEDDATE:
    string = naNewStringWithFormat("%lld%02d%02d",
                dts.year,
                dts.mon + 1,
                dts.day + 1);
    break;
  case NA_DATETIME_FORMAT_NATURAL:
    string = naNewStringWithFormat("%lld-%02d-%02d %02d:%02d:%02d",
                dts.year,
                dts.mon + 1,
                dts.day + 1,
                dts.hour,
                dts.min,
                dts.sec);
    break;
  }

  return string;
}










NA_DEF struct tm naMakeTMfromDateTime(const NADateTime* datetime){
  struct tm systemtimestruct;
  NADateTimeStruct dts;
  NADateTimeAttribute dta;
  naExtractDateTimeInformation(datetime, &dts, &dta);
  systemtimestruct.tm_sec = dts.sec;
  systemtimestruct.tm_min = dts.min;
  systemtimestruct.tm_hour = dts.hour;
  systemtimestruct.tm_mday = dts.day + 1;
  systemtimestruct.tm_mon = dts.mon;
  systemtimestruct.tm_year = (int)dts.year - 1900;
  systemtimestruct.tm_wday = (dta.weekday + 1) % 7;
  systemtimestruct.tm_yday = dta.dayofyear;
  systemtimestruct.tm_isdst = naHasDateTimeSummerTime(datetime)?1:0;
  #if NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    systemtimestruct.tm_gmtoff = (long)(dts.shift * NA_SECONDS_PER_MINUTE);
    if(naHasDateTimeSummerTime(datetime)){
      systemtimestruct.tm_zone = tzname[1];
    }else{
      systemtimestruct.tm_zone = tzname[0];
    }
  #endif
  return systemtimestruct;
}


NA_DEF int16 naMakeShiftFromTimeZone(const NATimeZone* timezn){
  int16 shift;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    shift = - (int16)timezn->Bias;
    if(timezn->DaylightBias){
      shift += NA_MINUTES_PER_HOUR;
    }
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    shift = -timezn->tz_minuteswest; // yes, its inverted.
    if(timezn->tz_dsttime){
      // In contrast to tm_gmtoff, the timezone struct does not automatically
      // contain the summertime shift and must be added manually.
      shift += NA_MINUTES_PER_HOUR;
    }
  #endif
  return shift;
}







#if NA_SYSTEM == NA_SYSTEM_WINDOWS

  NA_DEF NADateTime naMakeDateTimeFromFileTime(const FILETIME* filetime, const NATimeZone* timezn){
    NADateTime datetime;
    NAUInt taiperiod;
    int64 nanosecs = ((int64)filetime->dwHighDateTime << 32) | filetime->dwLowDateTime;

    datetime.errornum = NA_DATETIME_ERROR_NONE;
    datetime.nsec = (nanosecs % 10000000) * 100;  // 100-nanosecond intervals.
    datetime.sisec = nanosecs / 10000000 + NA_GREG_SECONDS_SINCE_BEGIN_1601;
    if(datetime.sisec >= 0){
      taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetime.sisec);
      datetime.sisec += (naTAIPeriods[taiperiod].startsisec - naTAIPeriods[taiperiod].startgregsec);
    }

    if(timezn){
      datetime.shift = naMakeShiftFromTimeZone(timezn);
      datetime.flags = ((timezn->DaylightBias) ? NA_DATETIME_FLAG_SUMMERTIME : 0);
    }else{
      datetime.shift = na_globaltimeshift;
      datetime.flags = ((na_globalsummertime) ? NA_DATETIME_FLAG_SUMMERTIME : 0);
    }
    return datetime;
  }

#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X

  NA_DEF struct timespec naMakeTimeSpecFromDateTime(const NADateTime* datetime){
    struct timespec timesp;
    NAUInt taiperiod = naGetTAIPeriodIndexForSISecond(datetime->sisec);
    timesp.tv_sec = (__darwin_time_t)(datetime->sisec - (naTAIPeriods[taiperiod].startsisec - naTAIPeriods[taiperiod].startgregsec));
    timesp.tv_sec -= (__darwin_time_t)NA_GREG_SECONDS_TILL_BEGIN_1970;
    timesp.tv_nsec = datetime->nsec;
    return timesp;
  }


  NA_DEF struct timeval naMakeTimeValFromDateTime(const NADateTime* datetime){
    struct timeval timevl;
    struct timespec timesp = naMakeTimeSpecFromDateTime(datetime);
    timevl.tv_sec = timesp.tv_sec;
    timevl.tv_usec = (int)(timesp.tv_nsec / 1000);
    return timevl;
  }


  NA_DEF NATimeZone naMakeTimeZoneFromDateTime(const NADateTime* datetime){
    NATimeZone timezn;
    timezn.tz_minuteswest = -datetime->shift; // yes, its inverted.
    if(naHasDateTimeSummerTime(datetime)){
      // In contrast to tm_gmtoff, the timezone struct does not automatically
      // contain the summertime shift and must be subtracted manually.
      timezn.tz_dsttime = 1;
      timezn.tz_minuteswest -= (int)NA_MINUTES_PER_HOUR;
    }else{
      timezn.tz_dsttime = 0;
    }
    return timezn;
  }


  NA_DEF NADateTime naMakeDateTimeFromTimeSpec(const struct timespec* timesp, const NATimeZone* timezn){
    NADateTime datetime;
    datetime.errornum = NA_DATETIME_ERROR_NONE;
    int64 datetimesec = timesp->tv_sec + NA_GREG_SECONDS_TILL_BEGIN_1970;
    if(datetimesec >= 0){
      NAUInt taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetimesec);
      datetime.sisec = datetimesec + (naTAIPeriods[taiperiod].startsisec - naTAIPeriods[taiperiod].startgregsec);
    }
    datetime.nsec = (int32)timesp->tv_nsec;
    if(timezn){
      datetime.shift = naMakeShiftFromTimeZone(timezn);
      datetime.flags = ((timezn->tz_dsttime) ? NA_DATETIME_FLAG_SUMMERTIME : 0);
    }else{
      datetime.shift = na_globaltimeshift;
      datetime.flags = ((na_globalsummertime) ? NA_DATETIME_FLAG_SUMMERTIME : 0);
    }
    return datetime;
  }


  NA_DEF NADateTime naMakeDateTimeFromTimeVal(const struct timeval* timevl, const NATimeZone* timezn){
    struct timespec timesp = {timevl->tv_sec, timevl->tv_usec * 1000};
    return naMakeDateTimeFromTimeSpec(&timesp, timezn);
  }


#endif




NA_DEF void naExtractDateTimeInformation(const NADateTime* datetime,
                          NADateTimeStruct* dts,
                       NADateTimeAttribute* dta){
  int64 remainingsecs;
  int64 years400;
  int64 years100;
  int64 years4;
  int64 remainingyears;
  int32 dayofyear = 0;
  NABool isleapyear;
  NABool exception100;
  NAUInt l, m, r;
  int32 d;
  int64 y;
  int32 mon;
  int64 K;
  int64 J;

  dts->year = 0;
  dts->mon = 0;
  dts->day = 0;
  dts->hour = 0;
  dts->min = 0;
  dts->sec = 0;
  dts->nsec = datetime->nsec;
  dts->shift = datetime->shift;
  dts->errornum = datetime->errornum;
  dts->flags = datetime->flags;
  remainingsecs = datetime->sisec + (dts->shift * NA_SECONDS_PER_MINUTE);
  
  if(remainingsecs < NA_START_GREGORIAN_PERIOD){
    // julian system with astronomic year numbering
    remainingsecs -= NA_YEAR_ZERO;

    years4 = remainingsecs / NA_SECONDS_IN_4_YEAR_PERIOD;
    if(remainingsecs < 0){years4--;}
    dts->year += 4 * years4;
    remainingsecs -= years4 * NA_SECONDS_IN_4_YEAR_PERIOD;
    if(remainingsecs >= NA_SECONDS_IN_LEAP_YEAR){
      dts->year++;
      remainingsecs -= NA_SECONDS_IN_LEAP_YEAR;
      
      remainingyears = remainingsecs / NA_SECONDS_IN_NORMAL_YEAR;
      dts->year += remainingyears;
      remainingsecs -= remainingyears * NA_SECONDS_IN_NORMAL_YEAR;
    }
    isleapyear = naIsLeapYearJulian(dts->year);
  }else if((remainingsecs < 0) || (remainingsecs >= naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec)){
    // gregorian system
    if(remainingsecs >= naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec){
      remainingsecs -= (naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec - naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startgregsec);
    }
    remainingsecs -= NA_YEAR_ZERO_OF_GREGORIAN_PERIOD;

    years400 = remainingsecs / NA_SECONDS_IN_400_YEAR_PERIOD;
    dts->year += 400 * years400;
    remainingsecs -= years400 * NA_SECONDS_IN_400_YEAR_PERIOD;
    
    exception100 = NA_FALSE;
    // The first 100-year period of a 400-year period has a leap day
    if(remainingsecs >= (NA_SECONDS_IN_100_YEAR_PERIOD + NA_SECONDS_PER_DAY)){
      exception100 = NA_TRUE;
      dts->year += 100;
      remainingsecs -= (NA_SECONDS_IN_100_YEAR_PERIOD + NA_SECONDS_PER_DAY);

      years100 = remainingsecs / NA_SECONDS_IN_100_YEAR_PERIOD;
      if(years100){
        dts->year += 100 * years100;
        remainingsecs -= years100 * NA_SECONDS_IN_100_YEAR_PERIOD;
      }
    }

    if(exception100){
      if(remainingsecs >= (4 * NA_SECONDS_IN_NORMAL_YEAR)){
        dts->year += 4;
        remainingsecs -= (4 * NA_SECONDS_IN_NORMAL_YEAR);
      }else if(remainingsecs >= NA_SECONDS_IN_NORMAL_YEAR){
        dts->year ++;
        remainingsecs -= NA_SECONDS_IN_NORMAL_YEAR;

        remainingyears = remainingsecs / NA_SECONDS_IN_NORMAL_YEAR;
        dts->year += remainingyears;
        remainingsecs -= remainingyears * NA_SECONDS_IN_NORMAL_YEAR;
      }
    }
    
    years4 = remainingsecs / NA_SECONDS_IN_4_YEAR_PERIOD;
    dts->year += 4 * years4;
    remainingsecs -= years4 * NA_SECONDS_IN_4_YEAR_PERIOD;
    
    if(remainingsecs >= NA_SECONDS_IN_LEAP_YEAR){
      dts->year++;
      remainingsecs -= NA_SECONDS_IN_LEAP_YEAR;

      remainingyears = remainingsecs / NA_SECONDS_IN_NORMAL_YEAR;
      dts->year += remainingyears;
      remainingsecs -= remainingyears * NA_SECONDS_IN_NORMAL_YEAR;
    }

    isleapyear = naIsLeapYearGregorian(dts->year);
  }else{
    // gregorian system with leap second information
    r = naGetTAIPeriodIndexForSISecond(remainingsecs);
    dts->year = naTAIPeriods[r].gregyear;
    remainingsecs -= naTAIPeriods[r].startsisec;

    isleapyear = naIsLeapYearGregorian(dts->year);

    switch(naTAIPeriods[r].indicator){
    case NA_START_JANUARY_FIRST:              dts->mon = 0 ;
                                              dayofyear = 0;
                                              break;
    case NA_START_JULY_FIRST:                 dts->mon = 6 ;
                                              dayofyear = (isleapyear ? 182 : 181);
                                              break;
    case NA_POSITIVE_LEAP_SECONDS_JUNE:       dts->mon = 5 ;
                                              dayofyear = (isleapyear ? 181 : 180);
                                              dts->hour = 23;
                                              dts->min = 59;
                                              dts->sec = 60;
                                              break;
    case NA_POSITIVE_LEAP_SECONDS_DECEMBER:   dts->mon = 11;
                                              dayofyear = (isleapyear ? 365 : 364);
                                              dts->hour = 23;
                                              dts->min = 59;
                                              dts->sec = 60;
                                              break;
    }
  }

  // reaching here, a base date is set as well as the remaining seconds.
  dayofyear += (int32)(remainingsecs / NA_SECONDS_PER_DAY);
  remainingsecs %= NA_SECONDS_PER_DAY;
  dts->hour += (int32)(remainingsecs / NA_SECONDS_PER_HOUR);
  remainingsecs %= NA_SECONDS_PER_HOUR;
  dts->min += (int32)(remainingsecs / NA_SECONDS_PER_MINUTE);
  remainingsecs %= NA_SECONDS_PER_MINUTE;
  dts->sec += (int32)(remainingsecs);

  // get the correct date.
  l = 0;
  r = 11;
  while(l != r){  // binary search
    m = (l+r)/2;
    if(na_cumulativemonthstartdays[2 * (m+1) + isleapyear] <= dayofyear){l = m + 1;}else{r = m;}
  }
  // r now defines the index of the month
  dts->mon = (int32)r;
  dts->day = dayofyear - na_cumulativemonthstartdays[2 * r + isleapyear];

  if(dta){
    // Fill the NADateTimeAttribute struct with every information we have.
    if(dts->shift < 0){
      dta->shiftsign = -1;
      dta->shifthour = naAbsi16(dts->shift) / (int16)NA_MINUTES_PER_HOUR;
      dta->shiftmin  = naAbsi16(dts->shift) % (int16)NA_MINUTES_PER_HOUR;
    }else{
      dta->shiftsign = +1;
      dta->shifthour = dts->shift / (int32)NA_MINUTES_PER_HOUR;
      dta->shiftmin  = dts->shift % (int32)NA_MINUTES_PER_HOUR;
    }

    dta->dayofyear = dayofyear;
    dta->isleapyear = isleapyear;
    dta->daysinmonth = na_cumulativemonthstartdays[2 * (r + 1) + dta->isleapyear] - na_cumulativemonthstartdays[2 * r + dta->isleapyear];
    dta->yearsign = (dts->year < 0)?-1:+1;

    // Weekday computation
    d = dts->day + 1;
    y = dts->year;
    mon = dts->mon + 1;
    if(mon<3){mon+=12; y--;}
    K = (((y % 100) + 100) % 100);
    if(y<0){
      J = ((y+1) / 100);
    }else{
      J = (y / 100);
    }
    if(y<0){J--;}
    if((datetime->sisec + (dts->shift * NA_SECONDS_PER_MINUTE)) < NA_START_GREGORIAN_PERIOD){
      // Julian weedkday computation
      dta->weekday = (int32)(((d + (((mon + 1) * 26) / 10) + K + (K/4) + 5 +     (6*J)) % 7) + 12) % 7;
    }else{
      // Gregorian weedkday computation
      dta->weekday = (int32)(((d + (((mon + 1) * 26) / 10) + K + (K/4) + (J/4) + (5*J)) % 7) + 12) % 7; // Zeller algorihm
    }
  }
}




NA_DEF void naExtractDateTimeUTCInformation(const NADateTime* datetime,
                             NADateTimeStruct* dts,
                          NADateTimeAttribute* dta){
  NADateTime utcdatetime = *datetime;
  naSetDateTimeZone(&utcdatetime, 0, NA_FALSE);
  naExtractDateTimeInformation(&utcdatetime, dts, dta);
}



NA_DEF void naSetDateTimeZone( NADateTime* datetime,
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


NA_DEF void naCorrectDateTimeZone( NADateTime* datetime,
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


NA_DEF void naAddDateTimeDifference(NADateTime* datetime, double difference){
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


NA_DEF NAString* naNewStringFromSecondDifference(double difference,
                                                 uint8 decimaldigits){
  uint64 allseconds, powten, decimals;
  uint8 seconds, minutes, hours;
  NAString* decimalstring;
  NAString* timestring;
  NAString* daystring;
  NAString* signstring;
  NAString* string;

  NABool needsign = NA_FALSE;
  if(difference<0){needsign = NA_TRUE; difference = -difference;}
  powten = naExp10i64(decimaldigits);
  allseconds = (uint64)(difference * (double)powten);
  decimals = allseconds % powten;
  allseconds /= powten;
  seconds = allseconds % NA_SECONDS_PER_MINUTE;
  allseconds /= NA_SECONDS_PER_MINUTE;
  minutes = allseconds % NA_MINUTES_PER_HOUR;
  allseconds /= NA_MINUTES_PER_HOUR;
  hours = allseconds % NA_HOURS_PER_DAY;
  allseconds /= NA_HOURS_PER_DAY;
  
  if(decimaldigits){
    NAString* decimalformatstring;
    decimalformatstring = naNewStringWithFormat(".%%0%dlld", decimaldigits);
    decimalstring = naNewStringWithFormat(naGetStringUTF8Pointer(decimalformatstring), decimals);
    naDelete(decimalformatstring);
  }else{
    decimalstring = naNewString();
  }
  
  timestring = naNewStringWithFormat("%02d:%02d:%02d", hours, minutes, seconds);
  
  if(allseconds){
    daystring = naNewStringWithFormat("%lldd ", allseconds);
  }else{
    daystring = naNewString();
  }
  
  if(needsign){
    signstring = naNewStringWithUTF8CStringLiteral("-");
  }else{
    signstring = naNewString();
  }
  
  string = naNewStringWithFormat("%s%s%s%s", naGetStringUTF8Pointer(signstring), naGetStringUTF8Pointer(daystring), naGetStringUTF8Pointer(timestring), naGetStringUTF8Pointer(decimalstring));
  naDelete(decimalstring);
  naDelete(timestring);
  naDelete(daystring);
  naDelete(signstring);
  return string;
}


NA_DEF NABool naHasDateTimeSummerTime(const NADateTime* datetime){
  return (datetime->flags & NA_DATETIME_FLAG_SUMMERTIME) ? NA_TRUE : NA_FALSE;
}

NA_DEF void naSetDateTimeSummertime(NADateTime* datetime, NABool summertime){
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



NA_DEF void naSetGlobalTimeShift(int16 shiftminutes, NABool summertime){
  na_globaltimeshift = shiftminutes;
  na_globalsummertime = summertime;
}


NA_DEF void naSetGlobalTimeShiftToSystemSettings(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NATimeZone curtimezone;
    GetTimeZoneInformation(&curtimezone);
    na_globaltimeshift = naMakeShiftFromTimeZone(&curtimezone);
    na_globalsummertime = ((curtimezone.DaylightBias) ? NA_TRUE : NA_FALSE);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    NATimeZone curtimezone;
    gettimeofday(NULL, &curtimezone);
    na_globaltimeshift = naMakeShiftFromTimeZone(&curtimezone);
    na_globalsummertime = ((curtimezone.tz_dsttime) ? NA_TRUE : NA_FALSE);
  #endif
}


NA_DEF int64 naGetFirstUncertainSecondNumber(){
  // The first uncertain second number is here defined to be the first second
  // of the last known TAI period.
  return naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec;
}


NA_DEF NAInt naGetLeapSecondCorrectionConstant(int64 olduncertainsecondnumber){
  NAInt taiperiod;
  if(olduncertainsecondnumber < 0){return INVALID_UNCERTAIN_SECOND_NUMBER;}
  // Note that the last entry of the structure storing all TAI periods always
  // is a non-leap-second-entry.
  if(olduncertainsecondnumber == naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec){return NO_CORRECTION_NEEDED;}
  if(olduncertainsecondnumber > naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec){return NEW_LIBRARY_IS_OLDER_THAN_BEFORE;}
  taiperiod = naGetTAIPeriodIndexForSISecond(olduncertainsecondnumber);
  // Find the earliest second which needs correction.
  while(taiperiod < NA_NUMBER_OF_TAI_PERIODS){
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE){return taiperiod;}
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER){return taiperiod;}
    taiperiod++;
  }
  // No leap second has been introduced in the meantime.
  return NO_CORRECTION_NEEDED;
}


NA_DEF void naCorrectDateTimeForLeapSeconds(NADateTime* datetime,
                                           NAInt leapsecondcorrectionconstant){
  NAUInt taiperiod;
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if(leapsecondcorrectionconstant < 0){return;}
  if(datetime->sisec < naTAIPeriods[leapsecondcorrectionconstant].startsisec){return;}
  
  // Correcting a date for leap seconds means that the siseconds stored are
  // greaterequal to an entry in the TAI periods structure which introduces
  // a leap second. When storing such a date with an earlier version of NALib,
  // no leap second introduction is assumed. Therefore, now as the leap second
  // is known, it must be added to the date.
  //
  // To do so, we subtract all previously known leap seconds...
  datetime->sisec -= (naTAIPeriods[leapsecondcorrectionconstant-1].startsisec - naTAIPeriods[leapsecondcorrectionconstant-1].startgregsec);
  // And add the correct number of leap seconds anew:
  taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetime->sisec);
  datetime->sisec += (naTAIPeriods[taiperiod].startsisec - naTAIPeriods[taiperiod].startgregsec);
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
