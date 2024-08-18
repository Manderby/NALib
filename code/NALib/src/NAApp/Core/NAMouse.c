
#include "NAAppCore.h"

#if NA_COMPILE_GUI == 1

#include "../NAMouse.h"



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



NA_DEF const NAMouseStatus* naGetMouseStatus() {
  return na_App->mouseStatus;
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


#endif // NA_COMPILE_GUI == 1
