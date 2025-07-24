# fastmake-docs
## Samples
#### Sample (Hello world written in C)
```
fastmake ../../docs/samples/hello_world_c
```
fastmake-file
```json
{
    "name":"hello_world_c",
    "settings": {
        "configurations":["Debug", "Release"],
        "platforms":["x64", "x86"],
        "files": [
          "main.c"
        ]
    }
}
```
main.c
```c
#include <stdio.h>

int main() {
  printf("Hello World\n");
  return 0;
}
```
#### Sample #2 (Simple dll-library project)
```
fastmake ../../docs/samples/simple_dll_library_builded_by_fastmake
```
fastmake-file
```json
{
    "name":"DllProject",
    "settings": {
        "configurations":["Debug", "Release"],
        "platforms":["x64", "x86"],
        "configuration_type":"dynamic",
        "sub_system":"lib",
        "files":[
          "DllMain.cpp"
        ]
    }
}
```
DllMain.cpp
```c
#include <iostream>
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Hello!", "Dll-library", NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
```
## Project settings (Json)
### Project name
```json
"name":"MyFirstProject", /* Example name of VS project (MyFirstProject.sln, MyFirstProject.vcxproj, ...) */
/* If skip, the default will be set to "UnnamedProject" */
"settings": { ...
```
##### Note
How to get project name
```
{$name}
```
Example
```json
"name":"MyFirstProject", /* Example name of VS project (MyFirstProject.sln, MyFirstProject.vcxproj, ...) */
"settings": {
  ...,
  "links":["{$name}_lib.lib"], /* MyFirstProject_lib.lib */
  ...
}
```
### Platforms
```json
"settings": {
  ...,
  "platforms":["x86","x64"], /* Example platforms */
  ...
}
```
### Configurations
```json
"settings": {
  ...,
  "configurations":["Debug","Release"], /* Example configurations */
  ...
}
```
### Configuration type
```json
"settings": {
  ...,
  "configuration_type":"?",
  /* "app"     - Application / .exe */
  /* "dynamic" - DynamicLibrary / .dll */
  /* "static"  - StaticLibrary / .lib */
  // Application is a default
  ...
}
```
### Sub-system
```json
"settings": {
  ...,
  "sub_system":"?",
  /* "console"    - Console */
  /* "windowed"   - Windows */
  /* "lib"        - Windows */
  // Console is a default
  ...
}
```
### Linking
```json
"settings": {
  ...,
  "links":["shlwapi.lib","d3d9.lib"], /* Example links */
  ...
}
```
### Link options
```json
"settings": {
  ...,
  "link_options":"/NODEFAULTLIB:LIBCMT /LTCG:OFF", /* Example link options */
  ...
}
```
### Output directory
```json
"settings": {
  ...,
  "out_dir":"$(SolutionDir)bin\\",
  /* If skip, the default will be set to "bin\%platform%\%configuration%\" */
  ...
}
```
### Intermediate directory
```json
"settings": {
  ...,
  "int_dir":"$(SolutionDir)obj\\",
  /* If skip, the default will be set to "obj\%platform%\%configuration%\" */
  ...
}
```
### Include directories
```json
"settings": {
  ...,
  "include_dirs":["$(ProjectDir)curl\\Include","$(DXSDK_DIR)Include"], /* Example include directories */
  ...
}
```
### Library directories
```json
"settings": {
  ...,
  "lib_dirs":["$(ProjectDir)curl\\Lib\\x86","$(DXSDK_DIR)Lib\\x86"], /* Example library directories */
  ...
}
```
### Preprocessor defines
```json
"settings": {
  ...,
  "defines":["_CRT_SECURE_NO_WARNINGS","CURLSTATIC_LIB"], /* Example preprocessor defines */
  ...
}
```
### Optimization
```json
"settings": {
  ...,
  "optimization":"?",
  /* "speed" - MaxSpeed: Optimize for the best performance */
  /* "size"  - MinSpace: Optimize for the smallest file size */
  /* "full"  - Full: Full optimization */
  // Disabled is a default
  ...
}
```
### Character set
```json
"settings": {
  ...,
  "characterset":"?",
  /* "mbcs"    - MultiByte */
  /* "uni"     - Unicode */
  // NotSet is a default
  ...
}
```
### Warning level
```json
"settings": {
  ...,
  "warnings":"?",
  /* "default" - Level3 */
  /* "high"    - Level4 */
  /* "all"     - EnableAllWarnings */
  // Level3 is a default
  ...
}
```
### Precompiled headers
```json
"settings": {
  ...,
  "pch":"?",
  /* "stdafx.h" - Use */
  // NotUsing is a default
  ...
}
```
### Function level linking
```json
"settings": {
  ...,
  "function_level_linking":"?",
  /* "on" - true */
  // false is a default
  ...
}
```
### Platformtoolset
```json
"settings": {
  ...,
  "platform_toolset":"?",
  /* "v110"         - Version */
  /* "v140"         - Version */
  /* "v141"         - Version */
  /* "v142"         - Version */
  /* "v143"         - Version */
  /* "kerneldrv8.0" - WindowsKernelModeDriver8.0 */
  /* "userdrv8.0"   - WindowsUserModeDriver8.0 */
  /* "appfordrv8.0" - WindowsApplicationForDrivers8.0 */
  /* "kerneldrv8.1" - WindowsKernelModeDriver8.1 */
  /* "userdrv8.1"   - WindowsUserModeDriver8.1 */
  /* "appfordrv8.1" - WindowsApplicationForDrivers8.1 */
  // v120 is a default
  ...
}
```
### Files
```json
"settings": {
  ...,
  "files":[
    "Source.cpp",  /* Example source file */
    "Header.h", /* Example header file */
  ]
}
```