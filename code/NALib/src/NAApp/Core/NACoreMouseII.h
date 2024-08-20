
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



struct NAMouseStatus{
  NAPos pos;
  NAPos prevPos;
  uint32 buttonPressed;
};



NA_HDEF NAMouseStatus* na_AllocMouseStatus() {
  NAMouseStatus* status = naAlloc(NAMouseStatus);
  status->pos = naMakePos(0, 0);
  status->prevPos = naMakePos(0, 0);
  status->buttonPressed = 0;
  return status;
}

NA_HDEF void na_DeallocMouseStatus(NAMouseStatus* status) {
  naFree(status);
}



NA_HDEF void na_SetMouseButtonPressed(
  NAMouseStatus* status,
  NAMouseButton button,
  NABool pressed)
{
  naSetFlagu32(&status->buttonPressed, 1 << button, pressed);
}



NA_HDEF void na_SetMouseWarpedTo(
  NAMouseStatus* status,
  NAPos newpos)
{
  status->prevPos = newpos;
  status->pos = newpos;
}



NA_HDEF void na_SetMouseMovedTo(
  NAMouseStatus* status,
  NAPos newpos) {
  status->prevPos = status->pos;
  status->pos = newpos;
}



NA_HDEF void na_SetMouseMovedByDiff(
  NAMouseStatus* status,
  double deltaX,
  double deltaY)
{
  status->prevPos = status->pos;
  status->pos.x += deltaX;
  status->pos.y += deltaY;
}



NA_HDEF void na_SetMouseEnteredAtPos(
  NAMouseStatus* status,
  NAPos newpos)
{
  status->prevPos = newpos;
  status->pos = newpos;
}



NA_HDEF void na_SetMouseExitedAtPos(
  NAMouseStatus* status,
  NAPos newpos)
{
  status->prevPos = status->pos;
  status->pos = newpos;
}



NA_DEF NABool naGetMouseButtonPressed(
  const NAMouseStatus* mouseStatus,
  NAMouseButton button)
{
  return naGetFlagu32(mouseStatus->buttonPressed, 1 << button);
}



NA_DEF NAPos naGetMousePos(const NAMouseStatus* mouseStatus) {
  return mouseStatus->pos;
}



NA_DEF NAPos naGetMouseDelta(const NAMouseStatus* mouseStatus) {
  return naMakePos(
    mouseStatus->pos.x - mouseStatus->prevPos.x,
    mouseStatus->pos.y - mouseStatus->prevPos.y);
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
