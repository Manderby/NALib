
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



// To use the GUI implementation of NALib, you need to configure it. You can
// do this by defining the preprocessor macro NA_COMPILE_GUI to be 1.
// If you need OpenGL, do the same thing for DNA_COMPILE_OPENGL.
//
// You can define NA_COMPILE_GUI by adding a preprocessor macro in your project
// settings:
// 
// CMake:
// add_definitions(-DNA_COMPILE_GUI=1)
// add_definitions(-DNA_COMPILE_OPENGL=1)
//
// XCode:
// Add the preprocessor macros to your project or target settings.
//
// VisualStudio:
// Add the preprocessor macros to your project settings.
// Or:
// Provide a property sheet named NAConfiguration.props right next to the
// solution. Add the following lines at an appropriate place in the props file:
//   <ItemDefinitionGroup>
//     <ClCompile>
//       <PreprocessorDefinitions>NA_COMPILE_GUI=1;NA_COMPILE_OPENGL=1;%(PreprocessorDefinitions)</PreprocessorDefinitions>
//     </ClCompile>
//     <Link>
//       <AdditionalDependencies>ComCtl32.lib;opengl32.lib;MSImg32.lib%(AdditionalDependencies)</AdditionalDependencies>
//     </Link>
//   </ItemDefinitionGroup>
// Add the following line to your all jour projects (not the solution!):
// <Import Project="$(SolutionDir)\NAConfiguration.props" Condition="Exists('$(SolutionDir)\NAConfiguration.props')" />
// Use a text editor for that. It is proposed to add this line right after the
// line which imports the property sheet "Microsoft.Cpp.Default.props".
//
// You need to link to the following libraries:
// Windows:
//   ComCtl32
// macOS:
//   Carbon
//   Cocoa
//   UniformTypeIdentifiers
// any OS if needed:
//   OpenGL


// Note that you could add an include path in your project to access the
// NAXXX.h files. But for the sake of simplicity, we use relative paths here.
#include "GUIExamples.h"



int main(){

  naStartRuntime();

    //NAString* pwd = naNewStringWithCurWorkingDirectory();

    naStartApplication(prestartup, poststartup, NA_NULL, NA_NULL);

    // When returning from naStartApplication, the GUI has already been
    // released. But we need to free the memory allocated in the app and
    // the controllers.
    clearApplication();
  naStopRuntime();

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
