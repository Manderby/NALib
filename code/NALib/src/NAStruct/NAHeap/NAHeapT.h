
// TEMPLATE
// This is an NALib template file. It uses macros which are defined before
// including this file to manipulate the implementation. Go look for the place
// this file is included to find more info.

// Movedown function.
// Perform a reordering of the heap such that a new key can be inserted. The
// return value is the index in which the key can safely be inserted. The
// curIndex points at the starting position and the element at curIndex is
// assumed to be empty (or contain no useful information).
NA_HHDEF NAInt NA_T3(na_HeapMoveDown, NA_T_DONT_MOVE_DOWN_COMPARATOR, NA_T_TYPE, NA_T_USE_BACKPOINTERS)(NAHeap* heap, const void* key, NAInt curIndex){
  #if NA_T_USE_BACKPOINTERS
    NAHeapBackEntry* entries = heap->data;
  #else
    NAHeapEntry* entries = heap->data;
  #endif
  NAInt nextindex = curIndex / 2;
  
  // Go from the leaf to the root and test, where the new element shall lie.
  while((nextindex > 0) && NA_KEY_OP(NA_T_DONT_MOVE_DOWN_COMPARATOR, NA_T_TYPE)(entries[nextindex].key, key)){
    entries[curIndex] = entries[nextindex];
    #if NA_T_USE_BACKPOINTERS
      *(entries[curIndex].backpointer) = curIndex;
    #endif
    curIndex = nextindex;
    nextindex /= 2;
  }
  
  return curIndex;
}



// Moveup function.
// Assumes the given index to point at an element which is empty (or contains
// no useful information). Performs a reordering of the heap such that the
// given key can be inserted at a suitable index and returns this index.
NA_HHDEF NAInt NA_T3(na_HeapMoveUp, NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE, NA_T_USE_BACKPOINTERS)(NAHeap* heap, const void* key, NAInt curIndex){
  #if NA_T_USE_BACKPOINTERS
    NAHeapBackEntry* entries = heap->data;
  #else
    NAHeapEntry* entries = heap->data;
  #endif

  NAInt indexl = curIndex * 2 + 0;
  NAInt indexr = curIndex * 2 + 1;
  // as long as there is at least one possible position... 
  while(NA_TRUE){
  
    if(indexr > heap->count){
      // There is at least one of the elements out of bounds.
      
      if(indexl <= heap->count){
        // only the left element must be checked. 
        if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, key)){
          entries[curIndex] = entries[indexl];
          #if NA_T_USE_BACKPOINTERS
            *(entries[curIndex].backpointer) = curIndex;
          #endif
          curIndex = indexl;
        }
      }
      // as the leaves have been reached, no need to perform another loop. 
      break;
    }

    if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, entries[indexr].key)){
      // the left element is more important than the right 
      if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, key)){
        // the left is more important than the key 
        entries[curIndex] = entries[indexl];
        #if NA_T_USE_BACKPOINTERS
          *(entries[curIndex].backpointer) = curIndex;
        #endif
        curIndex = indexl;
      }else{
        // noone is more important. 
        break;
      }
    }else{
      // the right element is more important than the left
      if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexr].key, key)){
        // the right is more important than the key 
        entries[curIndex] = entries[indexr];
        #if NA_T_USE_BACKPOINTERS
          *(entries[curIndex].backpointer) = curIndex;
        #endif
        curIndex = indexr;
      }else{
        // noone is more important. 
        break;
      }
    }

    indexl = curIndex * 2 + 0;
    indexr = curIndex * 2 + 1;
  }
  return curIndex;
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
