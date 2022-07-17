
#include "NAAppCore.h"

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
    if(!na_App)
      naCrash("No Application running");
  #endif

  naForeachListMutable(&(na_App->windows), (NAMutator)naDelete);
  naClearList(&(na_App->windows));

  naStopTranslator();
  na_ClearUIElement(&(application->uiElement));

  naRelease(application->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&(na_App->uiElements));
}
