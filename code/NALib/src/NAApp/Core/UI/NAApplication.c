
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1


// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF NABool na_IsApplicationRunning(void) {
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF void na_InitApplication(NAApplication* application, NANativePtr nativePtr) {
  na_App = application;

  naInitList(&application->windows);
  naInitList(&application->uiElements);

  application->translator = NA_NULL;
  naStartTranslator();
  
  // todo: make this a singleton.
  application->systemFont = NA_NULL;

  application->mouseStatus.pos = naMakePos(0, 0);
  application->mouseStatus.prevPos = naMakePos(0, 0);
  application->mouseStatus.buttonPressed = 0;
  
  application->curKeyStroke.keyCode = NA_KEYCODE_ESCAPE;
  application->curKeyStroke.modifiers = 0;
  
  application->flags = 0;
  application->flags |= NA_APPLICATION_FLAG_RUNNING;
  application->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  application->name = NA_NULL;
  application->companyName = NA_NULL;
  application->versionString = NA_NULL;
  application->buildString = NA_NULL;
  application->resourcePath = NA_NULL;
  application->iconPath = NA_NULL;

  // This is done at the very end of the InitApplication function as the
  // application must be fully functional before it can init any UIElements.
  na_InitUIElement(&application->uiElement, NA_UI_APPLICATION, nativePtr);
}



NA_HDEF void na_ClearApplication(NAApplication* application) {
  #if NA_DEBUG
    if(!naGetApplication())
      naCrash("No Application running");
  #endif

  naForeachListMutable(&naGetApplication()->windows, (NAMutator)naDelete);
  naClearList(&naGetApplication()->windows);

  naStopTranslator();
  na_ClearUIElement(&application->uiElement);

  if(application->systemFont)
    naRelease(application->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&naGetApplication()->uiElements);
}



NA_HDEF NARect na_GetApplicationRect(const NAApplication* application) {
  NA_UNUSED(application);
  NARect rect = {{0., 0.}, {1., 1.}};
  return rect;
}

NA_HDEF void na_SetApplicationRect(const NAApplication* application, NARect rect) {
  NA_UNUSED(application);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("Application rect can not be set.");
  #endif
}


NA_DEF void naStopApplication(void) {
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_RUNNING, NA_FALSE);
}



NA_DEF NAApplication* naGetApplication(void) {
  #if NA_DEBUG
    if(!na_App)
      naError("Internal error: application not started. Use naStartApplication");
  #endif
  return na_App;
}



#endif // NA_COMPILE_GUI == 1
