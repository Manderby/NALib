
#include "NAAppCore.h"



// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF NABool na_IsApplicationRunning(void){
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF void na_InitApplication(NAApplication* application, NANativePtr nativePtr){
  na_App = application;

  naInitList(&(application->windows));
  naInitList(&(application->uiElements));

  application->translator = NA_NULL;
  naStartTranslator();
  
  application->systemFont = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);

  application->mouseStatus.pos = naMakePos(0, 0);
  application->mouseStatus.prevPos = naMakePos(0, 0);
  
  application->curKeyStroke.keyCode = NA_KEYCODE_ESC;
  application->curKeyStroke.modifiers = 0;
  
  application->flags = 0;
  application->flags |= NA_APPLICATION_FLAG_RUNNING;
  application->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  application->name = NA_NULL;
  application->companyName = NA_NULL;
  application->versionString = NA_NULL;
  application->buildString = NA_NULL;
  application->iconPath = NA_NULL;

  // This is done at the very end of the InitApplication function as the
  // application must be fully functional before it can init any UIElements.
  na_InitUIElement(&(application->uiElement), NA_UI_APPLICATION, nativePtr);
}



NA_HDEF void na_ClearApplication(NAApplication* application){
  #if NA_DEBUG
    if(!naGetApplication())
      naCrash("No Application running");
  #endif

  naForeachListMutable(&(naGetApplication()->windows), (NAMutator)naDelete);
  naClearList(&(naGetApplication()->windows));

  naStopTranslator();
  na_ClearUIElement(&(application->uiElement));

  naRelease(application->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&(naGetApplication()->uiElements));
}



NA_HDEF NARect na_GetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_DEF void naStopApplication(void){
  na_App->flags &= ~NA_APPLICATION_FLAG_RUNNING;
}



NA_DEF NAApplication* naGetApplication(void){
  #if NA_DEBUG
    if(!na_App)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_App;
}
