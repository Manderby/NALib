
#include <stdio.h>

#include "../NALib/NASystem.h"
void printSystemTest(){
  printf("System: %s\n", na_system_strings[NA_SYSTEM]);
  printf("Native Endianness: %s\n", na_endianness_strings[NA_SYSTEM_ENDIANNESS]);
  printf("Bits per Address: %d\n", NA_SYSTEM_ADDRESS_BITS);
  printf("Max 8  Bit Unsigned Integer: %hhu\n", NA_UINT8_MAX);
  printf("Max 16 Bit Unsigned Integer: %hu\n", NA_UINT16_MAX);
  printf("Max 32 Bit Unsigned Integer: %u\n", NA_UINT32_MAX);
  printf("Max 64 Bit Unsigned Integer: %llu\n", NA_UINT64_MAX);
  printf("Max 8  Bit Signed Integer: %hhd\n", NA_INT8_MAX);
  printf("Max 16 Bit Signed Integer: %hd\n", NA_INT16_MAX);
  printf("Max 32 Bit Signed Integer: %d\n", NA_INT32_MAX);
  printf("Max 64 Bit Signed Integer: %lld\n", NA_INT64_MAX);
  printf("Min 8  Bit Signed Integer: %hhd\n", NA_INT8_MIN);
  printf("Min 16 Bit Signed Integer: %hd\n", NA_INT16_MIN);
  printf("Min 32 Bit Signed Integer: %d\n", NA_INT32_MIN);
  printf("Min 64 Bit Signed Integer: %lld\n", NA_INT64_MIN);
  printf("=======\n\n");
}



#include "../NALib/NAEndianness.h"
void printBinaryDataTest(){
  NAByte testarray128[16];
  NAByte testarray64[8];
  NAByte testarray32[4];
  NAByte testarray16[2];
  NAByte testarray8[1];
  NAByte array128[17];
  NAByte array64[9];
  NAByte array32[5];
  NAByte array16[3];
  NAByte array8[2];

  NAEndiannessConverter endconv;

  naSet128(testarray128, 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p');
  naSet128(array128, 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P');
  array128[16] = 0x0;
  printf("Setting 128 Bits: %s\n", array128);
  printf("Equal to same: %s\n", na_boolean_strings[naEqual128(array128, 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P')]);
  printf("Equal to different: %s\n", na_boolean_strings[naEqual128(array128, 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','X')]);
  naSwap128(array128, testarray128);
  printf("Swapped 128 Bits: %s\n", array128);
  naConvertArrayLittleBig128(array128, 1);
  printf("Little-Big-Endian-Conversion: %s\n", array128);
  printf("--------\n");
  
  naSet64(testarray64, 'a','b','c','d','e','f','g','h');
  naSet64(array64, 'A','B','C','D','E','F','G','H');
  array64[8] = 0x0;
  printf("Setting 64 Bits: %s\n", array64);
  printf("Equal to same: %s\n", na_boolean_strings[naEqual64(array64, 'A','B','C','D','E','F','G','H')]);
  printf("Equal to different: %s\n", na_boolean_strings[naEqual64(array64, 'A','B','C','D','E','F','G','X')]);
  naSwap64(array64, testarray64);
  printf("Swapped 64 Bits: %s\n", array64);
  naConvertArrayLittleBig64(array64, 1);
  printf("Little-Big-Endian-Conversion: %s\n", array64);
  printf("--------\n");
  
  naSet32(testarray32, 'a','b','c','d');
  naSet32(array32, 'A','B','C','D');
  array32[4] = 0x0;
  printf("Setting 32 Bits: %s\n", array32);
  printf("Equal to same: %s\n", na_boolean_strings[naEqual32(array32, 'A','B','C','D')]);
  printf("Equal to different: %s\n", na_boolean_strings[naEqual32(array32, 'A','B','C','X')]);
  naSwap32(array32, testarray32);
  printf("Swapped 32 Bits: %s\n", array32);
  naConvertArrayLittleBig32(array32, 1);
  printf("Little-Big-Endian-Conversion: %s\n", array32);
  printf("--------\n");

  naSet16(testarray16, 'a','b');
  naSet16(array16, 'A','B');
  array16[2] = 0x0;
  printf("Setting 16 Bits: %s\n", array16);
  printf("Equal to same: %s\n", na_boolean_strings[naEqual16(array16, 'A','B')]);
  printf("Equal to different: %s\n", na_boolean_strings[naEqual16(array16, 'A','X')]);
  naSwap16(array16, testarray16);
  printf("Swapped 16 Bits: %s\n", array16);
  naConvertArrayLittleBig16(array16, 1);
  printf("Little-Big-Endian-Conversion: %s\n", array16);
  printf("--------\n");

  naSet8(testarray8, 'a');
  naSet8(array8, 'A');
  array8[1] = 0x0;
  printf("Setting 8 Bits: %s\n", array8);
  printf("Equal to same: %s\n", na_boolean_strings[naEqual8(array8, 'A')]);
  printf("Equal to different: %s\n", na_boolean_strings[naEqual8(array8, 'X')]);
  naSwap8(array8, testarray8);
  printf("Swapped 8 Bits: %s\n", array8);
  naConvertArrayLittleBig8(array8, 1);
  printf("Little-Big-Endian-Conversion: %s\n", array8);
  printf("--------\n");
    
  endconv = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_BIG);
  naSet32(array32, 'A','B','C','D');
  printf("Testing Dynamic converter Native to Big: %s", array32);
  endconv.convertArray32(array32, 1);
  printf(" -> %s\n", array32);
  
  endconv = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_LITTLE);
  naSet32(array32, 'A','B','C','D');
  printf("Testing Dynamic converter Native to Little: %s", array32);
  endconv.convertArray32(array32, 1);
  printf(" -> %s\n", array32);

  naFillEndiannessMarker(array32, NA_ENDIANNESS_BIG);
  printf("Big Endian marker: %s", array32);
  printf(" -> %s\n", na_endianness_strings[naParseEndiannessMarker(array32)]);
  naFillEndiannessMarker(array32, NA_ENDIANNESS_LITTLE);
  printf("Little Endian marker: %s", array32);
  printf(" -> %s\n", na_endianness_strings[naParseEndiannessMarker(array32)]);

  printf("=======\n\n");
}




#include "../NALib/NAMathConstants.h"
void printMathConstantsTest(){
  printf("float infinity: %e\n", NA_INFINITYf);
  printf("double infinity: %e\n", NA_INFINITY);
  printf("long double infinity: %Le\n", NA_INFINITYl);
  printf("float singularity: %e\n",  NA_SINGULARITYf);
  printf("double singularity: %e\n",  NA_SINGULARITY);
  printf("long double singularity: %Le\n", NA_SINGULARITYl);
  printf("float subnorm: %1.6f\n",  NA_SUB_NORMf);
  printf("double subnorm: %1.15f\n",  NA_SUB_NORM);
  printf("long double subnorm: %1.18Lf\n", NA_SUB_NORMl);
  printf("float supnorm: %1.6f\n",  NA_SUP_NORMf);
  printf("double supnorm: %1.15f\n",  NA_SUP_NORM);
  printf("long double supnorm: %1.18Lf\n", NA_SUP_NORMl);
  printf("=======\n\n");
}



#include "../NALib/NAMathOperators.h"
void printMathOperatorsTest(){
  printf("Inv of 3: %f\n", naInv(3));
  printf("Sqrt of 42: %f\n", naSqrt(42));
  printf("Cbrt of 42: %f\n", naCbrt(42));
  printf("Fabs of -42: %f\n", naAbs(-42));
  printf("Floor of 1234.5: %f\n", naFloor(1234.5));
  printf("Ceil of 1234.5: %f\n", naCeil(1234.5));
  printf("Round of 1234.5: %f\n", naRound(1234.5));
  printf("Sin of 1/3 pi: %f\n", naSin(NA_PI * NA_THIRD));
  printf("Cos of 1/3 pi: %f\n", naCos(NA_PI * NA_THIRD));
  printf("Tan of 1/3 pi: %f\n", naTan(NA_PI * NA_THIRD));
  printf("Asin of .5: %f\n", naAsin(.5));
  printf("Acos of .5: %f\n", naAcos(.5));
  printf("Atan of .5: %f\n", naAtan(.5));
  printf("Atan2 of -1/0: %f\n", naAtan2(-1, 0.));
  printf("Natural Logarithm of 100: %f\n", naLog(100));
  printf("10-Logarithm of 100: %f\n", naLog10(100));
  printf("2-Logarithm of 100: %f\n", naLog2(100));
  printf("2-Logarithm of integer 100: %i\n", naLog2i32(100));
  printf("Exp e^5: %f\n", naExp(5));
  printf("2-Exp 2^5: %f\n", naExp2(5));
  printf("90 degrees in Rad: %f\n", naDegToRad(90));
  printf("Pi/2 radiant in degrees: %f\n", naRadToDeg(NA_PI_HALF));
  printf("=======\n\n");
}



#include "../NALib/NAComplex.h"
void printComplexTest(){
  NAComplex c1 = naMakeComplex(5, 6);
  NAComplex c2 = naMakeComplexFromPolar(.5, 6.);
  double x = 3.;
  NAComplex c3;
  
  printf("Complex Number c1: %f + %fi\n", c1.re, c1.im);
  printf("Complex Number c2: %f + %fi\n", c2.re, c2.im);
  printf("Rean Number x: %f\n", x);
  
  c3 = naAddComplexReal(c1, x);
  printf("Add real: %f + %fi\n", c3.re, c3.im);
  c3 = naSubComplexReal(c1, x);
  printf("Sub real: %f + %fi\n", c3.re, c3.im);
  c3 = naMulComplexReal(c1, x);
  printf("Mul real: %f + %fi\n", c3.re, c3.im);
  c3 = naDivComplexReal(c1, x);
  printf("Div real: %f + %fi\n", c3.re, c3.im);

  c3 = naAddComplexComplex(c1, c2);
  printf("Add complex: %f + %fi\n", c3.re, c3.im);
  c3 = naSubComplexComplex(c1, c2);
  printf("Sub complex: %f + %fi\n", c3.re, c3.im);
  c3 = naMulComplexComplex(c1, c2);
  printf("Mul complex: %f + %fi\n", c3.re, c3.im);
  c3 = naDivComplexComplex(c1, c2);
  printf("Div complex: %f + %fi\n", c3.re, c3.im);

  c3 = naNegComplex(c1);
  printf("Neg complex c1: %f + %fi\n", c3.re, c3.im);
  c3 = naInvComplex(c1);
  printf("Inv complex c1: %f + %fi\n", c3.re, c3.im);
  c3 = naConjComplex(c1);
  printf("Conj complex c1: %f + %fi\n", c3.re, c3.im);

  printf("Norm complex c2: %f\n", naNormComplex(c2));
  printf("Abs complex c2: %f\n", naAbsComplex(c2));
  printf("Arg complex c2: %f\n", naArgComplex(c2));

  c3 = naExpComplex(c2);
  printf("Exp complex c1: %f + %fi\n", c3.re, c3.im);
  c3 = naSinComplex(c2);
  printf("Sin complex c1: %f + %fi\n", c3.re, c3.im);
  c3 = naCosComplex(c2);
  printf("Cos complex c1: %f + %fi\n", c3.re, c3.im);

  printf("=======\n\n");
}



#include "../NALib/NACoord.h"
void printCoordTest(){
//  printf("Inv of 3: %f\n", Inv(3));
  printf("=======\n\n");
}




#include "../NALib/NAString.h"
void printStringTest(){
  NAInt linenum = 0;
  NAString* token = naCreateString(NULL);
  NAString* string = naCreateStringWithUTF8CString(NULL, "  Hello World !");
  
  printf("The string \"%s\"\nHas Tokens: ", naGetStringConstUTF8Pointer(string));
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("\"%s\", ", naGetStringConstUTF8Pointer(token));
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("\"%s\" and ", naGetStringConstUTF8Pointer(token));
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("\"%s\"\n", naGetStringConstUTF8Pointer(token));
  printf("Remaining string is empty: %s\n", na_boolean_strings[naIsStringEmpty(string)]);
  
  naDestroyString(string);
  string = naCreateStringWithUTF8CString(NULL, "3.44,6243,66,");
  printf("%s\nHas Comma-Tokens: ", naGetStringConstUTF8Pointer(string));
  naParseStringTokenWithDelimiter(string, token, ',');
  naDecoupleString(token);
  printf("\"%s\", ", naGetStringConstUTF8Pointer(token));
  naParseStringTokenWithDelimiter(string, token, ',');
  naDecoupleString(token);
  printf("\"%s\" and ", naGetStringConstUTF8Pointer(token));
  naParseStringTokenWithDelimiter(string, token, ',');
  naDecoupleString(token);
  printf("\"%s\"\n", naGetStringConstUTF8Pointer(token));
  printf("Remaining string is empty: %s\n", na_boolean_strings[naIsStringEmpty(string)]);

  naDestroyString(string);
  linenum = 0;
  printf("abc\\ndef\\r\\n\\nghi\\rjkl has lines:\n");
  string = naCreateStringWithUTF8CString(NULL, "abc\ndef\r\n\nghi\rjkl");
  linenum += naParseStringLine(string, token, NA_TRUE);
  naDecoupleString(token);
  printf(NA$INT": \"%s\"\n", linenum, naGetStringConstUTF8Pointer(token));
  linenum += naParseStringLine(string, token, NA_TRUE);
  naDecoupleString(token);
  printf(NA$INT": \"%s\"\n", linenum, naGetStringConstUTF8Pointer(token));
  linenum += naParseStringLine(string, token, NA_TRUE);
  naDecoupleString(token);
  printf(NA$INT": \"%s\"\n", linenum, naGetStringConstUTF8Pointer(token));
  linenum += naParseStringLine(string, token, NA_TRUE);
  naDecoupleString(token);
  printf(NA$INT": \"%s\"\n", linenum, naGetStringConstUTF8Pointer(token));
  linenum += naParseStringLine(string, token, NA_TRUE);
  naDecoupleString(token);
  printf(NA$INT": \"%s\"\n", linenum, naGetStringConstUTF8Pointer(token));
  printf("Remaining string is empty: %s\n", na_boolean_strings[naIsStringEmpty(string)]);

  naDestroyString(string);
  printf("Tokenization of string (This \"is \"\" a\" Test.)\n");
  printf("with double qoute encapsulation and double qoute escape by doubling:\n");
  string = naCreateStringWithUTF8CString(NULL, "This \"is \"\" a\" Test.");
  string->flags |= NA_STRING_ENCAPSULATE_DOUBLE_QUOTES;
  string->flags |= NA_STRING_ESCAPE_DOUBLE_QUOTE_DOUBLING_WITHIN_DOUBLE_QUOTES;
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("%s, ", naGetStringConstUTF8Pointer(token));
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("%s, ", naGetStringConstUTF8Pointer(token));
  naParseStringToken(string, token);
  naDecoupleString(token);
  printf("%s\n", naGetStringConstUTF8Pointer(token));
  printf("Remaining string is empty: %s\n", na_boolean_strings[naIsStringEmpty(string)]);

  naDestroyString(string);
  naDestroyString(token);
  printf("=======\n\n");
}


#define NA_START_GREGORIAN_PERIOD            (- NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 4LL * NA_SECONDS_IN_4_YEAR_PERIOD - NA_SECONDS_IN_NORMAL_YEAR - (31LL+30LL+31LL-14LL) * NA_SECONDS_PER_DAY)
// After julian Oct 4th 1582 directly comes gregorian Oct 15th 1582.
// The year zero in astronomic year numbering. Equals year 1 BC.
// This assumes the planned leap years by cesar.
#define NA_YEAR_ZERO                         (NA_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+4LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 395LL * NA_SECONDS_IN_4_YEAR_PERIOD)
#define NA_YEAR_ZERO_OF_GREGORIAN_PERIOD     (NA_START_GREGORIAN_PERIOD - (31LL+31LL+30LL+14LL) * NA_SECONDS_PER_DAY - NA_SECONDS_JAN_TO_JUN_IN_NORMAL_YEAR - NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 19LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 3LL * NA_SECONDS_IN_100_YEAR_PERIOD - NA_SECONDS_PER_DAY - 3LL * NA_SECONDS_IN_400_YEAR_PERIOD)
#define NA_GREG_SECONDS_TILL_BEGIN_1970    (3LL * NA_SECONDS_IN_4_YEAR_PERIOD)
#define NA_GREG_SECONDS_SINCE_BEGIN_1601   (-NA_SECONDS_IN_NORMAL_YEAR - NA_SECONDS_IN_LEAP_YEAR - 13LL * NA_SECONDS_IN_4_YEAR_PERIOD - 4LL * NA_SECONDS_IN_NORMAL_YEAR - 2LL * NA_SECONDS_IN_100_YEAR_PERIOD - 24LL * NA_SECONDS_IN_4_YEAR_PERIOD - 3LL * NA_SECONDS_IN_NORMAL_YEAR)


#include "../NALib/NADateTime.h"
void printDateTimeTest(){
  NADateTime timer;
  NADateTime dt;
  NAString string;
  double timediff;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    FILETIME filetime;
  #endif

  printf("Starting timer.\n");
  timer = naMakeDateTimeNow();

  dt = naMakeDateTimeNow();
  naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
  printf("Current datetime: %s\n", naGetStringConstUTF8Pointer(&string));
  naClearString(&string);
  
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    filetime.dwHighDateTime = 0;
    filetime.dwLowDateTime = 0;
    dt = naMakeDateTimeFromFileTime(&filetime, NA_NULL);
    naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
    printf("Windows zero date: %s\n", naGetStringConstUTF8Pointer(&string));
    naClearString(&string);
  #endif

  dt.sisec = NA_GREG_SECONDS_SINCE_BEGIN_1601;
  dt.shift = 0;
  naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
  printf("Year Zero: %s\n", naGetStringConstUTF8Pointer(&string));
  naClearString(&string);
  dt.sisec = NA_GREG_SECONDS_SINCE_BEGIN_1601-1;
  naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
  printf("Year Zero: %s\n", naGetStringConstUTF8Pointer(&string));
  naClearString(&string);

  printf("Sleeping for 150 Milliseconds...\n");
  naSleepM(150);
  
  dt = naMakeDateTimeNow();
  timediff = naGetDateTimeDiff(&dt, &timer);
  naCreateStringFromSecondDifference(&string, timediff, 6);
  printf("Stopping timer. Elapsed time: %s\n", naGetStringConstUTF8Pointer(&string));

  printf("=======\n\n");
}

int main(int argc, const char * argv[]){

  printSystemTest();
  printBinaryDataTest();
  printMathConstantsTest();
  printMathOperatorsTest();
  printComplexTest();
  printCoordTest();
  printStringTest();
  printDateTimeTest();
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);

  return 0;
}

