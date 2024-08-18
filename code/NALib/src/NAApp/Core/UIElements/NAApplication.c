
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1


// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF NABool na_IsApplicationRunning(void) {
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF void na_InitApplication(NAApplication* app, void* nativePtr) {
  na_App = app;

  naInitList(&app->windows);
  naInitList(&app->uiElements);

  app->translator = NA_NULL;
  naStartTranslator();
  
  // todo: make this a singleton.
  app->systemFont = NA_NULL;

  app->mouseStatus = na_AllocMouseStatus();
  
  app->curKeyStroke.keyCode = NA_KEYCODE_ESCAPE;
  app->curKeyStroke.modifiers = 0;
  
  app->flags = 0;
  app->flags |= NA_APPLICATION_FLAG_RUNNING;
  app->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  #if NA_OS == NA_OS_WINDOWS
  app->flags |= NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING;
  #endif

  app->name = NA_NULL;
  app->companyName = NA_NULL;
  app->versionString = NA_NULL;
  app->buildString = NA_NULL;
  app->resourcePath = NA_NULL;
  app->iconPath = NA_NULL;

  // This is done at the very end of the InitApplication function as the
  // application must be fully functional before it can init any UIElements.
  na_InitUIElement(&app->uiElement, NA_UI_APPLICATION, nativePtr);
}



NA_HDEF void na_ClearApplication(NAApplication* app) {
  #if NA_DEBUG
    if(!naGetApplication())
      naCrash("No Application running");
  #endif

  naForeachListMutable(&naGetApplication()->windows, (NAMutator)naDelete);
  naClearList(&naGetApplication()->windows);

  naStopTranslator();
  na_ClearUIElement(&app->uiElement);

  na_DeallocMouseStatus(app->mouseStatus);

  if(app->systemFont)
    naRelease(app->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&naGetApplication()->uiElements);
}



NA_HDEF NARect na_GetApplicationRect(const NAApplication* app) {
  NA_UNUSED(app);
  NARect rect = {{0., 0.}, {1., 1.}};
  return rect;
}

NA_HDEF void na_SetApplicationRect(const NAApplication* app, NARect rect) {
  NA_UNUSED(app);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("Application rect can not be set.");
  #endif
}




NA_HDEF NAMouseStatus* na_getApplicationMouseStatus(NAApplication* app) {
  return app->mouseStatus;
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



NA_HDEF NABool na_GetApplicationMouseVisible() {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Internal error: application not started. Use naStartApplication");
  #endif
  return naGetFlagu32(na_App->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE);
}


NA_HDEF void na_SetApplicationMouseVisible(NABool visible) {
  #if NA_DEBUG
  if(!na_App)
    naCrash("Internal error: application not started. Use naStartApplication");
  #endif
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE, visible);
}



NA_DEF NABool naGetDefaultWindowSystemKeyHandling() {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Internal error: application not started. Use naStartApplication");
  #endif
  return naGetFlagu32(na_App->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING);
}

NA_DEF void naSetDefaultWindowSystemKeyHandling(NABool enable) {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Internal error: application not started. Use naStartApplication");
  #endif
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING, enable);
}


#endif // NA_COMPILE_GUI == 1
