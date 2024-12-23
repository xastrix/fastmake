# fastmake-docs
## Samples
#### Sample (Hello world written in C)
```
fastmake docs/samples/hello_world_c
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
## Project settings (Json)
### Project name
```json
"name":"MyFirstProject", /* Example name of VS project (MyFirstProject.vcxproj, MyFirstProject.vcxproj.user) */
"settings": { ...
```
##### Note
How to get project name
```
{$name}
```
Example
```json
"settings": {
  ...,
  "links":["{$name}_lib.lib"], /* MyFirstProject_lib.lib */
  ...
}
```
### Platforms
```json
"settings": {
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
  "include_dirs":["$(ProjectDir)curl\\Include","$(DXSDK_DIR)Include"],
  ...
}
```
### Library directories
```json
"settings": {
  ...,
  "lib_dirs":["$(ProjectDir)curl\\Lib\\x86","$(DXSDK_DIR)Lib\\x86"],
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
    "Header.h" /* Example header file */
  ]
}
```