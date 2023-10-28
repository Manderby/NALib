
#include "../NADateTime.h"
#include "../NAString.h"
#include "../NABinaryData.h"
#include "../../NAMath/NAMathOperators.h"
#include "ctype.h"


#if NA_OS == NA_OS_WINDOWS
  #include <time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_s(storage, tme);}
#elif NA_IS_POSIX
  #include <sys/time.h>
//  NA_IDEF void Localtime(struct tm* storage, const time_t* tme){localtime_r(tme, storage);}
#else
  #error "System undefined"
#endif


// todo: global variables

// The global timeZone setting.
int16  na_GlobalTimeShift = 0;
NABool na_GlobalDaylightSavingTime = NA_FALSE;

const char* na_MonthEnglishNames[12] = {"January", "February", "March", "April", "Mai", "June", "July", "August", "September", "October", "November", "December"};
const char* na_MonthEnglishAbbreviationNames[12] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const char* na_DateTimeErrorStrings[NA_DATETIME_ERROR_COUNT] = {
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
int32 na_CumulativeMonthStartDays[2 * 13] = { 0,  0,
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
  NAi64 startgregsec;
  NAi64 startsiSecond;
  int16 gregyear;
  NALeapSecondCorrectionIndicator indicator;
} NATAIPeriod;

// Leap second information:
#define NA_TAI_PERIODS_COUNT 106

// This table stores all leap second entries since 1958. Every year has at
// least 1 entry. Every entry defines, what the number of its first second is.
// Additionally, it is defined, what the seconds in that period are. They can
// be regular seconds denoting a start date, or they can be lead seconds. This
// structure is future-proof for at least some centuries, as it will be able to
// map leap seconds even if they are introduced every second. The same goes for
// negative leap seconds. But there have not been introduced any so far.
NATAIPeriod naTAIPeriods[NA_TAI_PERIODS_COUNT] = {
  {naMakeu64WithLiteralLo(         0), naMakeu64WithLiteralLo(         0), 1958, NA_START_JANUARY_FIRST},           // [0]
  {naMakeu64WithLiteralLo(  31536000), naMakeu64WithLiteralLo(  31536000), 1959, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(  63072000), naMakeu64WithLiteralLo(  63072000), 1960, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(  94694400), naMakeu64WithLiteralLo(  94694400), 1961, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 126230400), naMakeu64WithLiteralLo( 126230400), 1962, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 157766400), naMakeu64WithLiteralLo( 157766400), 1963, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 189302400), naMakeu64WithLiteralLo( 189302400), 1964, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 220924800), naMakeu64WithLiteralLo( 220924800), 1965, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 252460800), naMakeu64WithLiteralLo( 252460800), 1966, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 283996800), naMakeu64WithLiteralLo( 283996800), 1967, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 315532800), naMakeu64WithLiteralLo( 315532800), 1968, NA_START_JANUARY_FIRST},           // [10]
  {naMakeu64WithLiteralLo( 347155200), naMakeu64WithLiteralLo( 347155200), 1969, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 378691200), naMakeu64WithLiteralLo( 378691200), 1970, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 410227200), naMakeu64WithLiteralLo( 410227200), 1971, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 441763200), naMakeu64WithLiteralLo( 441763200), 1971, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 10
  {naMakeu64WithLiteralLo( 441763200), naMakeu64WithLiteralLo( 441763210), 1972, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 457488000), naMakeu64WithLiteralLo( 457488010), 1972, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo( 457488000), naMakeu64WithLiteralLo( 457488011), 1972, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo( 473385600), naMakeu64WithLiteralLo( 473385611), 1972, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 473385600), naMakeu64WithLiteralLo( 473385612), 1973, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 504921600), naMakeu64WithLiteralLo( 504921612), 1973, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [20] + 1
  {naMakeu64WithLiteralLo( 504921600), naMakeu64WithLiteralLo( 504921613), 1974, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 536457600), naMakeu64WithLiteralLo( 536457613), 1974, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 536457600), naMakeu64WithLiteralLo( 536457614), 1975, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 567993600), naMakeu64WithLiteralLo( 567993614), 1975, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 567993600), naMakeu64WithLiteralLo( 567993615), 1976, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 599616000), naMakeu64WithLiteralLo( 599616015), 1976, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 599616000), naMakeu64WithLiteralLo( 599616016), 1977, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 631152000), naMakeu64WithLiteralLo( 631152016), 1977, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 631152000), naMakeu64WithLiteralLo( 631152017), 1978, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 662688000), naMakeu64WithLiteralLo( 662688017), 1978, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [30] + 1
  {naMakeu64WithLiteralLo( 662688000), naMakeu64WithLiteralLo( 662688018), 1979, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 694224000), naMakeu64WithLiteralLo( 694224018), 1979, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 694224000), naMakeu64WithLiteralLo( 694224019), 1980, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 725846400), naMakeu64WithLiteralLo( 725846419), 1981, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 741484800), naMakeu64WithLiteralLo( 741484819), 1981, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo( 741484800), naMakeu64WithLiteralLo( 741484820), 1981, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo( 757382400), naMakeu64WithLiteralLo( 757382420), 1982, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 773020800), naMakeu64WithLiteralLo( 773020820), 1982, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo( 773020800), naMakeu64WithLiteralLo( 773020821), 1982, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo( 788918400), naMakeu64WithLiteralLo( 788918421), 1983, NA_START_JANUARY_FIRST},           // [40]
  {naMakeu64WithLiteralLo( 804556800), naMakeu64WithLiteralLo( 804556821), 1983, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo( 804556800), naMakeu64WithLiteralLo( 804556822), 1983, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo( 820454400), naMakeu64WithLiteralLo( 820454422), 1984, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 852076800), naMakeu64WithLiteralLo( 852076822), 1985, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 867715200), naMakeu64WithLiteralLo( 867715222), 1985, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo( 867715200), naMakeu64WithLiteralLo( 867715223), 1985, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo( 883612800), naMakeu64WithLiteralLo( 883612823), 1986, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 915148800), naMakeu64WithLiteralLo( 915148823), 1987, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo( 946684800), naMakeu64WithLiteralLo( 946684823), 1987, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo( 946684800), naMakeu64WithLiteralLo( 946684824), 1988, NA_START_JANUARY_FIRST},           // [50]
  {naMakeu64WithLiteralLo( 978307200), naMakeu64WithLiteralLo( 978307224), 1989, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1009843200), naMakeu64WithLiteralLo(1009843224), 1989, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1009843200), naMakeu64WithLiteralLo(1009843225), 1990, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1041379200), naMakeu64WithLiteralLo(1041379225), 1990, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1041379200), naMakeu64WithLiteralLo(1041379226), 1991, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1072915200), naMakeu64WithLiteralLo(1072915226), 1992, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1088640000), naMakeu64WithLiteralLo(1088640026), 1992, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo(1088640000), naMakeu64WithLiteralLo(1088640027), 1992, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo(1104537600), naMakeu64WithLiteralLo(1104537627), 1993, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1120176000), naMakeu64WithLiteralLo(1120176027), 1993, NA_POSITIVE_LEAP_SECONDS_JUNE},    // [60] + 1
  {naMakeu64WithLiteralLo(1120176000), naMakeu64WithLiteralLo(1120176028), 1993, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo(1136073600), naMakeu64WithLiteralLo(1136073628), 1994, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1151712000), naMakeu64WithLiteralLo(1151712028), 1994, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo(1151712000), naMakeu64WithLiteralLo(1151712029), 1994, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo(1167609600), naMakeu64WithLiteralLo(1167609629), 1995, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1199145600), naMakeu64WithLiteralLo(1199145629), 1995, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1199145600), naMakeu64WithLiteralLo(1199145630), 1996, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1230768000), naMakeu64WithLiteralLo(1230768030), 1997, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1246406400), naMakeu64WithLiteralLo(1246406430), 1997, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo(1246406400), naMakeu64WithLiteralLo(1246406431), 1997, NA_START_JULY_FIRST},              // [70]
  {naMakeu64WithLiteralLo(1262304000), naMakeu64WithLiteralLo(1262304031), 1998, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1293840000), naMakeu64WithLiteralLo(1293840031), 1998, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1293840000), naMakeu64WithLiteralLo(1293840032), 1999, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1325376000), naMakeu64WithLiteralLo(1325376032), 2000, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1356998400), naMakeu64WithLiteralLo(1356998432), 2001, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1388534400), naMakeu64WithLiteralLo(1388534432), 2002, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1420070400), naMakeu64WithLiteralLo(1420070432), 2003, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1451606400), naMakeu64WithLiteralLo(1451606432), 2004, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1483228800), naMakeu64WithLiteralLo(1483228832), 2005, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1514764800), naMakeu64WithLiteralLo(1514764832), 2005, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// [80] + 1
  {naMakeu64WithLiteralLo(1514764800), naMakeu64WithLiteralLo(1514764833), 2006, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1546300800), naMakeu64WithLiteralLo(1546300833), 2007, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1577836800), naMakeu64WithLiteralLo(1577836833), 2008, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1609459200), naMakeu64WithLiteralLo(1609459233), 2008, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1609459200), naMakeu64WithLiteralLo(1609459234), 2009, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1640995200), naMakeu64WithLiteralLo(1640995234), 2010, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1672531200), naMakeu64WithLiteralLo(1672531234), 2011, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1704067200), naMakeu64WithLiteralLo(1704067234), 2012, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1719792000), naMakeu64WithLiteralLo(1719792034), 2012, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo(1719792000), naMakeu64WithLiteralLo(1719792035), 2012, NA_START_JULY_FIRST},              // [90]
  {naMakeu64WithLiteralLo(1735689600), naMakeu64WithLiteralLo(1735689635), 2013, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1767225600), naMakeu64WithLiteralLo(1767225635), 2014, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1798761600), naMakeu64WithLiteralLo(1798761635), 2015, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1814400000), naMakeu64WithLiteralLo(1814400035), 2015, NA_POSITIVE_LEAP_SECONDS_JUNE},    // + 1
  {naMakeu64WithLiteralLo(1814400000), naMakeu64WithLiteralLo(1814400036), 2015, NA_START_JULY_FIRST},
  {naMakeu64WithLiteralLo(1830297600), naMakeu64WithLiteralLo(1830297636), 2016, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1861920000), naMakeu64WithLiteralLo(1861920036), 2016, NA_POSITIVE_LEAP_SECONDS_DECEMBER},// + 1
  {naMakeu64WithLiteralLo(1861920000), naMakeu64WithLiteralLo(1861920037), 2017, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1893456000), naMakeu64WithLiteralLo(1893456037), 2018, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1924992000), naMakeu64WithLiteralLo(1924992037), 2019, NA_START_JANUARY_FIRST},           // [100]
  {naMakeu64WithLiteralLo(1956528000), naMakeu64WithLiteralLo(1956528037), 2020, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(1988150400), naMakeu64WithLiteralLo(1988150437), 2021, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(2019686400), naMakeu64WithLiteralLo(2019686437), 2022, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(2051222400), naMakeu64WithLiteralLo(2051222437), 2023, NA_START_JANUARY_FIRST},
  {naMakeu64WithLiteralLo(2066860800), naMakeu64WithLiteralLo(2066860838), 2023, NA_START_JULY_FIRST},
  // the last entry is the first date with unknown future leap seconds.
  // everything up and including that date is known.
};


// NA_DATETIME_SISEC_START_GREGORIAN_PERIOD denotes the siSecond relative to the
// start of the TAI period (siSecond == 0) denoting the start of the gregorian
// time period.
// TAI period starts at January 1st 1958
// Gregorian time period starts on Friday, October 15th 1582
// Julian time period ends with Thursday, October 4th 1582 (inclusive).
//#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD            (- NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 4LL * NA_SECONDS_IN_4_YEAR_PERIOD - NA_SECONDS_IN_NORMAL_YEAR - (31LL+30LL+31LL-14LL) * NA_SECONDS_PER_DAY)
//#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD    -11840601600LL
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_HI 0xfffffffd
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_LO 0x3e3ec200
#define NA_DATETIME_SISEC_START_GREGORIAN_PERIOD    naCastu64Toi64(naMakeu64(NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_HI, NA_DATETIME_SISEC_START_GREGORIAN_PERIOD_LO))

// NA_DATETIME_SISEC_JULIAN_YEAR_ZERO denotes the siSecond relative to the start of the
// TAI period (siSecond == 0) denoting the beginning of the julian year 0 when
// using astronomical year numbering.
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering equals year 1 BC.
// This assumes the planned leap years by cesar.
//#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO                 (NA_DATETIME_SISEC_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+4LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 395LL * NA_SECONDS_IN_4_YEAR_PERIOD)
//#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO                 -61788700800
#define NA_DATETIME_SISEC_YEAR_ZERO_HI              0xfffffff1
#define NA_DATETIME_SISEC_YEAR_ZERO_LO              0x9d1b3f80
#define NA_DATETIME_SISEC_JULIAN_YEAR_ZERO          naCastu64Toi64(naMakeu64(NA_DATETIME_SISEC_YEAR_ZERO_HI, NA_DATETIME_SISEC_YEAR_ZERO_LO))

// NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO denotes the siSecond relative to the start of the
// TAI period (siSecond == 0) denoting the beginning of the gregorian year 0 when
// using astronomical year numbering.
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering equals year 1 BC.
//#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     (NA_DATETIME_SISEC_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+14LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 19LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 3LL * NA_SECONDS_IN_400_YEAR_PERIOD)
//#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     -61788528000
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_HI  0xfffffff1
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_LO  0x9d1de280
#define NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO     naCastu64Toi64(naMakeu64(NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_HI, NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO_LO))

// the UNIX system time has no leap seconds and is nulled in 1970
#define NA_DATETIME_SISEC_UNIX_YEAR_ZERO          naMuli64(NA_SECONDS_IN_4_YEAR_PERIOD, naMakei64WithLo(3))

// the Windows fileTime has no leap seconds and is nulled in 1601
//#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      (-NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 2LL * NA_SECONDS_IN_100_YEAR_PERIOD - 24LL * NA_SECONDS_IN_4_YEAR_PERIOD - 3LL * NA_SECONDS_IN_NORMAL_YEAR)
//#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      -11265782400
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_HI   0xfffffffd
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_LO   0x6081cd80
#define NA_DATETIME_SISEC_FILETIME_YEAR_ZERO      naCastu64Toi64(naMakeu64(NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_HI, NA_DATETIME_SISEC_FILETIME_YEAR_ZERO_LO))


// Prototypes:
NAInt naGetTAIPeriodIndexForSISecond(NAi64 siSecondond);
NAInt naGetLatestTAIPeriodIndexForGregorianSecond(NAi64 gregsecond);
NADateTime naMakeDateTimeNow(void);
void naSetGlobalTimeShiftToSystemSettings(void);
NAi64 naGetFirstUncertainSecondNumber(void);




NA_DEF NAInt naGetTAIPeriodIndexForSISecond(NAi64 siSecondond){
  NAInt r;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 3].startsiSecond, siSecondond)){
    if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 2].startsiSecond, siSecondond)){
      if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startsiSecond, siSecondond)){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_TAI_PERIODS_COUNT - 1;
      }
      return NA_TAI_PERIODS_COUNT - 2;
    }
    return NA_TAI_PERIODS_COUNT - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(naSmalleri64(siSecondond, NA_ZERO_i64)){
    r = -1;
  }else{
    // In all other cases, perform a binary search in all TAI periods.
    NAInt l = 0;
    r = NA_TAI_PERIODS_COUNT - 4;
    while(l != r){  // binary search
      NAInt m = (l+r)/2;
      if(naSmallerEquali64(naTAIPeriods[m + 1].startsiSecond, siSecondond)){l = m + 1;}else{r = m;}
    }
    // l or r now define the index of the latest NATAIPeriod.
  }
  return r;
}



NA_DEF NAInt naGetLatestTAIPeriodIndexForGregorianSecond(NAi64 gregsecond){
  NAInt r;
  // First, check the last 3 TAI periods. There is a high probability that a
  // given date is within the last 3 entries. Three entries because the entry
  // of one leap second always contains the leap second itself plus the two
  // surrounding "normal" periods.
  if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 3].startgregsec, gregsecond)){
    if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 2].startgregsec, gregsecond)){
      if(naSmallerEquali64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startgregsec, gregsecond)){
        // Note that the last TAI period is explicitely returned, even if the
        // desired date is far in the future.
        return NA_TAI_PERIODS_COUNT - 1;
      }
      return NA_TAI_PERIODS_COUNT - 2;
    }
    return NA_TAI_PERIODS_COUNT - 3;
  }
  // Just in case the given date is not in leap second age at all...
  if(naSmalleri64(gregsecond, NA_ZERO_i64)){
    r = -1;
  }else{
    // In all other cases, perform a binary search in all TAI periods.
    NAInt l = 0;
    r = NA_TAI_PERIODS_COUNT - 4;
    while(l != r){  // binary search
      NAInt m = (l+r)/2;
      if(naSmallerEquali64(naTAIPeriods[m + 1].startgregsec, gregsecond)){l = m + 1;}else{r = m;}
    }
    // l or r now define the index of the latest NATAIPeriod.
  }
  return r;
}



NA_DEF int32 naGetMonthNumberWithEnglishAbbreviation(const NAString* str){
  int32 i;
  int32 monthindex = -1;
  for(i = 0; i < NA_MONTHS_PER_YEAR; ++i){
    if(naEqualStringToUTF8CString(str, na_MonthEnglishAbbreviationNames[i], NA_TRUE)){
      monthindex = i;
      break;
    }
  }
  #if NA_DEBUG
    if(monthindex == -1)
      naError("Month abbreviation unknown. Returning -1.");
  #endif
  return monthindex;
}


NA_DEF int32 naGetMonthNumberFromUTF8CStringLiteral(const NAUTF8Char* str){
  int32 monthindex = -1;
  if(naStrlen(str)){
    int32 i;
    for(i = 0; i<NA_MONTHS_PER_YEAR; ++i){
      if(naEqualUTF8CStringLiterals(str, na_MonthEnglishNames[i], 0, NA_FALSE)){
        monthindex = i;
        break;
      }
    }
    if(monthindex == -1){
      for(i = 0; i<NA_MONTHS_PER_YEAR; ++i){
        if(naEqualUTF8CStringLiterals(str, na_MonthEnglishAbbreviationNames[i], 0, NA_FALSE)){
          monthindex = i;
          break;
        }
      }
    }
    if(monthindex == -1){
      if(isdigit((const char)str[0])){
        int32 returnint;
        NAString* numberstring = naNewStringWithFormat(str);
        returnint = naParseStringi32(numberstring) - 1;
        naDelete(numberstring);
        if(returnint >= 0 && returnint < 12){monthindex = returnint;}
      }
    }
  }
  #if NA_DEBUG
    if(monthindex == -1)
      naError("Month unknown. Returning -1.");
  #endif
  return monthindex;
}







NA_DEF NADateTime naMakeDateTimeNow(){
  #if NA_OS == NA_OS_WINDOWS
    FILETIME fileTime;
    NATimeZone timeZone;
    GetSystemTimeAsFileTime(&fileTime);
    // Daylight saving is active if the function returns 2.
    NAInt daylightCode = GetTimeZoneInformation(&timeZone);
    return naMakeDateTimeFromFileTime(&fileTime, &timeZone, daylightCode == 2);
  #elif NA_IS_POSIX
    struct timeval curtime;
    NATimeZone curTimeZone;
    gettimeofday(&curtime, &curTimeZone);
    return naMakeDateTimeFromTimeVal(&curtime, &curTimeZone);
  #endif
}





NA_DEF NADateTime naMakeDateTimeWithDateTimeStruct(const NADateTimeStruct* dts){
  NADateTime dateTime;

  NAi64 remainingYears = dts->year;
  NAi64 years4;
  NABool isleap;
  NACalendarSystem calendarsystem;

  dateTime.errorNum = NA_DATETIME_ERROR_NONE;

  if((naEquali64(dts->year, NA_DATETIME_YEAR_1582)) && (dts->mon == 9) && (dts->day > 3) && (dts->day < 14)){
    dateTime.errorNum = NA_DATETIME_ERROR_JULIAN_GREGORIAN_CHASM;
  }
  if((naSmalleri64(dts->year, NA_DATETIME_YEAR_1582)) || ((naEquali64(dts->year, NA_DATETIME_YEAR_1582)) && ((dts->mon < 9) || ((dts->mon == 9) && (dts->day < 14))))){
    // julian system
    dateTime.siSecond = NA_DATETIME_SISEC_JULIAN_YEAR_ZERO;
    isleap = naIsLeapYearJulian(dts->year);
    calendarsystem = NA_CALENDAR_JULIAN;
  }else{
    // gregorian system
    NAi64 years400;
    NAi64 years100;
    dateTime.siSecond = NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO;
    years400 = naDivi64(remainingYears, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD);
    dateTime.siSecond = naAddi64(dateTime.siSecond, naMuli64(years400, NA_SECONDS_IN_400_YEAR_PERIOD));
    remainingYears = naSubi64(remainingYears, naMuli64(years400, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD));
    years100 = naDivi64(remainingYears, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD);
    dateTime.siSecond = naAddi64(dateTime.siSecond, naMuli64(years100, NA_SECONDS_IN_100_YEAR_PERIOD));
    remainingYears = naSubi64(remainingYears, naMuli64(years100, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD));
    isleap = naIsLeapYearGregorian(dts->year);
    if(naSmalleri64(dts->year, NA_DATETIME_YEAR_1958)){
      calendarsystem = NA_CALENDAR_GREGORIAN;
    }else{
      calendarsystem = NA_CALENDAR_GREGORIAN_WITH_LEAP_SECONDS;
    }
  }
  years4 = naDivi64(remainingYears, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD);
  if(naSmalleri64(remainingYears, NA_ZERO_i64)){naDeci64(years4);}
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMuli64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));
  remainingYears = naSubi64(remainingYears, naMuli64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
  if(!naEquali64(remainingYears, NA_ZERO_i64)){
    dateTime.siSecond = naAddi64(dateTime.siSecond, NA_SECONDS_IN_LEAP_YEAR); naDeci64(remainingYears);
    dateTime.siSecond = naAddi64(dateTime.siSecond, naMuli64(remainingYears, NA_SECONDS_IN_NORMAL_YEAR));
  }
  if((dts->mon < 0) || (dts->mon > 11)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_MONTH_NUMBER;}
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(na_CumulativeMonthStartDays[2 * dts->mon + (int32)isleap] * (int32)NA_SECONDS_PER_DAY));
  if((dts->day < 0) || ((na_CumulativeMonthStartDays[2 * dts->mon + (int32)isleap] + dts->day) >= na_CumulativeMonthStartDays[2 * (dts->mon + 1) + (int32)isleap])){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_DAY_NUMBER;}
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(dts->day * (int32)NA_SECONDS_PER_DAY));
  if((dts->hour < 0) || (dts->hour > 23)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_HOUR_NUMBER;}
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(dts->hour * (int32)NA_SECONDS_PER_HOUR));
  if((dts->min < 0) || (dts->min > 59)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_MINUTE_NUMBER;}
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(dts->min * (int32)NA_SECONDS_PER_MINUTE));
  if(calendarsystem == NA_CALENDAR_GREGORIAN_WITH_LEAP_SECONDS){
    if(naGreaterEquali64(dateTime.siSecond, naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startsiSecond)){
      if((dts->sec < 0) || (dts->sec > 59)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
      dateTime.siSecond = naAddi64(dateTime.siSecond, naSubi64(naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startsiSecond, naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startgregsec));
    }else{
      NAInt r = naGetLatestTAIPeriodIndexForGregorianSecond(dateTime.siSecond);
      // r now defines the index of the NATAIPeriod
      dateTime.siSecond = naAddi64(dateTime.siSecond, naSubi64(naTAIPeriods[r].startsiSecond, naTAIPeriods[r].startgregsec));
      dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(dts->sec));
      if((r+1 < NA_TAI_PERIODS_COUNT) && naGreaterEquali64(dateTime.siSecond, naTAIPeriods[r+1].startsiSecond)){
        if((naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE) || (naTAIPeriods[r+1].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER)){
          NAInt rPlus2 = r+2; // We need to add this because of static code analysis.
          if(rPlus2 < NA_TAI_PERIODS_COUNT && naGreaterEquali64(dateTime.siSecond, naTAIPeriods[rPlus2].startsiSecond)){
            // The leap seconds are overflown
            dateTime.errorNum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;
          }
        }else{
          // The period overflown has no leap seconds
          dateTime.errorNum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;
        }
      }else{
        if((dts->sec < 0) || (dts->sec > 59)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
      }
      dateTime.siSecond = naSubi64(dateTime.siSecond, naMakei64WithLo(dts->sec));
    }
  }else{
    if((dts->sec < 0) || (dts->sec > 59)){dateTime.errorNum = NA_DATETIME_ERROR_INVALID_SECOND_NUMBER;}
  }
  dateTime.siSecond = naAddi64(dateTime.siSecond, naMakei64WithLo(dts->sec));
  dateTime.siSecond = naSubi64(dateTime.siSecond, naMakei64WithLo(dts->shift * (int32)NA_SECONDS_PER_MINUTE));
  dateTime.nanoSecond  = dts->nanoSecond;
  dateTime.shift = dts->shift;
  dateTime.flags = dts->flags;

  return dateTime;
}






NA_DEF NADateTime naMakeDateTimeFromString(const NAString* string, NAAscDateTimeFormat format){
  NAString* str;
  NADateTimeStruct dts;
  NAString* token;
  int16 int16value;
  NABufferIterator iter;

  dts.nanoSecond = 0;

  str = naNewStringExtraction(string, 0, -1);
  iter = naMakeBufferAccessor(naGetStringBufferMutable(str));

  switch(format){
  case NA_DATETIME_FORMAT_APACHE:
    dts.day = naParseBufferi32(&iter, NA_TRUE);
    token = naParseBufferTokenWithDelimiter(&iter, '/', NA_FALSE);
    dts.mon = naGetMonthNumberWithEnglishAbbreviation(token);
    naDelete(token);
    dts.year = naParseBufferi64(&iter, NA_TRUE);

    dts.hour = naParseBufferi32(&iter, NA_TRUE);
    dts.min = naParseBufferi32(&iter, NA_TRUE);
    dts.sec = naParseBufferi32(&iter, NA_TRUE);

    // The remaining string contains the time shift
    int16value = naParseBufferi16(&iter, NA_FALSE);
    dts.shift = (int16value / 100) * NA_MINUTES_PER_HOUR;
    dts.shift += int16value % 100;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT:
    dts.year = naParseBufferi64(&iter, NA_TRUE);
    dts.mon = naParseBufferi32(&iter, NA_TRUE) - 1;
    dts.day = naParseBufferi32(&iter, NA_TRUE) - 1;

    dts.hour = naParseBufferi32(&iter, NA_TRUE);
    dts.min = naParseBufferi32(&iter, NA_TRUE);
    dts.sec = naParseBufferi32(&iter, NA_FALSE);

    dts.shift = naParseBufferi16(&iter, NA_TRUE) * NA_MINUTES_PER_HOUR;
    if(dts.shift < 0){
      dts.shift -= naParseBufferi16(&iter, NA_FALSE);
    }else{
      dts.shift += naParseBufferi16(&iter, NA_FALSE);
    }
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_CONDENSEDDATE:
    token = naNewStringExtraction(str, 0, -5);
    dts.year = naParseStringi64(token);
    naDelete(token);
    token = naNewStringExtraction(str, -4, -3);
    dts.mon = naParseStringi32(token);
    naDelete(token);
    token = naNewStringExtraction(str, -2, -1);
    dts.day = naParseStringi32(token);
    naDelete(token);
    dts.hour = 0;
    dts.min = 0;
    dts.sec = 0;
    dts.shift = 0;
    dts.flags = 0;
    break;
  case NA_DATETIME_FORMAT_NATURAL:
    dts.year = naParseBufferi64(&iter, NA_TRUE);
    dts.mon = naParseBufferi32(&iter, NA_TRUE);
    dts.day = naParseBufferi32(&iter, NA_TRUE);

    dts.hour = naParseBufferi32(&iter, NA_TRUE);
    dts.min = naParseBufferi32(&iter, NA_TRUE);
    dts.sec = naParseBufferi32(&iter, NA_FALSE);

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

  dts.nanoSecond = 0;

  switch(format){
  case NA_DATETIME_FORMAT_ICC_PROFILE:
    // ICC section 5.1.1, page 4, dateTimeNumber
    dts.year  = naMakei64WithLo(naReadBufferu16(&iter));
    dts.mon   = naReadBufferu16(&iter);
    dts.day   = naReadBufferu16(&iter);
    dts.hour  = naReadBufferu16(&iter);
    dts.min   = naReadBufferu16(&iter);
    dts.sec   = naReadBufferu16(&iter);
    dts.shift = 0;
    dts.flags = 0;
    break;

  case NA_DATETIME_FORMAT_PNG:
    dts.year  = naMakei64WithLo(naReadBufferu16(&iter));
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



NA_DEF const char* naGetDateTimeErrorString(uint8 errorNum){
  if(errorNum >= NA_DATETIME_ERROR_COUNT){
    #if NA_DEBUG
      naError("Error number invalid");
    #endif
    return NA_NULL;
  }
  return na_DateTimeErrorStrings[errorNum];
}



//NA_DEF NAByteArray* naInitByteArrayFromDateTime( NAByteArray* bytearray, const NADateTime* dateTime, NABinDateTimeFormat format){
//  uint16 valueu16;
//  NAByte* ptr;
//  NADateTimeStruct dts;
//  NADateTimeAttribute dta;
//  naExtractDateTimeInformation(dateTime, &dts, &dta);
//
//  switch(format){
//  case NA_DATETIME_FORMAT_ICC_PROFILE:
//    // ICC section 5.1.1, page 4, dateTimeNumber
//    bytearray = naInitByteArrayWithByteSize(bytearray, 12);
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



NA_DEF NAString* naNewStringWithDateTime(
  const NADateTime* dateTime,
  NAAscDateTimeFormat format)
{
  NAString* string;
  NADateTimeStruct dts;
  NADateTimeAttribute dta;
  naExtractDateTimeInformation(dateTime, &dts, &dta);

  switch(format){
  case NA_DATETIME_FORMAT_APACHE:
    string = naNewStringWithFormat("%02d/%s/%lld:%02d:%02d:%02d %c%02d%02d",
                dts.day + 1,
                na_MonthEnglishAbbreviationNames[dts.mon],
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










NA_DEF struct tm naMakeTMfromDateTime(const NADateTime* dateTime){
  struct tm systemtimestruct;
  NADateTimeStruct dts;
  NADateTimeAttribute dta;
  naExtractDateTimeInformation(dateTime, &dts, &dta);
  systemtimestruct.tm_sec = dts.sec;
  systemtimestruct.tm_min = dts.min;
  systemtimestruct.tm_hour = dts.hour;
  systemtimestruct.tm_mday = dts.day + 1;
  systemtimestruct.tm_mon = dts.mon;
  systemtimestruct.tm_year = naCasti64Toi32(dts.year) - 1900;
  systemtimestruct.tm_wday = (dta.weekday + 1) % 7;
  systemtimestruct.tm_yday = dta.dayOfYear;
  systemtimestruct.tm_isdst = naHasDateTimeDaylightSavingTime(dateTime)?1:0;
  #if NA_OS == NA_OS_MAC_OS_X
    systemtimestruct.tm_gmtoff = (long)(dts.shift * NA_SECONDS_PER_MINUTE);
    if(naHasDateTimeDaylightSavingTime(dateTime)){
      systemtimestruct.tm_zone = tzname[1];
    }else{
      systemtimestruct.tm_zone = tzname[0];
    }
  #endif
  return systemtimestruct;
}



NA_DEF int16 naMakeShiftFromTimeZone(const NATimeZone* timeZone, NABool daylightSaving){
  int16 shift;
  #if NA_OS == NA_OS_WINDOWS
    shift = -(int16)timeZone->Bias;
    if(daylightSaving){
      shift += NA_MINUTES_PER_HOUR;
    }
  #elif NA_OS == NA_OS_MAC_OS_X
    shift = (int16)-timeZone->tz_minuteswest; // yes, its inverted.
    if(daylightSaving){
      // In contrast to tm_gmtoff, the timeZone struct does not automatically
      // contain the daylightSavingTime shift and must be added manually.
      shift += NA_MINUTES_PER_HOUR;
    }
  #endif
  return shift;
}







#if NA_OS == NA_OS_WINDOWS

  NA_DEF NADateTime naMakeDateTimeFromFileTime(const FILETIME* fileTime, const NATimeZone* timeZone, NABool daylightSaving){
    NADateTime dateTime;
    NAi64 nanoSeconds = naCastu64Toi64(naMakeu64(fileTime->dwHighDateTime, fileTime->dwLowDateTime));

    dateTime.errorNum = NA_DATETIME_ERROR_NONE;
    dateTime.nanoSecond = naCasti64Toi32(naMuli64(naModi64(nanoSeconds, naMakei64WithLo(10000000)), naMakei64WithLo(100)));  // 100-nanosecond intervals.
    dateTime.siSecond = naAddi64(naDivi64(nanoSeconds, naMakei64WithLo(10000000)), NA_DATETIME_SISEC_FILETIME_YEAR_ZERO);
    if(naGreaterEquali64(dateTime.siSecond, NA_ZERO_i64)){
      NAInt taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(dateTime.siSecond);
      dateTime.siSecond = naAddi64(dateTime.siSecond, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec));
    }

    if(timeZone){
      dateTime.shift = naMakeShiftFromTimeZone(timeZone, daylightSaving);
      dateTime.flags = (daylightSaving ? NA_DATETIME_FLAG_DAYLIGHT_SAVING_TIME : 0);
    }else{
      dateTime.shift = na_GlobalTimeShift;
      dateTime.flags = ((na_GlobalDaylightSavingTime) ? NA_DATETIME_FLAG_DAYLIGHT_SAVING_TIME : 0);
    }
    return dateTime;
  }

#elif NA_IS_POSIX

  #if NA_OS == NA_OS_MAC_OS_X
  #define time_t __darwin_time_t
  #endif

  NA_DEF struct timespec naMakeTimeSpecFromDateTime(const NADateTime* dateTime, NABool daylightSaving){
    NA_UNUSED(daylightSaving);
    struct timespec timeSpec;
    NAInt taiperiod = naGetTAIPeriodIndexForSISecond(dateTime->siSecond);
    #if NA_TYPE_NATIVE_LONG_BITS == 32
      timeSpec.tv_sec = (__darwin_time_t)naCasti64Toi32(naSubi64(dateTime->siSecond, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec)));
      timeSpec.tv_sec -= (__darwin_time_t)naCasti64Toi32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
    #else
      #if !defined NA_TYPE_INT64
        // We fall back to 32 bits as there simply is no solution to this problem.
        #error "impossible to convert 64 bit integer. Falling back to 32 bits"
        timeSpec.tv_sec = (time_t)naCasti64Toi32(naSubi64(dateTime->siSecond, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec)));
        timeSpec.tv_sec -= (time_t)naCasti64Toi32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
      #else
        timeSpec.tv_sec = (time_t)(naSubi64(dateTime->siSecond, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec)));
        timeSpec.tv_sec -= (time_t)NA_DATETIME_SISEC_UNIX_YEAR_ZERO;
      #endif
    #endif
    timeSpec.tv_nsec = dateTime->nanoSecond;
    return timeSpec;
  }


  NA_DEF struct timeval naMakeTimeValFromDateTime(const NADateTime* dateTime){
    struct timeval timeVal;
    struct timespec timeSpec = naMakeTimeSpecFromDateTime(dateTime, NA_FALSE /*unused on mac*/);
    timeVal.tv_sec = timeSpec.tv_sec;
    timeVal.tv_usec = (int)(timeSpec.tv_nsec / 1000);
    return timeVal;
  }


  NA_DEF NATimeZone naMakeTimeZoneFromDateTime(const NADateTime* dateTime){
    NATimeZone timeZone;
    timeZone.tz_minuteswest = -dateTime->shift; // yes, its inverted.
    if(naHasDateTimeDaylightSavingTime(dateTime)){
      // In contrast to tm_gmtoff, the timeZone struct does not automatically
      // contain the daylightSavingTime shift and must be subtracted manually.
      timeZone.tz_dsttime = 1;
      timeZone.tz_minuteswest -= (int)NA_MINUTES_PER_HOUR;
    }else{
      timeZone.tz_dsttime = 0;
    }
    return timeZone;
  }


  NA_DEF NADateTime naMakeDateTimeFromTimeSpec(const struct timespec* timeSpec, const NATimeZone* timeZone){
    NADateTime dateTime;
    NAi64 dateTimesec;
    dateTime.errorNum = NA_DATETIME_ERROR_NONE;
    #if NA_TYPE_NATIVE_LONG_BITS == 32
      dateTimesec = naMakei64WithLo((int32)timeSpec->tv_sec + naCasti64Toi32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO));
    #else
      #if !defined NA_TYPE_INT64
        // We fall back to 32 bits as there simply is no solution to this problem.
        #error "impossible to convert 64 bit integer. Falling back to 32 bits"
        dateTimesec = naMakei64WithLo(timeSpec->tv_sec + naCasti64Toi32(NA_DATETIME_SISEC_UNIX_YEAR_ZERO));
      #else
        dateTimesec = naAddi64(timeSpec->tv_sec, NA_DATETIME_SISEC_UNIX_YEAR_ZERO);
      #endif
    #endif
    if(naGreateri64(dateTimesec, NA_ZERO_i64)){
      NAInt taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(dateTimesec);
      dateTime.siSecond = naAddi64(dateTimesec, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec));
    }else{
      dateTime.siSecond = NA_ZERO_i64; // todo
    }
    dateTime.nanoSecond = (int32)timeSpec->tv_nsec;
    if(timeZone){
      // todo: Maybe find a better method than this old timezone struct.
      NABool daylightSaving = timeZone->tz_dsttime;
      dateTime.shift = naMakeShiftFromTimeZone(timeZone, daylightSaving);
      dateTime.flags = (daylightSaving ? NA_DATETIME_FLAG_DAYLIGHT_SAVING_TIME : 0);
    }else{
      dateTime.shift = na_GlobalTimeShift;
      dateTime.flags = ((na_GlobalDaylightSavingTime) ? NA_DATETIME_FLAG_DAYLIGHT_SAVING_TIME : 0);
    }
    return dateTime;
  }


  NA_DEF NADateTime naMakeDateTimeFromTimeVal(const struct timeval* timeVal, const NATimeZone* timeZone){
    struct timespec timeSpec;
    timeSpec.tv_sec  = timeVal->tv_sec;
    timeSpec.tv_nsec = timeVal->tv_usec * 1000;
    return naMakeDateTimeFromTimeSpec(&timeSpec, timeZone);
  }


#endif




NA_DEF void naExtractDateTimeInformation(
  const NADateTime* dateTime,
  NADateTimeStruct* dts,
  NADateTimeAttribute* dta)
{
  NAi64 remainingSeconds;
  NAi64 years400;
  NAi64 years100;
  NAi64 years4;
  NAi64 remainingYears;
  int32 dayOfYear = 0;
  NABool isLeapYear;
  NABool exception100;
  NAInt l, r;

  dts->year = NA_ZERO_i64;
  dts->mon = 0;
  dts->day = 0;
  dts->hour = 0;
  dts->min = 0;
  dts->sec = 0;
  dts->nanoSecond = dateTime->nanoSecond;
  dts->shift = dateTime->shift;
  dts->errorNum = dateTime->errorNum;
  dts->flags = dateTime->flags;
  remainingSeconds = naAddi64(dateTime->siSecond, naMakei64WithLo(dts->shift * (int32)NA_SECONDS_PER_MINUTE));

  if(naSmalleri64(remainingSeconds, NA_DATETIME_SISEC_START_GREGORIAN_PERIOD)){
    // julian system with astronomic year numbering
    remainingSeconds = naSubi64(remainingSeconds, NA_DATETIME_SISEC_JULIAN_YEAR_ZERO);

    years4 = naDivi64(remainingSeconds, NA_SECONDS_IN_4_YEAR_PERIOD);
    if(naSmalleri64(remainingSeconds, NA_ZERO_i64)){naDeci64(years4);}
    dts->year = naAddi64(dts->year, naMuli64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
    remainingSeconds = naSubi64(remainingSeconds, naMuli64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));
    if(naGreaterEquali64(remainingSeconds, NA_SECONDS_IN_LEAP_YEAR)){
      naInci64(dts->year);
      remainingSeconds = naSubi64(remainingSeconds, NA_SECONDS_IN_LEAP_YEAR);

      remainingYears = naDivi64(remainingSeconds, NA_SECONDS_IN_NORMAL_YEAR);
      dts->year = naAddi64(dts->year, remainingYears);
      remainingSeconds = naSubi64(remainingSeconds, naMuli64(remainingYears, NA_SECONDS_IN_NORMAL_YEAR));
    }
    isLeapYear = naIsLeapYearJulian(dts->year);
  }else if(naSmalleri64(remainingSeconds, NA_ZERO_i64) || naGreaterEquali64(remainingSeconds, naTAIPeriods[NA_TAI_PERIODS_COUNT-1].startsiSecond)){
    // gregorian system
    if(naGreaterEquali64(remainingSeconds, naTAIPeriods[NA_TAI_PERIODS_COUNT-1].startsiSecond)){
      remainingSeconds = naSubi64(remainingSeconds, naSubi64(naTAIPeriods[NA_TAI_PERIODS_COUNT-1].startsiSecond, naTAIPeriods[NA_TAI_PERIODS_COUNT-1].startgregsec));
    }
    remainingSeconds = naSubi64(remainingSeconds, NA_DATETIME_SISEC_GREGORIAN_YEAR_ZERO);

    years400 = naDivi64(remainingSeconds, NA_SECONDS_IN_400_YEAR_PERIOD);
    dts->year = naAddi64(dts->year, naMuli64(years400, NA_DATETIME_GREGORIAN_400_YEAR_PERIOD));
    remainingSeconds = naSubi64(remainingSeconds, naMuli64(years400, NA_SECONDS_IN_400_YEAR_PERIOD));

    exception100 = NA_FALSE;
    // The first 100-year period of a 400-year period has a leap day
    if(naGreaterEquali64(remainingSeconds, naAddi64(NA_SECONDS_IN_100_YEAR_PERIOD, naMakei64WithLo(NA_SECONDS_PER_DAY)))){
      exception100 = NA_TRUE;
      dts->year = naAddi64(dts->year, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD);
      remainingSeconds = naSubi64(remainingSeconds, naAddi64(NA_SECONDS_IN_100_YEAR_PERIOD, naMakei64WithLo(NA_SECONDS_PER_DAY)));

      years100 = naDivi64(remainingSeconds, NA_SECONDS_IN_100_YEAR_PERIOD);
      if(!naEquali64(years100, NA_ZERO_i64)){
        dts->year = naAddi64(dts->year, naMuli64(years100, NA_DATETIME_GREGORIAN_100_YEAR_PERIOD));
        remainingSeconds = naSubi64(remainingSeconds, naMuli64(years100, NA_SECONDS_IN_100_YEAR_PERIOD));
      }
    }

    if(exception100){
      if(naGreaterEquali64(remainingSeconds, naMuli64(NA_DATETIME_GREGORIAN_4_YEAR_PERIOD, NA_SECONDS_IN_NORMAL_YEAR))){
        dts->year = naAddi64(dts->year, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD);
        remainingSeconds = naSubi64(remainingSeconds, naMuli64(NA_DATETIME_GREGORIAN_4_YEAR_PERIOD, NA_SECONDS_IN_NORMAL_YEAR));
      }else if(naGreaterEquali64(remainingSeconds, NA_SECONDS_IN_NORMAL_YEAR)){
        naInci64(dts->year);
        remainingSeconds = naSubi64(remainingSeconds, NA_SECONDS_IN_NORMAL_YEAR);

        remainingYears = naDivi64(remainingSeconds, NA_SECONDS_IN_NORMAL_YEAR);
        dts->year = naAddi64(dts->year, remainingYears);
        remainingSeconds = naSubi64(remainingSeconds, naMuli64(remainingYears, NA_SECONDS_IN_NORMAL_YEAR));
      }
    }

    years4 = naDivi64(remainingSeconds, NA_SECONDS_IN_4_YEAR_PERIOD);
    dts->year = naAddi64(dts->year, naMuli64(years4, NA_DATETIME_GREGORIAN_4_YEAR_PERIOD));
    remainingSeconds = naSubi64(remainingSeconds, naMuli64(years4, NA_SECONDS_IN_4_YEAR_PERIOD));

    if(naGreaterEquali64(remainingSeconds, NA_SECONDS_IN_LEAP_YEAR)){
      naInci64(dts->year);
      remainingSeconds = naSubi64(remainingSeconds, NA_SECONDS_IN_LEAP_YEAR);

      remainingYears = naDivi64(remainingSeconds, NA_SECONDS_IN_NORMAL_YEAR);
      dts->year = naAddi64(dts->year, remainingYears);
      remainingSeconds = naSubi64(remainingSeconds, naMuli64(remainingYears, NA_SECONDS_IN_NORMAL_YEAR));
    }

    isLeapYear = naIsLeapYearGregorian(dts->year);
  }else{
    // gregorian system with leap second information
    r = naGetTAIPeriodIndexForSISecond(remainingSeconds);
    dts->year = naMakei64WithLo(naTAIPeriods[r].gregyear);
    remainingSeconds = naSubi64(remainingSeconds, naTAIPeriods[r].startsiSecond);

    isLeapYear = naIsLeapYearGregorian(dts->year);

    switch(naTAIPeriods[r].indicator){
    case NA_START_JANUARY_FIRST:              dts->mon = 0 ;
                                              dayOfYear = 0;
                                              break;
    case NA_START_JULY_FIRST:                 dts->mon = 6 ;
                                              dayOfYear = (isLeapYear ? 182 : 181);
                                              break;
    case NA_POSITIVE_LEAP_SECONDS_JUNE:       dts->mon = 5 ;
                                              dayOfYear = (isLeapYear ? 181 : 180);
                                              dts->hour = 23;
                                              dts->min = 59;
                                              dts->sec = 60;
                                              break;
    case NA_POSITIVE_LEAP_SECONDS_DECEMBER:   dts->mon = 11;
                                              dayOfYear = (isLeapYear ? 365 : 364);
                                              dts->hour = 23;
                                              dts->min = 59;
                                              dts->sec = 60;
                                              break;
    }
  }

  // reaching here, a base date is set as well as the remaining seconds.
  dayOfYear += naCasti64Toi32(naDivi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_DAY)));
  remainingSeconds = naModi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_DAY));
  dts->hour += naCasti64Toi32(naDivi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_HOUR)));
  remainingSeconds = naModi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_HOUR));
  dts->min += naCasti64Toi32(naDivi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_MINUTE)));
  remainingSeconds = naModi64(remainingSeconds, naMakei64WithLo(NA_SECONDS_PER_MINUTE));
  dts->sec += naCasti64Toi32(remainingSeconds);

  // get the correct date.
  l = 0;
  r = 11;
  while(l != r){  // binary search
    NAInt m = (l+r)/2;
    if(na_CumulativeMonthStartDays[2 * (m+1) + (NAInt)isLeapYear] <= dayOfYear){l = m + 1;}else{r = m;}
  }
  // r now defines the index of the month
  dts->mon = (int32)r;
  dts->day = dayOfYear - na_CumulativeMonthStartDays[2 * r + (NAInt)isLeapYear];

  if(dta){
    int32 d;
    int32 mon;
    NAi64 firstterm;
    NAi64 y;
    NAi64 K;
    NAi64 J;

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

    dta->dayOfYear = dayOfYear;
    dta->isLeapYear = (int32)isLeapYear;
    dta->daysinmonth = na_CumulativeMonthStartDays[2 * (r + 1) + dta->isLeapYear] - na_CumulativeMonthStartDays[2 * r + dta->isLeapYear];
    dta->yearsign = naSmalleri64(dts->year, NA_ZERO_i64)?-1:+1;

    // Weekday computation
    d = dts->day + 1;
    y = dts->year;
    mon = dts->mon + 1;
    if(mon<3){mon += 12; naDeci64(y);}
    K = naModi64(naAddi64(naModi64(y, naMakei64WithLo(100)), naMakei64WithLo(100)), naMakei64WithLo(100));
    if(naSmalleri64(y, NA_ZERO_i64)){
      J = naDivi64(naAddi64(y, NA_ONE_i64), naMakei64WithLo(100));
    }else{
      J = naDivi64(y, naMakei64WithLo(100));
    }
    if(naSmalleri64(y, NA_ZERO_i64)){naDeci64(J);}
    firstterm = naAddi64(naAddi64(naMakei64WithLo(d + ((mon + (int32)1) * (int32)26) / (int32)10), K), naDivi64(K, naMakei64WithLo(4)));
    if(naSmalleri64(naAddi64(dateTime->siSecond, naMakei64WithLo(dts->shift * (int32)NA_SECONDS_PER_MINUTE)), NA_DATETIME_SISEC_START_GREGORIAN_PERIOD)){
      // Julian weedkday computation
      dta->weekday = naCasti64Toi32(naModi64((naAddi64((naModi64((naAddi64(naAddi64(firstterm, naMakei64WithLo(5)), naMuli64(naMakei64WithLo(6), J))), naMakei64WithLo(7))), naMakei64WithLo(12))), naMakei64WithLo(7)));
    }else{
      // Gregorian weedkday computation
      dta->weekday = naCasti64Toi32(naModi64((naAddi64((naModi64((naAddi64(naAddi64(firstterm, naDivi64(J, naMakei64WithLo(4))), naMuli64(naMakei64WithLo(5), J))), naMakei64WithLo(7))), naMakei64WithLo(12))), naMakei64WithLo(7))); // Zeller algorihm
    }
  }
}






NA_DEF NAString* naNewStringFromSecondDifference(double difference,
                                                 uint8 decimalDigits){
  NAi64 allseconds, powten, decimals;
  uint8 seconds, minutes, hours;
  NAString* decimalstring;
  NAString* timestring;
  NAString* daystring;
  NAString* signstring;
  NAString* string;

  NABool needsign = NA_FALSE;
  if(difference < 0){needsign = NA_TRUE; difference = -difference;}
  powten = naExp10i64(naMakei64WithLo(decimalDigits));
  allseconds = naMakei64WithDouble(difference * naCasti64ToDouble(powten));
  decimals = naModi64(allseconds, powten);
  allseconds = naDivi64(allseconds, powten);
  seconds = naCasti64Tou8(naModi64(allseconds, naMakei64WithLo(NA_SECONDS_PER_MINUTE)));
  allseconds = naDivi64(allseconds, naMakei64WithLo(NA_SECONDS_PER_MINUTE));
  minutes = naCasti64Tou8(naModi64(allseconds, naMakei64WithLo(NA_MINUTES_PER_HOUR)));
  allseconds = naDivi64(allseconds, naMakei64WithLo(NA_MINUTES_PER_HOUR));
  hours = naCasti64Tou8(naModi64(allseconds, naMakei64WithLo(NA_HOURS_PER_DAY)));
  allseconds = naDivi64(allseconds, naMakei64WithLo(NA_HOURS_PER_DAY));

  if(decimalDigits){
    NAString* decimalformatstring;
    decimalformatstring = naNewStringWithFormat(".%%0%dlld", decimalDigits);
    decimalstring = naNewStringWithFormat(naGetStringUTF8Pointer(decimalformatstring), decimals);
    naDelete(decimalformatstring);
  }else{
    decimalstring = naNewString();
  }

  timestring = naNewStringWithFormat("%02d:%02d:%02d", hours, minutes, seconds);

  if(!naEquali64(allseconds, NA_ZERO_i64)){
    daystring = naNewStringWithFormat("%lldd ", allseconds);
  }else{
    daystring = naNewString();
  }

  if(needsign){
    signstring = naNewStringWithFormat("-");
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





NA_DEF NAInt naGetLeapSecondCorrectionConstant(NAi64 oldUncertainSecondNumber){
  NAInt taiperiod;
  if(naSmalleri64(oldUncertainSecondNumber, NA_ZERO_i64)){return NA_DATETIME_INVALID_UNCERTAIN_SECOND_NUMBER;}
  // Note that the last entry of the structure storing all TAI periods always
  // is a non-leap-second-entry.
  if(naEquali64(oldUncertainSecondNumber, naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startsiSecond)){return NA_DATETIME_NO_CORRECTION_NEEDED;}
  if(naGreateri64(oldUncertainSecondNumber, naTAIPeriods[NA_TAI_PERIODS_COUNT - 1].startsiSecond)){return NA_DATETIME_NEW_LIBRARY_IS_OLDER_THAN_BEFORE;}
  taiperiod = naGetTAIPeriodIndexForSISecond(oldUncertainSecondNumber);
  // Find the earliest second which needs correction.
  while(taiperiod < NA_TAI_PERIODS_COUNT){
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_JUNE){return taiperiod;}
    if(naTAIPeriods[taiperiod].indicator == NA_POSITIVE_LEAP_SECONDS_DECEMBER){return taiperiod;}
    taiperiod++;
  }
  // No leap second has been introduced in the meantime.
  return NA_DATETIME_NO_CORRECTION_NEEDED;
}



NA_DEF void naSetGlobalTimeShiftToSystemSettings(){
  #if NA_OS == NA_OS_WINDOWS
    NATimeZone curTimeZone;
    // Daylight saving is active if the function returns 2.
    NAInt daylightCode = GetTimeZoneInformation(&curTimeZone);
    NABool daylightSaving = (daylightCode == 2);
    na_GlobalTimeShift = naMakeShiftFromTimeZone(&curTimeZone, daylightSaving);
    na_GlobalDaylightSavingTime = (daylightSaving ? NA_TRUE : NA_FALSE);
  #elif NA_OS == NA_OS_MAC_OS_X
    NATimeZone curTimeZone;
    gettimeofday(NULL, &curTimeZone);
    NABool daylightSaving = curTimeZone.tz_dsttime;
    na_GlobalTimeShift = naMakeShiftFromTimeZone(&curTimeZone, daylightSaving);
    na_GlobalDaylightSavingTime = (daylightSaving ? NA_TRUE : NA_FALSE);
  #endif
}



NA_DEF NAi64 naGetFirstUncertainSecondNumber(){
  // The first uncertain second number is here defined to be the first second
  // of the last known TAI period.
  return naTAIPeriods[NA_TAI_PERIODS_COUNT-1].startsiSecond;
}



NA_DEF void naCorrectDateTimeForLeapSeconds(NADateTime* dateTime, NAInt leapSecondCorrectionConstant){
  dateTime->errorNum = NA_DATETIME_ERROR_NONE;
  if((leapSecondCorrectionConstant > 0) && naGreaterEquali64(dateTime->siSecond, naTAIPeriods[leapSecondCorrectionConstant].startsiSecond)){
    NAInt taiperiod;
    // Correcting a date for leap seconds means that the siSecondonds stored are
    // greaterequal to an entry in the TAI periods structure which introduces
    // a leap second. When storing such a date with an earlier version of NALib,
    // no leap second introduction is assumed. Therefore, now as the leap second
    // is known, it must be added to the date.
    //
    // To do so, we subtract all previously known leap seconds...
    dateTime->siSecond = naSubi64(dateTime->siSecond, naSubi64(naTAIPeriods[leapSecondCorrectionConstant-1].startsiSecond, naTAIPeriods[leapSecondCorrectionConstant-1].startgregsec));
    // And add the correct number of leap seconds anew:
    taiperiod = naGetLatestTAIPeriodIndexForGregorianSecond(dateTime->siSecond);
    dateTime->siSecond = naAddi64(dateTime->siSecond, naSubi64(naTAIPeriods[taiperiod].startsiSecond, naTAIPeriods[taiperiod].startgregsec));
  }
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
