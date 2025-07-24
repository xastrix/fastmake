local proj = "fastmake"

workspace (proj)
    configurations { "Debug", "Release" }
    platforms { "x64", "x86" }

project (proj)
    kind "ConsoleApp"
	language "C++"
	location "src"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	files {
	   "src/dependencies/jsoncpp/src/lib_json/*.cpp",
	   "src/dependencies/jsoncpp/src/lib_json/*.h",

	   "src/fs/**",
	   "src/mod/**",
	   "src/utils/**",
	   "src/builder/**",
	   "src/client/**"
	}

	externalincludedirs "src/dependencies/jsoncpp/include"
	buildoptions "/std:c++latest"
	links "rpcrt4.lib"
	characterset "MBCS"
	warnings "Off"
	optimize "Size"