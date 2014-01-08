
#include <stdio.h>




const char* na_boolean_strings[2] = {
  "False",
  "True"
};




#include "../NALib/NASystem.h"
// Various values useful for outputting, debugging, ...
const char* na_system_strings[NA_SYSTEM_COUNT] = {
  "Mac OS X",
  "Windows",
  "Unknown"
};
const char* na_endianness_strings[NA_ENDIANNESS_COUNT] = {
  "Native",
  "Little",
  "Big",
  "Unknown",
};
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
  printf("float NaN: %e\n", NA_NANf);
  printf("double NaN: %e\n", NA_NAN);
  printf("long double NaN: %Le\n", NA_NANl);
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
  NAString* string2 = naCreateString(NULL);
  
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
  naDestroyString(string2);
  string = naCreateStringWithUTF8CString(NA_NULL, "This \" text \' will < be > encoded &");
  printf("\nEncoding and decoding String in XML:\n%s\n", naGetStringConstUTF8Pointer(string));
  string2 = naCreateStringXMLEncoded(NA_NULL, string);
  printf("Encoded: %s\n", naGetStringConstUTF8Pointer(string2));
  naDestroyString(string);
  string = naCreateStringXMLDecoded(NA_NULL, string2);
  printf("Decoded: %s\n", naGetStringConstUTF8Pointer(string));
  
  naDestroyString(string);
  naDestroyString(string2);
  string = naCreateStringWithUTF8CString(NA_NULL, "This ( text ) will be \\ encoded");
  printf("\nEncoding and decoding String in EPS:\n%s\n", naGetStringConstUTF8Pointer(string));
  string2 = naCreateStringEPSEncoded(NA_NULL, string);
  printf("Encoded: %s\n", naGetStringConstUTF8Pointer(string2));
  naDestroyString(string);
  string = naCreateStringEPSDecoded(NA_NULL, string2);
  printf("Decoded: %s\n", naGetStringConstUTF8Pointer(string));

  naDestroyString(string);
  naDestroyString(string2);
  string = naCreateStringWithUTF8CString(NA_NULL, "String1");
  string2 = naCreateStringWithUTF8CString(NA_NULL, "String2");
  naAppendStringWithString(string, string2);
  printf("\nAppending: %s\n", naGetStringConstUTF8Pointer(string));
  naDestroyString(string);
  string = naCreateStringWithUTF8CString(NA_NULL, "String1");
  naAppendStringWithFormat(string, "String%d", 3);
  printf("Appending: %s\n", naGetStringConstUTF8Pointer(string));

  naDestroyString(string);
  naDestroyString(string2);
  naDestroyString(token);
  printf("=======\n\n");
}



#include "../NALib/NADateTime.h"
void printDateTimeTest(){
  NADateTime timer;
  NADateTime dt;
  NAString string;
  NADateTimeStruct dts1;
  NADateTimeStruct dts2;
  NADateTimeAttribute dta1;
  NADateTimeAttribute dta2;
  NABool errorfound = NA_FALSE;
  double timediff;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    FILETIME filetime;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    struct timespec timesp;
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
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    timesp.tv_nsec = 0;
    timesp.tv_sec = 0;
    dt = naMakeDateTimeFromTimeSpec(&timesp, NA_NULL);
    naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
    printf("Mac zero date: %s\n", naGetStringConstUTF8Pointer(&string));
    naClearString(&string);
  #endif

  printf("Checking for errors...\n");
  errorfound = NA_FALSE;
  dt.shift = 0;
  dt.sisec = -10LL * NA_SECONDS_IN_400_YEAR_PERIOD;
  naExtractDateTimeInformation(&dt, &dts1, &dta1);
  // Note that the initialization expression in the followin for-loop is empty.
  // The author could write dt.sisec = dt.sisec but this may lead to warnings
  // in some systems.
  for(; dt.sisec < NA_SECONDS_IN_400_YEAR_PERIOD; dt.sisec += 23LL * NA_SECONDS_PER_HOUR){
    naExtractDateTimeInformation(&dt, &dts2, &dta2);
    if((dta2.dayofyear != dta1.dayofyear) && (dta2.weekday != (dta1.weekday + 1) % 7)){
      naCreateStringWithDateTime(&string, &dt, NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
      printf("Weekday error: %s\n", naGetStringConstUTF8Pointer(&string));
      naClearString(&string);
      errorfound = NA_TRUE;
    }
    dts1 = dts2;
    dta1 = dta2;
  }
  if(!errorfound){
    printf("No errors found.\n");
  }

  printf("Sleeping for 150 Milliseconds...\n");
  naSleepM(150);
  
  dt = naMakeDateTimeNow();
  timediff = naGetDateTimeDiff(&dt, &timer);
  naCreateStringFromSecondDifference(&string, timediff, 6);
  printf("Stopping timer. Elapsed time: %s\n", naGetStringConstUTF8Pointer(&string));

  printf("=======\n\n");
}



#include "../NALib/NARandom.h"
void printRandomTest(){
  printf("Random number with default seed: %f\n", naUniformRandZEf());
  naSeedRand(0);
  printf("Random number with time seed: %f\n", naUniformRandZEf());
  printf("=======\n\n");
}


#include "../NALib/NAVectorAlgebra.h"
void printVectorAlgebarTest(){
  NAVec3f v1;
  NAVec3f v2;
  naSetV3fRandom(v1);
  naSetV3fRandomSpherical(v2, NA_FALSE);
  printf("Random Position: %f, %f, %f\n", v1[0], v1[1], v1[2]);
  printf("Random Direction: %f, %f, %f\n", v2[0], v2[1], v2[2]);
  printf("Position length: %f\n", naLengthV3f(v1));
  printf("Direction length: %f\n", naLengthV3f(v2));
  printf("=======\n\n");
}


#include "../NALib/NAList.h"
void printListTest(){
  NAInt* ivalue;
  printf("Testing list\n");
  NAList* list = naCreateList(NA_NULL);
  NAInt values[5] = {1, 2, 3, 4, 5};
  naAddListElementLast(list, &(values[0]));
  naAddListElementLast(list, &(values[1]));
  naAddListElementLast(list, &(values[2]));
  naAddListElementLast(list, &(values[3]));
  naAddListElementLast(list, &(values[4]));
  naFirstListElement(list);
  while((ivalue = (NAInt*)naGetListCurrentContent(list))){
    printf(NA$INT " ", *ivalue);
    naNextListElement(list);
  }
  printf("\n");
  naLastListElement(list);
  while((ivalue = (NAInt*)naGetListCurrentContent(list))){
    printf(NA$INT " ", *ivalue);
    naPrevListElement(list);
  }
  printf("\n");

  printf("Removing Number 3 while iterating\n");
  naFirstListElement(list);
  while((ivalue = (NAInt*)naGetListCurrentContent(list))){
    if(*ivalue == 3){
      naRemoveListElementCurrent(list, NA_FALSE);
    }else{
      printf(NA$INT " ", *ivalue);
    }
    naNextListElement(list);
  }
  printf("\n");

  naDestroyList(list);
  printf("=======\n\n");
}


#include "../NALib/NAByteMap2D.h"
void printByteMap2DTest(){
  printf("=======\n\n");
}


int main(int argc, const char * argv[]){

//  printSystemTest();
//  printBinaryDataTest();
//  printMathConstantsTest();
//  printMathOperatorsTest();
//  printComplexTest();
//  printCoordTest();
//  printStringTest();
//  printDateTimeTest();
//  printRandomTest();
//  printVectorAlgebarTest();
//  printListTest();
  printByteMap2DTest();
  
//  printf("\nPress enter to quit.\n");
//  fgetc(stdin);

  return 0;
}

