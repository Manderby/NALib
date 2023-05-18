
#include "NAFile.h"


// This project has the purpose to create a string of hexadecimal numbers
// out of a given raw image.
//
// The numbers are then used for the na_pixelFont5x9. Each character has a size
// of 5 / 9 pixels. The whole image has a size of 5*16 / 6*9.
//
// The numbers are formatted so they can be directly added to a source file.
//
// You find the .raw image in the res folder. Copy it to the working directory
// when running. You can also find a GIMP image which was the original file
// the .raw image was created from.


#include "../../NALib/src/NAUtility/NAMemory.h"
#include "../../NALib/src/NAStruct/NABuffer.h"

int main(int argc, char *argv[]){
  naStartRuntime();

  NAString* cwd = naNewStringWithCurWorkingDirectory();
  printf("Working Directory: %s\n", naGetStringUTF8Pointer(cwd));
  naDelete(cwd);
  
  printf("Creating Font... ");

  NABuffer* readBuffer = naCreateBufferWithInputPath("miniascii.raw");
  NABufferIterator bufIter = naMakeBufferAccessor(readBuffer);
  NABuffer* outBuffer = naCreateBuffer(NA_FALSE);
  NABufferIterator outIter = naMakeBufferModifier(outBuffer);
  
  int byteCount = 0;
  int bitCount = 0;
  char outByte = 0;
  while(!naIsBufferAtEnd(&bufIter)){
    uint8 byte = naReadBufferu8(&bufIter);
    byteCount += 1;
    naIterateBuffer(&bufIter, 3);
    byteCount += 3;
    
    outByte |= (1 - (byte / 255)) << (7 - bitCount);
    bitCount++;
    if(bitCount == 8){
      naWriteBufferu8(&outIter, outByte);
      outByte = 0;
      bitCount = 0;
    }
  }
  
  int outByteCount = 0;
  NABufferIterator printIter = naMakeBufferAccessor(outBuffer);
  while(!naIsBufferAtEnd(&printIter)){
    uint8 byte = naReadBufferu8(&printIter);
    printf("0x%02x, ", (int)byte);
    outByteCount++;
    if(outByteCount == (5 * 16) / 8){
      outByteCount = 0;
      printf("\n");
    }
  }
  
  naClearBufferIterator(&printIter);
  naClearBufferIterator(&outIter);
  naClearBufferIterator(&bufIter);
  naRelease(readBuffer);
  naRelease(outBuffer);
  naStopRuntime();

  printf("Done\n");
  return 0;
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
