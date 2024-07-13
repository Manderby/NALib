
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_SetMouseButtonPressed(NAMouseButton button, NABool pressed) {
  naSetFlagu32(&na_App->mouseStatus.buttonPressed, 1 << button, pressed);
}



NA_HDEF void na_SetMouseWarpedTo(NAPos newpos) {
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}



NA_HDEF void na_SetMouseMovedTo(NAPos newpos) {
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos = newpos;
}



NA_HDEF void na_SetMouseMovedByDiff(double deltaX, double deltaY) {
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos.x += deltaX;
  na_App->mouseStatus.pos.y += deltaY;
}



NA_HDEF void na_SetMouseEnteredAtPos(NAPos newpos) {
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}



NA_HDEF void na_SetMouseExitedAtPos(NAPos newpos) {
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos = newpos;
}



NA_DEF const NAMouseStatus* naGetMouseStatus() {
  return &na_App->mouseStatus;
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
