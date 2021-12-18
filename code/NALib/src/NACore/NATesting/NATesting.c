
#include "../../NATesting.h"

#if NA_TESTING_ENABLED == 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if NA_OS == NA_OS_WINDOWS
  #include <windows.h>
  #include <Tlhelp32.h>
#elif NA_OS == NA_OS_MAC_OS_X
  #include <sys/time.h>
  #include <sys/sysctl.h>
  #include <libproc.h>
  #include <errno.h>
#endif

#include "../../NAStack.h"
#include "../../NAList.h"
#include "../../NAString.h"
#include "../../NAURL.h"



// How many precomputed random numbers.
#define NA_TEST_INDEX_COUNT 0x10000
#define NA_TEST_INDEX_MASK (NA_TEST_INDEX_COUNT - 1)



typedef struct NATestData NATestData;
struct NATestData {
  const char* name;
  int lineNum;
  NABool success;
  NAStack childs;
  int childSuccessCount;
  int leafSuccessCount;
  int totalLeafCount;
  NATestData* parent;
};

typedef struct NATesting NATesting;
struct NATesting {
  NATestData* rootTestData;
  NATestData* curTestData;
  double timePerBenchmark;
  NABool printAllTestGroups;
  NABool testCaseRunning;
  NABool letCrashTestsCrash;
  NABool testingStartSuccessful;
  int errorCount;
  NAStack untestedStrings;
  NAList testRestriction;
  NAListIterator restrictionIt;
  int curInIndex;
  uint32 in[NA_TEST_INDEX_COUNT];
  #if NA_OS == NA_OS_WINDOWS
    HANDLE logFile;
  #elif NA_OS == NA_OS_MAC_OS_X
    NAFile* logFile;
  #endif
};

NATesting* na_Testing = NA_NULL;



NA_HDEF NAString* na_NewTestApplicationPath(void){
  NAString* exePath;
  #if NA_OS == NA_OS_WINDOWS
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    exePath = naNewStringFromSystemString(modulePath);
  #elif NA_OS == NA_OS_MAC_OS_X
    char pathBuf[PROC_PIDPATHINFO_MAXSIZE];
    pid_t pid = (pid_t)getpid();
    proc_pidpath (pid, pathBuf, sizeof(pathBuf));
    exePath = naNewStringWithFormat("%s", pathBuf);
  #endif
  // This is how Linux would do it:
//    char dest[PATH_MAX];
//    memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
//    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
//      perror("readlink");
//    } else {
//      printf("%s\n", dest);
//    }
//    return naNewStringWithFormat("%s", dest);
  return exePath;
}



NA_HIDEF void na_InitTestingData(NATestData* testData, const char* name, NATestData* parent, int lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NA_TRUE;
  naInitStack(&(testData->childs), sizeof(NATestData), 0, 0);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



NA_HIDEF void na_ClearTestingData(NATestData* testData){
  naForeachStackMutable(&(testData->childs), (NAMutator)na_ClearTestingData);
  naClearStack(&(testData->childs));
}



NA_HDEF NAString* na_NewTestPath(NATestData* testData, NABool escape){
  NAString* str = naNewStringWithFormat("%s", testData->name);
  if(escape)
  {
    NAString* escapeName = naNewStringCEscaped(str);
    naDelete(str);
    str = naNewStringWithFormat("\"%s\"", naGetStringUTF8Pointer(escapeName));
    naDelete(escapeName);
  }

  if(testData->parent){
    NAString* parentName = na_NewTestPath(testData->parent, escape);
    NAString* combinedName = naNewStringWithFormat("%s %s", naGetStringUTF8Pointer(parentName), naGetStringUTF8Pointer(str));
    naDelete(parentName);
    naDelete(str);
    str = combinedName;
  }
  return str;
}



NA_HIDEF void na_PrintTestName(NATestData* testData){
  NAString* testPath = na_NewTestPath(testData, NA_FALSE);
  printf("%s", naGetStringUTF8Pointer(testPath));
  naDelete(testPath);
}



NA_HIDEF void na_PrintRatio(int successCount, int totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf (" (%.02f%%)", ratio);
  }
}



NA_HIDEF void na_PrintTestGroup(NATestData* testData){
  int childTotalCount = (int)naGetStackCount(&(testData->childs));
  if(testData->totalLeafCount == 0){return;}

  printf("G ");
  if(testData->parent){na_PrintTestName(testData);}
  if(testData->totalLeafCount == childTotalCount){
    printf(": %d / %d Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NA_NL);
  }else{
    printf(": %d / %d Groups ok", testData->childSuccessCount, childTotalCount);
    na_PrintRatio(testData->childSuccessCount, childTotalCount);
    printf(", %d / %d Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NA_NL);
  }
}



NA_DEF NABool naStartTesting(const NAUTF8Char* rootName, double timePerBenchmark, NABool printAllGroups, int argc, const char** argv){
  #if NA_DEBUG
    if(na_Testing)
      naError("Testing already running.");
  #endif

//  if(argc > 1){
//    printf("Runnging tests with args:" NA_NL);
//    for(int i = 1; i < argc; i++){
//      printf("%s ", argv[i]);
//    }
//  }

  na_Testing = naAlloc(NATesting);

  na_Testing->rootTestData = naAlloc(NATestData);
  na_InitTestingData(na_Testing->rootTestData, rootName, NA_NULL, 0);

  na_Testing->curTestData = na_Testing->rootTestData;
  na_Testing->timePerBenchmark = timePerBenchmark;
  na_Testing->printAllTestGroups = printAllGroups;
  na_Testing->letCrashTestsCrash = NA_FALSE;
  na_Testing->testingStartSuccessful = NA_FALSE;
  na_SetTestCaseRunning(NA_FALSE);
  na_ResetErrorCount();

  for(na_Testing->curInIndex = 0; na_Testing->curInIndex < NA_TEST_INDEX_COUNT; na_Testing->curInIndex++){
    na_Testing->in[na_Testing->curInIndex] = ((uint32)rand() << 20) ^ ((uint32)rand() << 10) ^ ((uint32)rand());
  }

  naInitStack(&(na_Testing->untestedStrings), sizeof(NAString*), 0, 0);
  naInitList(&(na_Testing->testRestriction));

  if(argc > 1){
    for(int i = 1; i < argc; i++)
    {
      if(argv[i][0] == '-'){
        if(argv[i][1] == 'C'){
          na_Testing->letCrashTestsCrash = NA_TRUE;
        }else{
          printf("Unrecognized executable argument: %c" NA_NL, argv[i][1]);
        }
      }else{
        NAString* argString = naNewStringWithFormat("%s", argv[i]);
        if(naGetStringChar(argString, 0) == '\"'){
          NAString* newArgString = naNewStringExtraction(argString, 1, -2);
//          printf("Quotes detected: %s -> %s\n", naGetStringUTF8Pointer(argString), naGetStringUTF8Pointer(newArgString));
          naDelete(argString);
          argString = newArgString;
        }
        naAddListLastMutable(&(na_Testing->testRestriction), argString);
      }
    }
  }
  
  if(naIsListEmpty(&(na_Testing->testRestriction))){
    naAddListLastMutable(&(na_Testing->testRestriction), naNewStringWithFormat("*"));
  }
  na_Testing->restrictionIt = naMakeListAccessor(&(na_Testing->testRestriction));
  naIterateList(&(na_Testing->restrictionIt));

  NAString* modulePath = na_NewTestApplicationPath();
  NAString* runPath = naNewStringWithBasenameOfPath(modulePath);
  NAString* crashLogPath = naNewStringWithFormat("%s_latestCrash.log", naGetStringUTF8Pointer(runPath));

  #if NA_OS == NA_OS_WINDOWS
    SECURITY_ATTRIBUTES securityAttributes;
      securityAttributes.nLength = sizeof(securityAttributes);
      securityAttributes.lpSecurityDescriptor = NULL;
      securityAttributes.bInheritHandle = TRUE;
    TCHAR* systemCrashLogPath = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(crashLogPath));
    na_Testing->logFile = CreateFile(systemCrashLogPath,
          FILE_APPEND_DATA,
          FILE_SHARE_WRITE | FILE_SHARE_READ,
          &securityAttributes,
          CREATE_ALWAYS,
          FILE_ATTRIBUTE_NORMAL,
          NULL );
    naFree(systemCrashLogPath);
  #elif NA_OS == NA_OS_MAC_OS_X
    na_Testing->logFile = naCreateFileWritingPath(naGetStringUTF8Pointer(crashLogPath), NA_FILEMODE_DEFAULT);
  #endif

  naDelete(crashLogPath);
  naDelete(runPath);
  naDelete(modulePath);

  na_Testing->testingStartSuccessful = na_ShallExecuteGroup(rootName);
  return na_Testing->testingStartSuccessful;
}



NA_DEF void naStopTesting(){
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  na_StopTestGroup();

  if(na_Testing->testingStartSuccessful){
    if(na_Testing->rootTestData->success){
      printf("All test successful." NA_NL);
      if(!na_Testing->printAllTestGroups){
        na_PrintTestGroup(na_Testing->rootTestData);
      }
    }
    printf("Testing finished." NA_NL NA_NL);
  }

  na_ClearTestingData(na_Testing->rootTestData);
  naFree(na_Testing->rootTestData);
  naForeachStackpMutable(&(na_Testing->untestedStrings), (NAMutator)naDelete);
  naClearStack(&(na_Testing->untestedStrings));

  naClearListIterator(&(na_Testing->restrictionIt));
  naForeachListMutable(&(na_Testing->testRestriction), (NAMutator)naDelete);
  naClearList(&(na_Testing->testRestriction));

  #if NA_OS == NA_OS_WINDOWS
    CloseHandle(na_Testing->logFile);
  #elif NA_OS == NA_OS_MAC_OS_X
    naReleaseFile(na_Testing->logFile);
  #endif

  naFree(na_Testing);
  na_Testing = NA_NULL;
}



NA_DEF void naPrintUntested(void){
  int stackCount = (int)naGetStackCount(&(na_Testing->untestedStrings));
  if(!stackCount){
    printf(NA_NL "No untested functionality." NA_NL);
  }else{
    printf(NA_NL "Untested functionality (%d):" NA_NL, stackCount);
    NAStackIterator iter = naMakeStackAccessor(&(na_Testing->untestedStrings));
    while(naIterateStack(&iter)){
      const NAString* string = naGetStackCurpConst(&iter);
      printf("U %s" NA_NL, naGetStringUTF8Pointer(string));
    }
    naClearStackIterator(&iter);
  }
}



NA_HDEF void na_UpdateTestParentLeaf(NATestData* testData, NABool leafSuccess){
  if(testData->parent){
    na_UpdateTestParentLeaf(testData->parent, leafSuccess);
  }

  testData->totalLeafCount++;
  if(leafSuccess){
    testData->leafSuccessCount++;
  }else{
    if(testData->success && testData->parent){
      testData->parent->childSuccessCount--;
    }
    testData->success = NA_FALSE;
  }
}



NA_HDEF void na_AddTest(const char* expr, int success, int lineNum){
  #if NA_DEBUG
    if(!na_Testing)
      naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  if(na_GetErrorCount() > 0){
    testData->success = NA_FALSE;
    na_UpdateTestParentLeaf(na_Testing->curTestData, NA_FALSE);
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf(" Line %d: %d errors occured in %s" NA_NL, lineNum, na_GetErrorCount(), expr);
  }else{
    testData->success = (NABool)success;
    na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)success);
    if(!success){
      printf("  ");
      if(testData->parent){na_PrintTestName(testData->parent);}
      printf(" Line %d: %s" NA_NL, lineNum, expr);
    }
  }

  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_AddTestError(const char* expr, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  testData->success = na_GetErrorCount() != 0;
  na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)testData->success);
  if(!testData->success){
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf(" Line %d: No Error raised in %s" NA_NL, lineNum, expr);
  }

  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_AddTestCrash(const char* expr, int lineNum){
  NA_UNUSED(expr);
  NA_UNUSED(lineNum);
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  // Nothing special to be done here. This method is only back iterating in
  // order to finish the application successfully. If that happens, the
  // process returns a successful return value and the calling process knows
  // that no crash occurred.
  
  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_ExecuteCrashProcess(const char* expr, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);

  // We create a parallel process and run this test application again, but
  // with a path to the current test and the -C argument which causes the
  // test app to execute the test even if it is expected to crash.

  #if NA_OS == NA_OS_WINDOWS
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;

    // Set the logFile as the output for stdout and stderr
    naZeron(&startupInfo, sizeof(STARTUPINFOW));
    startupInfo.cb = sizeof(STARTUPINFOW);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = na_Testing->logFile;
    startupInfo.hStdError = na_Testing->logFile;

    NAString* modulePath = na_NewTestApplicationPath();
    NAString* testPath = na_NewTestPath(testData, NA_TRUE);
    
    // DO NOT TURN -C OPTION OFF!!!
    NAString* commandPath = naNewStringWithFormat("%s -C %s", naGetStringUTF8Pointer(modulePath), naGetStringUTF8Pointer(testPath));
    TCHAR* systemCommandPath = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(commandPath));

    BOOL success = CreateProcess(
      NA_NULL,
      systemCommandPath,
      NA_NULL,
      NA_NULL,
      NA_TRUE, /* bInheritHandles */
      0,
      NA_NULL,
      NA_NULL,
      &startupInfo,
      &processInfo
    );

    if(success){
      WaitForSingleObject( processInfo.hProcess, INFINITE );

      DWORD exitCode;
      GetExitCodeProcess(processInfo.hProcess, &exitCode);
      testData->success = exitCode != EXIT_SUCCESS;
      na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)testData->success);

      if(!testData->success){
        printf("  ");
        if(testData->parent){na_PrintTestName(testData->parent);}
        printf(" Line %d: No Crash happened in %s" NA_NL, lineNum, expr);
      }

    }else{
      printf("  ");
      if(testData->parent){na_PrintTestName(testData->parent);}
      printf(" Line %d: CreateProcess failed in %s (System error code %d)." NA_NL, lineNum, expr, GetLastError());
    }

    naFree(systemCommandPath);
    naDelete(modulePath);
    naDelete(testPath);
    naDelete(commandPath);
  
  #elif NA_OS == NA_OS_MAC_OS_X

    // Set the logFile as the output for stdout and stderr
    int oldStdOut = dup(1);
    close(1); //Close stdout
    dup(na_Testing->logFile->desc);
    int oldStdErr = dup(2);
    close(2); //Close stderr
    dup(na_Testing->logFile->desc);

    NAString* modulePath = na_NewTestApplicationPath();

    #define NA_MAX_TEST_INPUT_STRINGS 100
    const char* testPathStrings[NA_MAX_TEST_INPUT_STRINGS];
    size_t curTestPathStringIndex = 0;

    testPathStrings[curTestPathStringIndex] = expr;
    curTestPathStringIndex++;

    NATestData* curTestData = na_Testing->curTestData;
    while(NA_TRUE){
      testPathStrings[curTestPathStringIndex] = curTestData->name;
      curTestPathStringIndex++;
      if(curTestData->parent){
       curTestData = curTestData->parent;
      }else{
        break;
      }
    }

    // Reserve 3 more slots for the args: testAppName -C and a final NULL
    char** const argv = naMalloc((curTestPathStringIndex + 3) * sizeof(const char*));
    argv[0] = naMalloc(naGetStringByteSize(modulePath) + 1);
    argv[0][naGetStringByteSize(modulePath)] = '\0';
    naWriteBufferToData(naGetStringBufferMutable(modulePath), argv[0]);
    argv[1] = "-C"; // DO NOT TURN -C OPTION OFF!!!
    int i = 2;

    for(size_t curBackIndex = 0; curBackIndex < curTestPathStringIndex; curBackIndex++)
    {
      const char* curPathItem = testPathStrings[curTestPathStringIndex - curBackIndex - 1];
      NAString* pathItemString = naNewStringWithFormat("%s", curPathItem);
      NAString* escapedPathItemString = naNewStringCEscaped(pathItemString);
      NAString* encapsulatedPathItemString = naNewStringWithFormat("\"%s\"", naGetStringUTF8Pointer(escapedPathItemString));
      argv[i] = naMalloc(naGetStringByteSize(encapsulatedPathItemString) + 1);
      naWriteBufferToData(naGetStringBufferMutable(encapsulatedPathItemString), argv[i]);
      argv[i][naGetStringByteSize(encapsulatedPathItemString)] = '\0';
      naDelete(encapsulatedPathItemString);
      naDelete(escapedPathItemString);
      naDelete(pathItemString);
      i++;
    }
    argv[curTestPathStringIndex + 2] = NA_NULL;
    
    pid_t childPid = fork();
    if(!childPid){

      // Don't use newly constructed structs of NARuntime here!!!
      // It will cause concurrency errors which are hard to track.

      execv(argv[0], argv);

      // If reaching here, something went wrong. Return success so that the
      // test eventually fails.
      printf("Starting crash test was unsuccessful.\n");
      exit(EXIT_SUCCESS);

    }else{
      int exitCode;
      waitpid(childPid, &exitCode, 0);

      NABool hasExitedNormally = WIFEXITED(exitCode);
//      NABool hasBeenSignaled = WIFSIGNALED(exitCode);
//      int sigNum = WTERMSIG(exitCode);
      int exitStatus = WEXITSTATUS(exitCode);
      testData->success = !hasExitedNormally || exitStatus != EXIT_SUCCESS;
      na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)testData->success);

      // Revert the file descriptors
      close(1);
      dup(oldStdOut);
      close(oldStdOut);
      close(2);
      dup(oldStdErr);
      close(oldStdErr);

      if(!testData->success){
        printf("  ");
        if(testData->parent){na_PrintTestName(testData->parent);}
        printf(" Line %d: No Crash happened in %s" NA_NL, lineNum, expr);
      }
    }
    
    naDelete(modulePath);

  #endif
}



NA_HDEF void na_RegisterUntested(const char* text){
  NAString** string = naPushStack(&(na_Testing->untestedStrings));
  *string = naNewStringWithFormat("%s", text);
}



NA_HDEF NABool na_GetTestCaseRunning(){
  return na_Testing->testCaseRunning;
}



NA_HDEF void na_SetTestCaseRunning(NABool running){
  na_Testing->testCaseRunning = running;
}



NA_HDEF void na_IncErrorCount(void){
  na_Testing->errorCount++;
}



NA_HDEF void na_ResetErrorCount(void){
  na_Testing->errorCount = 0;
}



NA_HDEF int na_GetErrorCount(void){
  return na_Testing->errorCount;
}



NA_HDEF NABool na_LetCrashTestCrash(){
  return na_Testing->letCrashTestsCrash;
}



NA_HDEF NABool na_ShallExecuteGroup(const char* name){
  const NAString* allowedGroup = naGetListCurConst(&(na_Testing->restrictionIt));
  NABool shallExecute =
    naEqualStringToUTF8CString(allowedGroup, "*", NA_TRUE) ||
    naEqualStringToUTF8CString(allowedGroup, name, NA_TRUE);
  if(shallExecute){
    naIterateList(&(na_Testing->restrictionIt));
    if(naIsListAtInitial(&(na_Testing->restrictionIt))){
      // We arrived at the end of the list. Artificially add an asterix and
      // let the iterator point to this new, last entry.
      naAddListLastConst(&(na_Testing->testRestriction), naNewStringWithFormat("*"));
      naIterateListBack(&(na_Testing->restrictionIt));
    }
  }
  return shallExecute;
}



NA_HDEF NABool na_StartTestGroup(const char* name, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NABool shallExecute = na_ShallExecuteGroup(name);
  if(shallExecute)
  {
    NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
    na_InitTestingData(testData, name, na_Testing->curTestData, lineNum);
    na_Testing->curTestData->childSuccessCount++;
    na_Testing->curTestData = testData;
  }
  return shallExecute;
}



NA_HDEF void na_StopTestGroup(){
  #if NA_DEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  if(na_Testing->printAllTestGroups || !na_Testing->curTestData->success){
    na_PrintTestGroup(na_Testing->curTestData);
  }
  na_Testing->curTestData = na_Testing->curTestData->parent;
  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF uint32 na_GetBenchmarkIn(){
  na_Testing->curInIndex = (na_Testing->curInIndex + 1) & NA_TEST_INDEX_MASK;
  return na_Testing->in[na_Testing->curInIndex];
}



NA_HDEF double na_BenchmarkTime(){
  // Note: Reimplemented here because NADateTime uses int64 to compute.
  #if NA_OS == NA_OS_WINDOWS
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    fileTime.dwLowDateTime;
    return fileTime.dwLowDateTime / 10000000.; // 10 mil. see dwLowDateTime
  #else
    struct timeval curtime;
    struct timezone curTimeZone;
    gettimeofday(&curtime, &curTimeZone);
    return curtime.tv_sec + curtime.tv_usec / 1000000.;
  #endif
}



NA_HDEF double na_GetBenchmarkLimit(){
  return na_Testing->timePerBenchmark;
}



NA_HDEF size_t na_GetBenchmarkTestSizeLimit(){
  return 30;  // in bits
}



NA_HDEF void na_PrintBenchmark(double timeDiff, size_t testSize, const char* exprString, int lineNum){
  if(timeDiff < na_GetBenchmarkLimit() || testSize >= naPowerOf2s(na_GetBenchmarkTestSizeLimit())){
      printf("Line %4d: Immeasurable   : %s" NA_NL, lineNum, exprString);
  }else{
    double execsPerSec = testSize / timeDiff;
    if(execsPerSec > 1000000000.)
      printf("Line %4d: %6.2f G : %s" NA_NL, lineNum, execsPerSec * .000000001, exprString);
    else if(execsPerSec > 1000000.)
      printf("Line %4d: %6.2f M : %s" NA_NL, lineNum, execsPerSec * .000001, exprString);
    else if(execsPerSec > 1000.)
      printf("Line %4d: %6.2f k : %s" NA_NL, lineNum, execsPerSec * .001, exprString);
    else
      printf("Line %4d: %6.2f   : %s" NA_NL, lineNum, execsPerSec, exprString);
  }
}



#else // NA_TESTING_ENABLED == 1fCould

NA_DEF NABool naStartTesting(const NAUTF8Char* rootName, double timePerBenchmark, NABool printAllGroups, int argc, const char** argv){
  NA_UNUSED(rootName);
  NA_UNUSED(timePerBenchmark);
  NA_UNUSED(printAllGroups);
  NA_UNUSED(argc);
  NA_UNUSED(argv);
  #if NA_DEBUG
    naError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
  return NA_FALSE;
}



NA_DEF void naStopTesting(){
  #if NA_DEBUG
    naError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
}

#endif // NA_TESTING_ENABLED == 1



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
