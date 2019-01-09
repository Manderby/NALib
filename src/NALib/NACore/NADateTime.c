
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NADateTime.h"
#include "../NAString.h"
#include "../NABinaryData.h"
#include "../NAMathOperators.h"
#include "ctype.h"


#if NA_OS == NA_OS_WINDOWS
  #include <time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_s(storage, tme);}
#elif NA_OS == NA_OS_MAC_OS_X
  #include <sys/time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_r(tme, storage);}
#else
  #error "System undefined"
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
  NA_CALENDAR_JULIAN,
  NA_CALENDAR_GREGORIAN,
  NA_CALENDAR_GREGORIAN_WITH_LEAP_SECONDS
} NACalendarSystem;

typedef enum{
  NA_START_JANUARY_FIRST,
  NA_START_JULY_FIRST,
  NA_POSITIVE_LEAP_SECONDS_JUNE,
  NA_POSITIVE_LEAP_SECONDS_DECEMBER
} NALeapSecondCorrectionIndicator;

typedef struct{
  int64 startgregsec;
  int64 startsisec;
  int16 gregyear;
  NALeapSecondCorrectionIndicator indicator;
} NATAIPeriod;

// Leap second information:
#define NA_NUMBER_OF_TAI_PERIODS 102

// This table stores all leap second entries since 1958. Every year has at
// least 1 entry. Every entry defines, what the number of its first second is.
// Additionally, it is defined, what the seconds in that period are. They can
// be regular seconds denoting a start date, or they can be lead seconds. This
// structure is future-proof for at least some centuries, as it will be able to
// map leap seconds even if they are introduced every second. The same goes for
// negative leap seconds. But there have not been introduced any so far.
NATAIPeriod naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS] = {
  {naMakeUInt64WithLiteralLo(         0), naMakeUInt64WithLiteralLo(         0), 1958, NA_START_JANUARY_FIRST},           // [0]
  {naMakeUInt64WithLiteralLo(  31536000), naMakeUInt64WithLiteralLo(  31536000), 1959, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(  63072000), naMakeUInt64WithLiteralLo(  63072000), 1960, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(  94694400), naMakeUInt64WithLiteralLo(  94694400), 1961, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 126230400), naMakeUInt64WithLiteralLo( 126230400), 1962, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 157766400), naMakeUInt64WithLiteralLo( 157766400), 1963, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 189302400), naMakeUInt64WithLiteralLo( 189302400), 1964, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 220924800), naMakeUInt64WithLiteralLo( 220924800), 1965, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 252460800), naMakeUInt64WithLiteralLo( 252460800), 1966, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 283996800), naMakeUInt64WithLiteralLo( 283996800), 1967, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 315532800), naMakeUInt64WithLiteralLo( 315532800), 1968, NA_START_JANUARY_FIRST},           // [10]
  {naMakeUInt64WithLiteralLo( 347155200), naMakeUInt64WithLiteralLo( 347155200), 1969, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 378691200), naMakeUInt64WithLiteralLo( 378691200), 1970, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 410227200), naMakeUInt64WithLiteralLo( 410227200), 1971, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 441763200), naMakeUInt64WithLiteralLo( 441763200), 1971, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 10
  {naMakeUInt64WithLiteralLo( 441763200), naMakeUInt64WithLiteralLo( 441763210), 1972, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 457488000), naMakeUInt64WithLiteralLo( 457488010), 1972, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo( 457488000), naMakeUInt64WithLiteralLo( 457488011), 1972, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo( 473385600), naMakeUInt64WithLiteralLo( 473385611), 1972, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 473385600), naMakeUInt64WithLiteralLo( 473385612), 1973, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 504921600), naMakeUInt64WithLiteralLo( 504921612), 1973, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [20] + 1
  {naMakeUInt64WithLiteralLo( 504921600), naMakeUInt64WithLiteralLo( 504921613), 1974, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 536457600), naMakeUInt64WithLiteralLo( 536457613), 1974, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 536457600), naMakeUInt64WithLiteralLo( 536457614), 1975, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 567993600), naMakeUInt64WithLiteralLo( 567993614), 1975, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 567993600), naMakeUInt64WithLiteralLo( 567993615), 1976, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 599616000), naMakeUInt64WithLiteralLo( 599616015), 1976, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 599616000), naMakeUInt64WithLiteralLo( 599616016), 1977, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 631152000), naMakeUInt64WithLiteralLo( 631152016), 1977, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 631152000), naMakeUInt64WithLiteralLo( 631152017), 1978, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 662688000), naMakeUInt64WithLiteralLo( 662688017), 1978, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [30] + 1
  {naMakeUInt64WithLiteralLo( 662688000), naMakeUInt64WithLiteralLo( 662688018), 1979, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 694224000), naMakeUInt64WithLiteralLo( 694224018), 1979, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 694224000), naMakeUInt64WithLiteralLo( 694224019), 1980, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 725846400), naMakeUInt64WithLiteralLo( 725846419), 1981, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 741484800), naMakeUInt64WithLiteralLo( 741484819), 1981, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo( 741484800), naMakeUInt64WithLiteralLo( 741484820), 1981, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo( 757382400), naMakeUInt64WithLiteralLo( 757382420), 1982, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 773020800), naMakeUInt64WithLiteralLo( 773020820), 1982, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo( 773020800), naMakeUInt64WithLiteralLo( 773020821), 1982, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo( 788918400), naMakeUInt64WithLiteralLo( 788918421), 1983, NA_START_JANUARY_FIRST},           // [40]
  {naMakeUInt64WithLiteralLo( 804556800), naMakeUInt64WithLiteralLo( 804556821), 1983, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo( 804556800), naMakeUInt64WithLiteralLo( 804556822), 1983, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo( 820454400), naMakeUInt64WithLiteralLo( 820454422), 1984, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 852076800), naMakeUInt64WithLiteralLo( 852076822), 1985, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 867715200), naMakeUInt64WithLiteralLo( 867715222), 1985, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo( 867715200), naMakeUInt64WithLiteralLo( 867715223), 1985, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo( 883612800), naMakeUInt64WithLiteralLo( 883612823), 1986, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 915148800), naMakeUInt64WithLiteralLo( 915148823), 1987, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo( 946684800), naMakeUInt64WithLiteralLo( 946684823), 1987, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo( 946684800), naMakeUInt64WithLiteralLo( 946684824), 1988, NA_START_JANUARY_FIRST},           // [50]
  {naMakeUInt64WithLiteralLo( 978307200), naMakeUInt64WithLiteralLo( 978307224), 1989, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1009843200), naMakeUInt64WithLiteralLo(1009843224), 1989, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1009843200), naMakeUInt64WithLiteralLo(1009843225), 1990, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1041379200), naMakeUInt64WithLiteralLo(1041379225), 1990, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1041379200), naMakeUInt64WithLiteralLo(1041379226), 1991, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1072915200), naMakeUInt64WithLiteralLo(1072915226), 1992, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1088640000), naMakeUInt64WithLiteralLo(1088640026), 1992, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo(1088640000), naMakeUInt64WithLiteralLo(1088640027), 1992, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo(1104537600), naMakeUInt64WithLiteralLo(1104537627), 1993, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1120176000), naMakeUInt64WithLiteralLo(1120176027), 1993, NA_POSITIVE_LEAP_SECONDS_JUNE},    // [60] + 1
  {naMakeUInt64WithLiteralLo(1120176000), naMakeUInt64WithLiteralLo(1120176028), 1993, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo(1136073600), naMakeUInt64WithLiteralLo(1136073628), 1994, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1151712000), naMakeUInt64WithLiteralLo(1151712028), 1994, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo(1151712000), naMakeUInt64WithLiteralLo(1151712029), 1994, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo(1167609600), naMakeUInt64WithLiteralLo(1167609629), 1995, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1199145600), naMakeUInt64WithLiteralLo(1199145629), 1995, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1199145600), naMakeUInt64WithLiteralLo(1199145630), 1996, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1230768000), naMakeUInt64WithLiteralLo(1230768030), 1997, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1246406400), naMakeUInt64WithLiteralLo(1246406430), 1997, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo(1246406400), naMakeUInt64WithLiteralLo(1246406431), 1997, NA_START_JULY_FIRST},              // [70]
  {naMakeUInt64WithLiteralLo(1262304000), naMakeUInt64WithLiteralLo(1262304031), 1998, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1293840000), naMakeUInt64WithLiteralLo(1293840031), 1998, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1293840000), naMakeUInt64WithLiteralLo(1293840032), 1999, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1325376000), naMakeUInt64WithLiteralLo(1325376032), 2000, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1356998400), naMakeUInt64WithLiteralLo(1356998432), 2001, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1388534400), naMakeUInt64WithLiteralLo(1388534432), 2002, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1420070400), naMakeUInt64WithLiteralLo(1420070432), 2003, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1451606400), naMakeUInt64WithLiteralLo(1451606432), 2004, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1483228800), naMakeUInt64WithLiteralLo(1483228832), 2005, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1514764800), naMakeUInt64WithLiteralLo(1514764832), 2005, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [80] + 1
  {naMakeUInt64WithLiteralLo(1514764800), naMakeUInt64WithLiteralLo(1514764833), 2006, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1546300800), naMakeUInt64WithLiteralLo(1546300833), 2007, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1577836800), naMakeUInt64WithLiteralLo(1577836833), 2008, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1609459200), naMakeUInt64WithLiteralLo(1609459233), 2008, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1609459200), naMakeUInt64WithLiteralLo(1609459234), 2009, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1640995200), naMakeUInt64WithLiteralLo(1640995234), 2010, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1672531200), naMakeUInt64WithLiteralLo(1672531234), 2011, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1704067200), naMakeUInt64WithLiteralLo(1704067234), 2012, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1719792000), naMakeUInt64WithLiteralLo(1719792034), 2012, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo(1719792000), naMakeUInt64WithLiteralLo(1719792035), 2012, NA_START_JULY_FIRST},              // [90]
  {naMakeUInt64WithLiteralLo(1735689600), naMakeUInt64WithLiteralLo(1735689635), 2013, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1767225600), naMakeUInt64WithLiteralLo(1767225635), 2014, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1798761600), naMakeUInt64WithLiteralLo(1798761635), 2015, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1814400000), naMakeUInt64WithLiteralLo(1814400035), 2015, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeUInt64WithLiteralLo(1814400000), naMakeUInt64WithLiteralLo(1814400036), 2015, NA_START_JULY_FIRST},
  {naMakeUInt64WithLiteralLo(1830297600), naMakeUInt64WithLiteralLo(1830297636), 2016, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1861920000), naMakeUInt64WithLiteralLo(1861920036), 2016, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeUInt64WithLiteralLo(1861920000), naMakeUInt64WithLiteralLo(1861920037), 2017, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1893456000), naMakeUInt64WithLiteralLo(1893456037), 2018, NA_START_JANUARY_FIRST},
  {naMakeUInt64WithLiteralLo(1924992000), naMakeUInt64WithLiteralLo(1924992037), 2019, NA_START_JANUARY_FIRST},           // [100]
  {naMakeUInt64WithLiteralLo(1940630400), naMakeUInt64WithLiteralLo(1940630437), 2019, NA_START_JULY_FIRST},
  // the last entry is the first date with unknown future leap seconds.
  // everything up and including that date is known.
};


// NA_DATETIME_SISEC_START_GREGORIAN_PERIOD denotes the sisec relative to the
// start of the TAI period (sisec == 0) denoting the start of the gregorian
// time period.
// TAI period starts at January 1st 1958
// Gregorian time period starts on Friday, October 15th 1582
// Julian time period ends with Thursday, October 4th 1582 (inclusive).
//#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD            (- NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 4LL * NA_SECONDS_IN_4_YEAR_PERIOD - NA_SECONDS_IN_NORMAL_YEAR - (31LL+30LL+31LL-14LL) * NA_SECONDS_PER_DAY)
//#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD    -11840601600LL
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_HI 0xfffffffd
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_LO 0x3e3ec200
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD    naCastUInt64ToInt64(naMakeUInt64(NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_HI, NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_LO))

// NA_DATETIME_SISEC_JULIAN_YEAR_ZERO denotes the sisec relative to the start of the
// TAI period (sisec == 0) denoting the beginning of the julian year 0 when
// using astronomical year numbering.
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering equals year 1 BC.
// This assumes the planned leap years by cesar.
//#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO                 (NA_DATETIME_SISEC_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+4LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 395LL * NA_SECONDS_IN_4_YEAR_PERIOD)
//#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO                 -61788700800
#define NA_DATETIME_SISEC_YEAR_ZERO_HI              0xfffffff1
#define NA_DATETIME_SISEC_YEAR_ZERO_LO              0x9d1b3f80
#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO          naCastUInt64ToInt64(naMakeUInt64(NA_DATETIME_SISEC_YEAR_ZERO_HI, NA_DATETIME_SISEC_YEAR_ZERO_LO))

// NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO denotes the sisec relative to the start of the
// TAI period (sisec == 0) denoting the beginning of the gregorian year 0 when
// using astronomical year numbering.
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering equals year 1 BC.
//#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     (NA_DATETIME_SISEC_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+14LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 19LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 3LL * NA_SECONDS_IN_400_YEAR_PERIOD)
//#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     -61788528000
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_HI  0xfffffff1
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_LO  0x9d1de280
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     naCastUInt64ToInt64(naMakeUInt64(NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_HI, NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_LO))

// the UNIX system time has no leap seconds and is nulled in 1970
#define NA_DATETIME_SISEC_UNIX_YEAR_ZERO          naMulInt64(NA_SECONDS_IN_4_YEAR_PERIOD, naMakeInt64WithLo(3))

// the Windows filetime has no leap seconds and is nulled in 1601
//#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      (-NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 2LL * NA_SECONDS_IN_100_YEAR_PERIOD - 24LL * NA_SECONDS_IN_4_YEAR_PERIOD - 3LL * NA_SECONDS_IN_NORMAL_YEAR)
//#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      -11265782400
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_HI   0xfffffffd
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_LO   0x6081cd80
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      naCastUInt64ToInt64(naMakeUInt64(NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_HI, NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_LO))


// Prototypes:
NAInt naGetTAIPeriodIndexForSISecond(int64 sisecond);
NAInt naGetLatestTAIPeriodIndexForGregorianSecond(int64 gregsecond);
NADateTime naMakeDateTimeNow(void);
void naSetGlobalTimeShiftToSystemSettings(void);
int64 naGetFirstUncertainSecondNumber(void);




NA_DEF NAInt naGetTAIPeriodIndexForSISecond(int64 sisecond){
  NAInt l, r, m;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 3].startsisec, sisecond)){
    if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 2].startsisec, sisecond)){
      if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec, sisecond)){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_NUMBER_OF_TAI_PERIODS - 1;
      }
      return NA_NUMBER_OF_TAI_PERIODS - 2;
    }
    return NA_NUMBER_OF_TAI_PERIODS - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(naSmallerInt64(sisecond, NA_ZERO_64)){
    r = -1;
  }else{
    // In all other cases, perform a binary search in all TAI periods.
    l = 0;
    r = NA_NUMBER_OF_TAI_PERIODS - 4;
    while(l != r){  // binary search
      m = (l+r)/2;
      if(naSmallerEqualInt64(naTAIPeriods[m + 1].startsisec, sisecond)){l = m + 1;}else{r = m;}
    }
    // l or r now define the index of the latest NATAIPeriod.
  }
  return r;
}



NA_DEF NAInt naGetLatestTAIPeriodIndexForGregorianSecond(int64 gregsecond){
  NAInt l, r, m;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 3].startgregsec, gregsecond)){
    if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 2].startgregsec, gregsecond)){
      if(naSmallerEqualInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startgregsec, gregsecond)){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_NUMBER_OF_TAI_PERIODS - 1;
      }
      return NA_NUMBER_OF_TAI_PERIODS - 2;
    }
    return NA_NUMBER_OF_TAI_PERIODS - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(naSmallerInt64(gregsecond, NA_ZERO_64)){
    r = -1;
  }else{
    // In all other cases, perform a binary search in all TAI periods.
    l = 0;
    r = NA_NUMBER_OF_TAI_PERIODS - 4;
    while(l != r){  // binary search
      m = (l+r)/2;
      if(naSmallerEqualInt64(naTAIPeriods[m + 1].startgregsec, gregsecond)){l = m + 1;}else{r = m;}
    }
    // l or r now define the index of the latest NATAIPeriod.
  }
  return r;
}



NA_DEF int32 naGetMonthNumberWithEnglishAbbreviation(const NAString* str){
  int32 i;
  int32 monthindex = -1;
  for(i=0; i<NA_MONTHS_PER_YEAR; i++){
    if(naEqualStringToUTF8CStringLiteral(str, na_monthenglishabbreviationnames[i], NA_TRUE)){
      monthindex = i;
      break;
    }
  }
  #ifndef NDEBUG
    if(monthindex == -1)
      naError("naGetMonthNumberWithEnglishAbbreviation", "Month abbreviation unknown. Returning -1.");
  #endif
  return monthindex;
}


NA_DEF int32 naGetMonthNumberFromUTF8CStringLiteral(const NAUTF8Char* str){
  int32 i;
  int32 monthindex = -1;
  if(naStrlen(str)){
    for(i=0; i<NA_MONTHS_PER_YEAR; i++){
      if(naEqualUTF8CStringLiterals(str, na_monthenglishnames[i], 0, NA_FALSE)){
        monthindex = i;
        break;
      }
    }
    if(monthindex == -1){
      for(i=0; i<NA_MONTHS_PER_YEAR; i++){
        if(naEqualUTF8CStringLiterals(str, na_monthenglishabbreviationnames[i], 0, NA_FALSE)){
          monthindex = i;
          break;
        }
      }
    }
    if(monthindex == -1){
      if(isdigit((const char)str[0])){
        int32 returnint;
        NAString* numberstring = naNewStringWithUTF8CStringLiteral(str);
        returnint = naParseStringInt32(numberstring) - 1;
        naDelete(numberstring);
        if(returnint >= 0 && returnint < 12){monthindex = returnint;}
      }
    }
  }
  #ifndef NDEBUG
    if(monthindex == -1)
      naError("naGetMonthNumberFromUTF8CStringLiteral", "Month unknown. Returning -1.");
  #endif
  return monthindex;
}







NA_DEF NADateTime naMakeDateTimeNow(){
//  printf("%llx %lld\n", NA_DATETIME_SISEC_START_GREGORIAN_PERIOD, NA_DATETIME_SISEC_START_GREGORIAN_PERIOD);
//  printf("%llx %lld\n", NA_DATETIME_SISEC_JULIAN_YEAR_ZERO, NA_DATETIME_SISEC_JULIAN_YEAR_ZERO);
//  printf("%llx %lld\n", NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO, NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO);
//  printf("%016llx %+lld\n", NA_DATETIME_SISEC_UNIX_YEAR_ZERO, NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
//  printf("%llx %lld\n", NA_DATETIME_SISEC_FILETIME_YEAR_ZERO, NA_DATETIME_SISEC_FILETIME_YEAR_ZERO);
  #if NA_OS == NA_OS_WINDOWS
    FILETIME filetime;
    NATimeZone timezone;
    GetSystemTimeAsFileTime(&filetime);
    GetTimeZoneInformation(&timezone);
    return naMakeDateTimeFromFileTime(&filetime, &timezone);
  #elif NA_OS == NA_OS_MAC_OS_X
    struct timeval curtime;
    NATimeZone curtimezone;
    gettimeofday(&curtime, &curtimezone);
    return naMakeDateTimeFromTimeVal(&curtime, &curtimezone);
  #endif
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

  if((naEqualInt64(dts->year, NA_DATETIME_YEAR_1582)) && (dts->mon == 9) && (dts->day > 3) && (dts->day < 14)){
    datetime.errornum = NA_DATETIME_ERROR_JULIAN_GREGORIAN_CHASM;
  }
  if((naSmallerInt64(dts->year, NA_DATETIME_YEAR_1582)) || ((naEqualInt64(dts->year, NA_DATETIME_YEAR_1582)) && ((dts->mon < 9) || ((dts->mon == 9) && (dts->day < 14))))){
    // julian system
    datetime.sisec = NA_DATETIME_SISEC_JULIAN_YEAR_ZERO;
    isleap = naIsLeapYearJulian(dts->year);
    calendarsystem = NA_CALENDAR_JULIAN;
  }else{
    // gregorian system
    datetime.sisec = NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO;
    years400 = naDivInt64(remainingyears, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD);
    datetime.sisec = naAddInt64(datetime.sisec, naMulInt64(years400, NA_SECONDS_IN_400_YEAR_PERIOD));
    remainingyears = naSubInt64(remainingyears, naMulInt64(years400, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD));
    years100 = naDivInt64(remainingyears, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD);
    datetime.sisec = naAddInt64(datetime.sisec, naMulInt64(years100, NA_SECONDS_IN_100_YEAR_PERIOD));
    remainingyears = naSubInt64(remainingyears, naMulInt64(years100, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD));
    isleap = naIsLeapYearGregorian(dts->year);
    if(naSmallerInt64(dts->year, NA_DATETIME_YEAR_1958)){
      calendarsystem = NA_CALENDAR_GREGORIAN;
    }else{
      calendarsystem = NA_CALENDAR_GREGORIAN_WITH_LEAP_SECONDS;
    }
  }
  years4 = naDivInt64(remainingyears, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD);
  if(naSmallerInt64(remainingyears, NA_ZERO_64)){naDecInt64(years4);}
  datetime.sisec = naAddInt64(datetime.sisec, naMulInt64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));
  remainingyears = naSubInt64(remainingyears, naMulInt64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
  if(!naEqualInt64(remainingyears, NA_ZERO_64)){
    datetime.sisec = naAddInt64(datetime.sisec, NA_SECONDS_IN_LEAP_YEAR); naDecInt64(remainingyears);
    datetime.sisec = naAddInt64(datetime.sisec, naMulInt64(remainingyears, NA_SECONDS_IN_NORMAL_YEAR));
  }
  if((dts->mon < 0) || (dts->mon > 11)){datetime.errornum = NA_DATETIME_ERROR_INVALID_MONTH_NUMBER;}
  datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(na_cumulativemonthstartdays[2 * dts->mon + (int32)isleap] * NA_SECONDS_PER_DAY));
  if((dts->day < 0) || ((na_cumulativemonthstartdays[2 * dts->mon + (int32)isleap] + dts->day) >= na_cumulativemonthstartdays[2 * (dts->mon + 1) + (int32)isleap])){datetime.errornum = NA_DATETIME_ERROR_INVALID_DAY_NUMBER;}
  datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(dts->day * NA_SECONDS_PER_DAY));
  if((dts->hour < 0) || (dts->hour > 23)){datetime.errornum = NA_DATETIME_ERROR_INVALID_HOUR_NUMBER;}
  datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(dts->hour * NA_SECONDS_PER_HOUR));
  if((dts->min < 0) || (dts->min > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_MINUTE_NUMBER;}
  datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(dts->min * NA_SECONDS_PER_MINUTE));
  if(calendarsystem == NA_CALENDAR_GREGORIAN_WITH_LEAP_SECONDS){
    if(naGreaterEqualInt64(datetime.sisec, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec)){
      if((dts->sec < 0) || (dts->sec > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
      datetime.sisec = naAddInt64(datetime.sisec, naSubInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startgregsec));
    }else{
      NAInt r = naGetLatestTAIPeriodIndexForGregorianSecond(datetime.sisec);
      // r now defines the index of the NATAIPeriod
      datetime.sisec = naAddInt64(datetime.sisec, naSubInt64(naTAIPeriods[r].startsisec, naTAIPeriods[r].startgregsec));
      datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(dts->sec));
      if((r < NA_NUMBER_OF_TAI_PERIODS-1) && naGreaterEqualInt64(datetime.sisec, naTAIPeriods[r+1].startsisec)){
        if((naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE) || (naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER)){
          if((r+2 < NA_NUMBER_OF_TAI_PERIODS) && naGreaterEqualInt64(datetime.sisec, naTAIPeriods[r+2].startsisec)){ // todo. What is wrong here?
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
      datetime.sisec = naSubInt64(datetime.sisec, naMakeInt64WithLo(dts->sec));
    }
  }else{
    if((dts->sec < 0) || (dts->sec > 59)){datetime.errornum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
  }
  datetime.sisec = naAddInt64(datetime.sisec, naMakeInt64WithLo(dts->sec));
  datetime.sisec = naSubInt64(datetime.sisec, naMakeInt64WithLo(dts->shift * NA_SECONDS_PER_MINUTE));
  datetime.nsec  = dts->nsec;
  datetime.shift = dts->shift;
  datetime.flags = dts->flags;

  return datetime;
}






NA_DEF NADateTime naMakeDateTimeFromString(const NAString* string, NAAscDateTimeFormat format){
  NAString* str;
  NADateTimeStruct dts;
  NAString* token;
  int16 int16value;
  NABufferIterator iter;

  dts.nsec = 0;

  str = naNewStringExtraction(string, 0, -1);
  iter = naMakeBufferAccessor(naGetStringBufferMutable(str));

  switch(format){
  case NA_DATETIME_FORMAT_APACHE:
    dts.day = naParseBufferInt32(&iter, NA_TRUE);
    token = naParseBufferTokenWithDelimiter(&iter, '/');
    dts.mon = naGetMonthNumberWithEnglishAbbreviation(token);
    naDelete(token);
    dts.year = naParseBufferInt64(&iter, NA_TRUE);

    dts.hour = naParseBufferInt32(&iter, NA_TRUE);
    dts.min = naParseBufferInt32(&iter, NA_TRUE);
    dts.sec = naParseBufferInt32(&iter, NA_TRUE);

    // The remaining string contains the time shift
    int16value = naParseBufferInt16(&iter, NA_FALSE);
    dts.shift = (int16value / 100) * NA_MINUTES_PER_HOUR;
    dts.shift += int16value % 100;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT:
    dts.year = naParseBufferInt64(&iter, NA_TRUE);
    dts.mon = naParseBufferInt32(&iter, NA_TRUE) - 1;
    dts.day = naParseBufferInt32(&iter, NA_TRUE) - 1;

    dts.hour = naParseBufferInt32(&iter, NA_TRUE);
    dts.min = naParseBufferInt32(&iter, NA_TRUE);
    dts.sec = naParseBufferInt32(&iter, NA_FALSE);

    dts.shift = naParseBufferInt16(&iter, NA_TRUE) * NA_MINUTES_PER_HOUR;
    if(dts.shift < 0){
      dts.shift -= naParseBufferInt16(&iter, NA_FALSE);
    }else{
      dts.shift += naParseBufferInt16(&iter, NA_FALSE);
    }
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_CONDENSEDDATE:
    token = naNewStringExtraction(str, 0, -5);
    dts.year = naParseStringInt64(token);
    naDelete(token);
    token = naNewStringExtraction(str, -4, -3);
    dts.mon = naParseStringInt32(token);
    naDelete(token);
    token = naNewStringExtraction(str, -2, -1);
    dts.day = naParseStringInt32(token);
    naDelete(token);
    dts.hour = 0;
    dts.min = 0;
    dts.sec = 0;
    dts.shift = 0;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_NATURAL:
    dts.year = naParseBufferInt64(&iter, NA_TRUE);
    dts.mon = naParseBufferInt32(&iter, NA_TRUE);
    dts.day = naParseBufferInt32(&iter, NA_TRUE);

    dts.hour = naParseBufferInt32(&iter, NA_TRUE);
    dts.min = naParseBufferInt32(&iter, NA_TRUE);
    dts.sec = naParseBufferInt32(&iter, NA_FALSE);

    dts.shift = 0;
    dts.flags = 0;
    break;
  }

  naClearBufferIterator(&iter);
  return naMakeDateTimeWithDateTimeStruct(&dts);
}




NA_DEF NADateTime naMakeDateTimeFromBuffer(NABuffer* buffer, NABinDateTimeFormat format){
  NADateTimeStruct dts;
  NABufferIterator iter = naMakeBufferAccessor(buffer);

  dts.nsec = 0;

  switch(format){
  case NA_DATETIME_FORMAT_ICC_PROFILE:
    // ICC section 5.1.1, page 4, dateTimeNumber
    dts.year  = naMakeInt64WithLo(naReadBufferu16(&iter));
    dts.mon   = naReadBufferu16(&iter);
    dts.day   = naReadBufferu16(&iter);
    dts.hour  = naReadBufferu16(&iter);
    dts.min   = naReadBufferu16(&iter);
    dts.sec   = naReadBufferu16(&iter);
    dts.shift = 0;
    dts.flags = 0;
    break;

  case NA_DATETIME_FORMAT_PNG:
    dts.year  = naMakeInt64WithLo(naReadBufferu16(&iter));
    dts.mon   = naReadBufferu8(&iter);
    dts.day   = naReadBufferu8(&iter);
    dts.hour  = naReadBufferu8(&iter);
    dts.min   = naReadBufferu8(&iter);
    dts.sec   = naReadBufferu8(&iter);
    dts.shift = 0;
    dts.flags = 0;
    break;
  }

  naClearBufferIterator(&iter);
  return naMakeDateTimeWithDateTimeStruct(&dts);
}



NA_DEF const char* naGetDateTimeErrorString(uint8 errornum){
  if(errornum >= NA_DATETIME_ERROR_COUNT){
    #ifndef NDEBUG
      naError("naGetDateTimeErrorString", "Error number invalid");
    #endif
    return NA_NULL;
  }
  return na_datetime_errorstrings[errornum];
}



//NA_DEF NAByteArray* naInitByteArrayFromDateTime( NAByteArray* bytearray, const NADateTime* datetime, NABinDateTimeFormat format){
//  uint16 valueu16;
//  NAByte* ptr;
//  NADateTimeStruct dts;
//  NADateTimeAttribute dta;
//  naExtractDateTimeInformation(datetime, &dts, &dta);
//
//  switch(format){
//  case NA_DATETIME_FORMAT_ICC_PROFILE:
//    // ICC section 5.1.1, page 4, dateTimeNumber
//    bytearray = naInitByteArrayWithBytesize(bytearray, 12);
//    ptr = naGetByteArrayMutablePointer(bytearray);
//
//    valueu16 = (uint16)dts.year;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[ 0]), &valueu16);
//    valueu16 = (uint16)dts.mon;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[ 2]), &valueu16);
//    valueu16 = (uint16)dts.day;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[ 4]), &valueu16);
//    valueu16 = (uint16)dts.hour;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[ 6]), &valueu16);
//    valueu16 = (uint16)dts.min;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[ 8]), &valueu16);
//    valueu16 = (uint16)dts.sec;
//    naConvertNativeBig16(&valueu16);
//    naCopy16(&(ptr[10]), &valueu16);
//    break;
//
//  case NA_DATETIME_FORMAT_PNG:
////    dts.year  = naReadBufferu16(buffer);
////    dts.mon   = naReadBufferu8(buffer);
////    dts.day   = naReadBufferu8(buffer);
////    dts.hour  = naReadBufferu8(buffer);
////    dts.min   = naReadBufferu8(buffer);
////    dts.sec   = naReadBufferu8(buffer);
////    dts.shift = 0;
////    dts.flags = 0;
//    break;
//
//  }
//
//  return bytearray;
//}



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
  default:
    string = naNewString();
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
  systemtimestruct.tm_year = naCastInt64ToInt32(dts.year) - 1900;
  systemtimestruct.tm_wday = (dta.weekday + 1) % 7;
  systemtimestruct.tm_yday = dta.dayofyear;
  systemtimestruct.tm_isdst = naHasDateTimeSummerTime(datetime)?1:0;
  #if NA_OS == NA_OS_MAC_OS_X
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
  #if NA_OS == NA_OS_WINDOWS
    shift = - (int16)timezn->Bias;
    if(timezn->DaylightBias){
      shift += NA_MINUTES_PER_HOUR;
    }
  #elif NA_OS == NA_OS_MAC_OS_X
    shift = (int16)-timezn->tz_minuteswest; // yes, its inverted.
    if(timezn->tz_dsttime){
      // In contrast to tm_gmtoff, the timezone struct does not automatically
      // contain the summertime shift and must be added manually.
      shift += NA_MINUTES_PER_HOUR;
    }
  #endif
  return shift;
}







#if NA_OS == NA_OS_WINDOWS

  NA_DEF NADateTime naMakeDateTimeFromFileTime(const FILETIME* filetime, const NATimeZone* timezn){
    NADateTime datetime;
    NAInt taiperiod;
    int64 nanosecs = naCastUInt64ToInt64(naMakeUInt64(filetime->dwHighDateTime, filetime->dwLowDateTime));

    datetime.errornum = NA_DATETIME_ERROR_NONE;
    datetime.nsec = naCastInt64ToInt32(naMulInt64(naModInt64(nanosecs, naMakeInt64WithLo(10000000)), naMakeInt64WithLo(100)));  // 100-nanosecond intervals.
    datetime.sisec = naAddInt64(naDivInt64(nanosecs, naMakeInt64WithLo(10000000)), NA_DATETIME_SISEC_FILETIME_YEAR_ZERO);
    if(naGreaterEqualInt64(datetime.sisec, NA_ZERO_64)){
      taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetime.sisec);
      datetime.sisec = naAddInt64(datetime.sisec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec));
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

#elif NA_OS == NA_OS_MAC_OS_X

  NA_DEF struct timespec naMakeTimeSpecFromDateTime(const NADateTime* datetime){
    struct timespec timesp;
    NAInt taiperiod = naGetTAIPeriodIndexForSISecond(datetime->sisec);
    #if NA_TYPE_NATIVE_LONG_INT_BITS == 32
      timesp.tv_sec = (__darwin_time_t)naCastInt64ToInt32(naSubInt64(datetime->sisec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec)));
      timesp.tv_sec -= (__darwin_time_t)naCastInt64ToInt32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
    #else
      #if !defined NA_TYPE_INT64
        // We fall back to 32 bits as there simply is no solution to this problem.
        #error "impossible to convert int64. Falling back to int32"
        timesp.tv_sec = (__darwin_time_t)naCastInt64ToInt32(naSubInt64(datetime->sisec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec)));
        timesp.tv_sec -= (__darwin_time_t)naCastInt64ToInt32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
      #else
        timesp.tv_sec = (__darwin_time_t)(naSubInt64(datetime->sisec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec)));
        timesp.tv_sec -= (__darwin_time_t)NA_DATETIME_SISEC_UNIX_YEAR_ZERO;
      #endif
    #endif
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
    int64 datetimesec;
    datetime.errornum = NA_DATETIME_ERROR_NONE;
    #if NA_TYPE_NATIVE_LONG_INT_BITS == 32
      datetimesec = naMakeInt64WithLo((int32)timesp->tv_sec + naCastInt64ToInt32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO));
    #else
      #if !defined NA_TYPE_INT64
        // We fall back to 32 bits as there simply is no solution to this problem.
        #error "impossible to convert int64. Falling back to int32"
        datetimesec = naMakeInt64WithLo(timesp->tv_sec + naCastInt64ToInt32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO));
      #else
        datetimesec = naAddInt64(timesp->tv_sec, naMakeInt64WithLo(NA_DATETIME_SISEC_UNIX_YEAR_ZERO));
      #endif
    #endif
    if(naGreaterInt64(datetimesec, NA_ZERO_64)){
      NAInt taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetimesec);
      datetime.sisec = naAddInt64(datetimesec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec));
    }else{
      datetime.sisec = NA_ZERO_64; // todo
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
    struct timespec timesp;
    timesp.tv_sec  = timevl->tv_sec;
    timesp.tv_nsec = timevl->tv_usec * 1000;
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
  NAInt l, m, r;
  int32 d;
  int64 y;
  int32 mon;
  int64 K;
  int64 J;

  dts->year = NA_ZERO_64;
  dts->mon = 0;
  dts->day = 0;
  dts->hour = 0;
  dts->min = 0;
  dts->sec = 0;
  dts->nsec = datetime->nsec;
  dts->shift = datetime->shift;
  dts->errornum = datetime->errornum;
  dts->flags = datetime->flags;
  remainingsecs = naAddInt64(datetime->sisec, naMakeInt64WithLo(dts->shift * NA_SECONDS_PER_MINUTE));

  if(naSmallerInt64(remainingsecs, NA_DATETIME_SISEC_START_GREGORIAN_PERIOD)){
    // julian system with astronomic year numbering
    remainingsecs = naSubInt64(remainingsecs, NA_DATETIME_SISEC_JULIAN_YEAR_ZERO);

    years4 = naDivInt64(remainingsecs, NA_SECONDS_IN_4_YEAR_PERIOD);
    if(naSmallerInt64(remainingsecs, NA_ZERO_64)){naDecInt64(years4);}
    dts->year = naAddInt64(dts->year, naMulInt64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
    remainingsecs = naSubInt64(remainingsecs, naMulInt64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));
    if(naGreaterEqualInt64(remainingsecs, NA_SECONDS_IN_LEAP_YEAR)){
      naIncInt64(dts->year);
      remainingsecs = naSubInt64(remainingsecs, NA_SECONDS_IN_LEAP_YEAR);

      remainingyears = naDivInt64(remainingsecs, NA_SECONDS_IN_NORMAL_YEAR);
      dts->year = naAddInt64(dts->year, remainingyears);
      remainingsecs = naSubInt64(remainingsecs, naMulInt64(remainingyears, NA_SECONDS_IN_NORMAL_YEAR));
    }
    isleapyear = naIsLeapYearJulian(dts->year);
  }else if(naSmallerInt64(remainingsecs, NA_ZERO_64) || naGreaterEqualInt64(remainingsecs, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec)){
    // gregorian system
    if(naGreaterEqualInt64(remainingsecs, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec)){
      remainingsecs = naSubInt64(remainingsecs, naSubInt64(naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startsisec, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS-1].startgregsec));
    }
    remainingsecs = naSubInt64(remainingsecs, NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO);

    years400 = naDivInt64(remainingsecs, NA_SECONDS_IN_400_YEAR_PERIOD);
    dts->year = naAddInt64(dts->year, naMulInt64(years400, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD));
    remainingsecs = naSubInt64(remainingsecs, naMulInt64(years400, NA_SECONDS_IN_400_YEAR_PERIOD));

    exception100 = NA_FALSE;
    // The first 100-year period of a 400-year period has a leap day
    if(naGreaterEqualInt64(remainingsecs, naAddInt64(NA_SECONDS_IN_100_YEAR_PERIOD, naMakeInt64WithLo(NA_SECONDS_PER_DAY)))){
      exception100 = NA_TRUE;
      dts->year = naAddInt64(dts->year, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD);
      remainingsecs = naSubInt64(remainingsecs, naAddInt64(NA_SECONDS_IN_100_YEAR_PERIOD, naMakeInt64WithLo(NA_SECONDS_PER_DAY)));

      years100 = naDivInt64(remainingsecs, NA_SECONDS_IN_100_YEAR_PERIOD);
      if(!naEqualInt64(years100, NA_ZERO_64)){
        dts->year = naAddInt64(dts->year, naMulInt64(years100, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD));
        remainingsecs = naSubInt64(remainingsecs, naMulInt64(years100, NA_SECONDS_IN_100_YEAR_PERIOD));
      }
    }

    if(exception100){
      if(naGreaterEqualInt64(remainingsecs, naMulInt64(NA_DATETIME_GREGORIAN_4_YEAR_PERIOD, NA_SECONDS_IN_NORMAL_YEAR))){
        dts->year = naAddInt64(dts->year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD);
        remainingsecs = naSubInt64(remainingsecs, naMulInt64(NA_DATETIME_GREGORIAN_4_YEAR_PERIOD, NA_SECONDS_IN_NORMAL_YEAR));
      }else if(naGreaterEqualInt64(remainingsecs, NA_SECONDS_IN_NORMAL_YEAR)){
        naIncInt64(dts->year);
        remainingsecs = naSubInt64(remainingsecs, NA_SECONDS_IN_NORMAL_YEAR);

        remainingyears = naDivInt64(remainingsecs, NA_SECONDS_IN_NORMAL_YEAR);
        dts->year = naAddInt64(dts->year, remainingyears);
        remainingsecs = naSubInt64(remainingsecs, naMulInt64(remainingyears, NA_SECONDS_IN_NORMAL_YEAR));
      }
    }

    years4 = naDivInt64(remainingsecs, NA_SECONDS_IN_4_YEAR_PERIOD);
    dts->year = naAddInt64(dts->year, naMulInt64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
    remainingsecs = naSubInt64(remainingsecs, naMulInt64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));

    if(naGreaterEqualInt64(remainingsecs, NA_SECONDS_IN_LEAP_YEAR)){
      naIncInt64(dts->year);
      remainingsecs = naSubInt64(remainingsecs, NA_SECONDS_IN_LEAP_YEAR);

      remainingyears = naDivInt64(remainingsecs, NA_SECONDS_IN_NORMAL_YEAR);
      dts->year = naAddInt64(dts->year, remainingyears);
      remainingsecs = naSubInt64(remainingsecs, naMulInt64(remainingyears, NA_SECONDS_IN_NORMAL_YEAR));
    }

    isleapyear = naIsLeapYearGregorian(dts->year);
  }else{
    // gregorian system with leap second information
    r = naGetTAIPeriodIndexForSISecond(remainingsecs);
    dts->year = naMakeInt64WithLo(naTAIPeriods[r].gregyear);
    remainingsecs = naSubInt64(remainingsecs, naTAIPeriods[r].startsisec);

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
  dayofyear += naCastInt64ToInt32(naDivInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_DAY)));
  remainingsecs = naModInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_DAY));
  dts->hour += naCastInt64ToInt32(naDivInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_HOUR)));
  remainingsecs = naModInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_HOUR));
  dts->min += naCastInt64ToInt32(naDivInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_MINUTE)));
  remainingsecs = naModInt64(remainingsecs, naMakeInt64WithLo(NA_SECONDS_PER_MINUTE));
  dts->sec += naCastInt64ToInt32(remainingsecs);

  // get the correct date.
  l = 0;
  r = 11;
  while(l != r){  // binary search
    m = (l+r)/2;
    if(na_cumulativemonthstartdays[2 * (m+1) + (NAInt)isleapyear] <= dayofyear){l = m + 1;}else{r = m;}
  }
  // r now defines the index of the month
  dts->mon = (int32)r;
  dts->day = dayofyear - na_cumulativemonthstartdays[2 * r + (NAInt)isleapyear];

  if(dta){
    int64 firstterm;

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
    dta->isleapyear = (int32)isleapyear;
    dta->daysinmonth = na_cumulativemonthstartdays[2 * (r + 1) + dta->isleapyear] - na_cumulativemonthstartdays[2 * r + dta->isleapyear];
    dta->yearsign = naSmallerInt64(dts->year, NA_ZERO_64)?-1:+1;

    // Weekday computation
    d = dts->day + 1;
    y = dts->year;
    mon = dts->mon + 1;
    if(mon<3){mon+=12; naDecInt64(y);}
    K = naModInt64(naAddInt64(naModInt64(y, naMakeInt64WithLo(100)), naMakeInt64WithLo(100)), naMakeInt64WithLo(100));
    if(naSmallerInt64(y, NA_ZERO_64)){
      J = naDivInt64(naAddInt64(y, NA_ONE_64), naMakeInt64WithLo(100));
    }else{
      J = naDivInt64(y, naMakeInt64WithLo(100));
    }
    if(naSmallerInt64(y, NA_ZERO_64)){naDecInt64(J);}
    firstterm = naAddInt64(naAddInt64(naMakeInt64WithLo(d + ((mon + 1) * 26) / 10), K), naDivInt64(K, naMakeInt64WithLo(4)));
    if(naSmallerInt64(naAddInt64(datetime->sisec, naMakeInt64WithLo(dts->shift * NA_SECONDS_PER_MINUTE)), NA_DATETIME_SISEC_START_GREGORIAN_PERIOD)){
      // Julian weedkday computation
      dta->weekday = naCastInt64ToInt32(naModInt64((naAddInt64((naModInt64((naAddInt64(naAddInt64(firstterm, naMakeInt64WithLo(5)), naMulInt64(naMakeInt64WithLo(6), J))), naMakeInt64WithLo(7))), naMakeInt64WithLo(12))), naMakeInt64WithLo(7)));
    }else{
      // Gregorian weedkday computation
      dta->weekday = naCastInt64ToInt32(naModInt64((naAddInt64((naModInt64((naAddInt64(naAddInt64(firstterm, naDivInt64(J, naMakeInt64WithLo(4))), naMulInt64(naMakeInt64WithLo(5), J))), naMakeInt64WithLo(7))), naMakeInt64WithLo(12))), naMakeInt64WithLo(7))); // Zeller algorihm
    }
  }
}






NA_DEF NAString* naNewStringFromSecondDifference(double difference,
                                                 uint8 decimaldigits){
  int64 allseconds, powten, decimals;
  uint8 seconds, minutes, hours;
  NAString* decimalstring;
  NAString* timestring;
  NAString* daystring;
  NAString* signstring;
  NAString* string;

  NABool needsign = NA_FALSE;
  if(difference<0){needsign = NA_TRUE; difference = -difference;}
  powten = naExp10i64(naMakeInt64WithLo(decimaldigits));
  allseconds = naMakeInt64WithDouble(difference * naCastInt64ToDouble(powten));
  decimals = naModInt64(allseconds, powten);
  allseconds = naDivInt64(allseconds, powten);
  seconds = naCastInt64ToUInt8(naModInt64(allseconds, naMakeInt64WithLo(NA_SECONDS_PER_MINUTE)));
  allseconds = naDivInt64(allseconds, naMakeInt64WithLo(NA_SECONDS_PER_MINUTE));
  minutes = naCastInt64ToUInt8(naModInt64(allseconds, naMakeInt64WithLo(NA_MINUTES_PER_HOUR)));
  allseconds = naDivInt64(allseconds, naMakeInt64WithLo(NA_MINUTES_PER_HOUR));
  hours = naCastInt64ToUInt8(naModInt64(allseconds, naMakeInt64WithLo(NA_HOURS_PER_DAY)));
  allseconds = naDivInt64(allseconds, naMakeInt64WithLo(NA_HOURS_PER_DAY));

  if(decimaldigits){
    NAString* decimalformatstring;
    decimalformatstring = naNewStringWithFormat(".%%0%dlld", decimaldigits);
    decimalstring = naNewStringWithFormat(naGetStringUTF8Pointer(decimalformatstring), decimals);
    naDelete(decimalformatstring);
  }else{
    decimalstring = naNewString();
  }

  timestring = naNewStringWithFormat("%02d:%02d:%02d", hours, minutes, seconds);

  if(!naEqualInt64(allseconds, NA_ZERO_64)){
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





NA_DEF NAInt naGetLeapSecondCorrectionConstant(int64 olduncertainsecondnumber){
  NAInt taiperiod;
  if(naSmallerInt64(olduncertainsecondnumber, NA_ZERO_64)){return NA_DATETIME_INVALID_UNCERTAIN_SECOND_NUMBER;}
  // Note that the last entry of the structure storing all TAI periods always
  // is a non-leap-second-entry.
  if(naEqualInt64(olduncertainsecondnumber, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec)){return NA_DATETIME_NO_CORRECTION_NEEDED;}
  if(naGreaterInt64(olduncertainsecondnumber, naTAIPeriods[NA_NUMBER_OF_TAI_PERIODS - 1].startsisec)){return NA_DATETIME_NEW_LIBRARY_IS_OLDER_THAN_BEFORE;}
  taiperiod = naGetTAIPeriodIndexForSISecond(olduncertainsecondnumber);
  // Find the earliest second which needs correction.
  while(taiperiod < NA_NUMBER_OF_TAI_PERIODS){
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE){return taiperiod;}
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER){return taiperiod;}
    taiperiod++;
  }
  // No leap second has been introduced in the meantime.
  return NA_DATETIME_NO_CORRECTION_NEEDED;
}



NA_DEF void naSetGlobalTimeShiftToSystemSettings(){
  #if NA_OS == NA_OS_WINDOWS
    NATimeZone curtimezone;
    GetTimeZoneInformation(&curtimezone);
    na_globaltimeshift = naMakeShiftFromTimeZone(&curtimezone);
    na_globalsummertime = ((curtimezone.DaylightBias) ? NA_TRUE : NA_FALSE);
  #elif NA_OS == NA_OS_MAC_OS_X
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



NA_DEF void naCorrectDateTimeForLeapSeconds(NADateTime* datetime, NAInt leapsecondcorrectionconstant){
  NAInt taiperiod;
  datetime->errornum = NA_DATETIME_ERROR_NONE;
  if((leapsecondcorrectionconstant >= 0) && naGreaterEqualInt64(datetime->sisec, naTAIPeriods[leapsecondcorrectionconstant].startsisec)){
    // Correcting a date for leap seconds means that the siseconds stored are
    // greaterequal to an entry in the TAI periods structure which introduces
    // a leap second. When storing such a date with an earlier version of NALib,
    // no leap second introduction is assumed. Therefore, now as the leap second
    // is known, it must be added to the date.
    //
    // To do so, we subtract all previously known leap seconds...
    datetime->sisec = naSubInt64(datetime->sisec, naSubInt64(naTAIPeriods[leapsecondcorrectionconstant-1].startsisec, naTAIPeriods[leapsecondcorrectionconstant-1].startgregsec));
    // And add the correct number of leap seconds anew:
    taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(datetime->sisec);
    datetime->sisec = naAddInt64(datetime->sisec, naSubInt64(naTAIPeriods[taiperiod].startsisec, naTAIPeriods[taiperiod].startgregsec));
  }
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
