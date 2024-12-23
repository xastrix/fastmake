#include "builder.h"

#include "../utils/utils.h"
#include "../fs/fs.h"

#include <chrono>

static bool is_cpp(const Json::Value& object);
static bool is_hpp(const Json::Value& object);
static bool is_lib(const Json::Value& object);
static void fast_link(xml_string& string, const xml_string& param);
static void fast_link_closed(xml_string& string, const xml_string& param, const xml_string& param1);

int main(int argc, const char** argv)
{
	if (argc < 2) {
		printf("usage: fastmake <path>\n");
		printf("to get more information read's the docs\n");
		return 1;
	}

	if (!g_mod.initialize(argv[1])) {
		printf("%s\n", g_mod.get_err().c_str());
		return 1;
	}

	auto start = std::chrono::high_resolution_clock::now();

	builder builder;
	switch (builder.run(g_mod.get_project_path())) {
	case be_fail_access: {
		printf("Error: Failed to create VS project files (Don't have permissions)\n");
		break;
	}
	case be_configurations_empty: {
		printf("Error: Configurations are empty\n");
		break;
	}
	case be_platforms_empty: {
		printf("Error: Platforms are empty\n");
		break;
	}
	case be_ok: {
		std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;
		
		printf("\n");
		printf("-- Done (%.0f ms)\n", duration.count());
	}
	}

	return 0;
}

b_err builder::run(const std::string& path)
{
	cfg_obj      = g_mod.get()["settings"]["configurations"];
	platform_obj = g_mod.get()["settings"]["platforms"];

	printf("-- Building...\n");

	if (cfg_obj.empty())
		return be_configurations_empty;

	if (platform_obj.empty())
		return be_platforms_empty;

	for (auto& platform : platform_obj)
		if (platform.asString() == "x86")
			platform = "Win32";

	printf("-- Generating %s%s...\n", g_mod.get_project_name().c_str(), VS_PROJECT_SOLUTION_EXTENSION);

	if (!build_solution_file(path))
		return be_fail_access;

	printf("-- %s%s OK\n", g_mod.get_project_name().c_str(), VS_PROJECT_SOLUTION_EXTENSION);
	printf("-- Generating %s%s...\n", g_mod.get_project_name().c_str(), VS_PROJECT_EXTENSION);

	if (!build_vcxproj(path))
		return be_fail_access;

	printf("-- %s%s OK\n", g_mod.get_project_name().c_str(), VS_PROJECT_EXTENSION);
	printf("-- Generating %s%s...\n", g_mod.get_project_name().c_str(), VS_PROJECT_USER_EXTENSION);
	
	if (!build_vcxproj_user(path))
		return be_fail_access;

	printf("-- %s%s OK\n", g_mod.get_project_name().c_str(), VS_PROJECT_USER_EXTENSION);

	return be_ok;
}

bool builder::build_solution_file(const std::string& path)
{
	std::string output{};
	std::string project_guid = g_utils.get_guid();

	output += "Microsoft Visual Studio Solution File, Format Version 12.00\n";
	output += "# Visual Studio 15\n";
	output += "Project(\"{" + g_utils.get_guid() + "}\") = \"" + g_mod.get_project_name() + "\", \"" + g_mod.get_project_name() + ".vcxproj\", \"{" + project_guid + "}\"\n";
	output += "EndProject\n";
	output += "Global\n";

	output += "	GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
	
	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			output += "		" + cfg + "|" + plat + " = " + cfg + "|" + plat + "\n";
		}
	}

	output += "	EndGlobalSection\n";

	output += "	GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";

	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			output += "		{" + project_guid + "}." + cfg + "|" + plat + ".ActiveCfg = " + cfg + "|" + plat + "\n";
			output += "		{" + project_guid + "}." + cfg + "|" + plat + ".Build.0 = " + cfg + "|" + plat + "\n";
		}
	}

	output += "	EndGlobalSection\n";

	output += "	GlobalSection(SolutionProperties) = preSolution\n";
	output += "		HideSolutionNode = FALSE\n";
	output += "	EndGlobalSection\n";

	output += "EndGlobal";
	output += "\n";

	return g_fs.create_object(path + VS_PROJECT_SOLUTION_EXTENSION, output);
}

bool builder::build_vcxproj(const std::string& path)
{
	std::string output{};

	set_base(output);
	{
		set_project_configuration(output);
		set_globals(output);

		fast_link(output, "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />");

		set_configurations(output);

		fast_link(output, "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />");
		fast_link_closed(output, "<ImportGroup Label=\"ExtensionSettings\">", "</ImportGroup>");

		set_import_configurations(output);

		fast_link(output, "<PropertyGroup Label=\"UserMacros\" />");

		set_property_groups(output);
		set_item_definition_groups(output);
		set_cpp_files(output);
		set_header_files(output);
		set_library_files(output);
		set_unknown_files(output);

		fast_link(output, "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />");
		fast_link_closed(output, "<ImportGroup Label=\"ExtensionTargets\">", "</ImportGroup>");
	}
	set_end_base(output);

	return g_fs.create_object(path + VS_PROJECT_EXTENSION, output);
}

bool builder::build_vcxproj_user(const std::string& path)
{
	std::string output{};

	set_base(output);
	{
		output += xml_string(2, ' ') + "<PropertyGroup>";
		output += "\n";
		{
			output += xml_string(4, ' ') + "<ShowAllFiles>";
			{
				output += "true";
			}
			output += "</ShowAllFiles>";
			output += "\n";
		}
		output += xml_string(2, ' ') + "</PropertyGroup>";
		output += "\n";
	}
	set_end_base(output);

	return g_fs.create_object(path + VS_PROJECT_USER_EXTENSION, output);
}

void builder::set_base(xml_string& string)
{
	string += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	string += "\n";

	string += "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">";
	string += "\n";
}

void builder::set_globals(xml_string& string)
{
	Json::Value config_type_obj = g_mod.get()["settings"]["configuration_type"];

	string += xml_string(2, ' ') + "<PropertyGroup Label=\"Globals\">";
	string += "\n";
	{
		string += xml_string(4, ' ') + "<ProjectGuid>" + std::string{ "{" + g_utils.get_guid() + "}" } + "</ProjectGuid>";
		string += "\n";

		string += xml_string(4, ' ') + "<IgnoreWarnCompileDuplicatedFilename>" + "true" + "</IgnoreWarnCompileDuplicatedFilename>";
		string += "\n";

		if (config_type_obj.asString() == "app")
		{
			string += xml_string(4, ' ') + "<Keyword>" + "Win32Proj" + "</Keyword>";
			string += "\n";
		}

		string += xml_string(4, ' ') + "<RootNamespace>" + g_mod.get_project_name() + "</RootNamespace>";
		string += "\n";

		string += xml_string(4, ' ') + "<LatestTargetPlatformVersion>";
		string += "$([Microsoft.Build.Utilities.ToolLocationHelper]::GetLatestSDKTargetPlatformVersion('Windows', '10.0'))";
		string += "</LatestTargetPlatformVersion>";
		string += "\n";

	}
	string += xml_string(2, ' ') + "</PropertyGroup>";
	string += "\n";
}

void builder::set_project_configuration(xml_string& string)
{
	string += xml_string(2, ' ') + "<ItemGroup Label=\"ProjectConfigurations\">";
	string += "\n";
	{
		for (const auto& platform : platform_obj)
		{
			for (const auto& config : cfg_obj)
			{
				std::string cfg  = config.asString();
				std::string plat = platform.asString();

				string += xml_string(4, ' ') + "<ProjectConfiguration Include=\"" + cfg + "|" + plat + "\">";
				string += "\n";

				string += xml_string(6, ' ') + "<Configuration>" + cfg + "</Configuration>";
				string += "\n";

				string += xml_string(6, ' ') + "<Platform>" + plat + "</Platform>";
				string += "\n";

				string += xml_string(4, ' ') + "</ProjectConfiguration>";
				string += "\n";
			}
		}
	}
	string += xml_string(2, ' ') + "</ItemGroup>";
	string += "\n";
}

void builder::set_configurations(xml_string& string)
{
	Json::Value config_type_obj      = g_mod.get()["settings"]["configuration_type"];
	Json::Value characterset_obj     = g_mod.get()["settings"]["characterset"];
	Json::Value platform_toolset_obj = g_mod.get()["settings"]["platform_toolset"];

	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			string += xml_string(2, ' ') + "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='" + cfg + "|" + plat + "'\" Label=\"Configuration\">";
			string += "\n";
			{
				string += xml_string(4, ' ') + "<ConfigurationType>";
				{
					if (config_type_obj.asString() == "app")
						string += "Application";

					else if (config_type_obj.asString() == "dynamic")
						string += "DynamicLibrary";

					else if (config_type_obj.asString() == "static")
						string += "StaticLibrary";

					else
						string += "Application";
				}
				string += "</ConfigurationType>";
				string += "\n";

				string += xml_string(4, ' ') + "<UseDebugLibraries>" + "false" + "</UseDebugLibraries>";
				string += "\n";

				string += xml_string(4, ' ') + "<CharacterSet>";
				{
					if (characterset_obj.asString() == "mbcs")
						string += "MultiByte";

					else if (characterset_obj.asString() == "uni")
						string += "Unicode";

					else
						string += "NotSet";
				}
				string += "</CharacterSet>";
				string += "\n";

				string += xml_string(4, ' ') + "<PlatformToolset>";
				{
					if (platform_toolset_obj.asString() == "v110")
						string += "v110";

					else if (platform_toolset_obj.asString() == "v140")
						string += "v140";

					else if (platform_toolset_obj.asString() == "v141")
						string += "v141";

					else if (platform_toolset_obj.asString() == "v142")
						string += "v142";

					else if (platform_toolset_obj.asString() == "v143")
						string += "v143";

					else if (platform_toolset_obj.asString() == "kerneldrv8.1")
						string += "WindowsKernelModeDriver8.1";

					else if (platform_toolset_obj.asString() == "userdrv8.1")
						string += "WindowsUserModeDriver8.1";

					else if (platform_toolset_obj.asString() == "appfordrv8.1")
						string += "WindowsApplicationForDrivers8.1";

					else if (platform_toolset_obj.asString() == "kerneldrv8.0")
						string += "WindowsKernelModeDriver8.0";

					else if (platform_toolset_obj.asString() == "userdrv8.0")
						string += "WindowsUserModeDriver8.0";

					else if (platform_toolset_obj.asString() == "appfordrv8.0")
						string += "WindowsApplicationForDrivers8.0";

					else
						string += "v120";
				}
				string += "</PlatformToolset>";
				string += "\n";
			}
			string += xml_string(2, ' ') + "</PropertyGroup>";
			string += "\n";
		}
	}
}

void builder::set_import_configurations(xml_string& string)
{
	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			string += xml_string(2, ' ') + "<ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='" + cfg + "|" + plat + "'\" Label=\"PropertySheets\">";
			string += "\n";
			{
				string += xml_string(4, ' ') + "<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />";
				string += "\n";
			}
			string += xml_string(2, ' ') + "</ImportGroup>";
			string += "\n";
		}
	}
}

void builder::set_property_groups(xml_string& string)
{
	Json::Value outdir_obj       = g_mod.get()["settings"]["out_dir"];
	Json::Value intdir_obj       = g_mod.get()["settings"]["int_dir"];
	Json::Value config_type_obj  = g_mod.get()["settings"]["configuration_type"];
	Json::Value include_dirs_obj = g_mod.get()["settings"]["include_dirs"];
	Json::Value library_dirs_obj = g_mod.get()["settings"]["lib_dirs"];

	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			string += xml_string(2, ' ') + "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='" + cfg + "|" + plat + "'\">";
			string += "\n";
			{
				string += xml_string(4, ' ') + "<LinkIncremental>" + "false" + "</LinkIncremental>";
				string += "\n";

				string += xml_string(4, ' ') + "<OutDir>";
				{
					if (outdir_obj.empty())
						string += "bin\\" + platform.asString() + "\\" + config.asString() + "\\";
					else
						string += outdir_obj.asString();
				}
				string += "</OutDir>";
				string += "\n";

				string += xml_string(4, ' ') + "<IntDir>";
				{
					if (intdir_obj.empty())
						string += "obj\\" + platform.asString() + "\\" + config.asString() + "\\";
					else
						string += intdir_obj.asString();
				}
				string += "</IntDir>";
				string += "\n";

				string += xml_string(4, ' ') + "<TargetName>" + g_mod.get_project_name() + "</TargetName>";
				string += "\n";

				string += xml_string(4, ' ') + "<TargetExt>";
				{
					if (config_type_obj.asString() == "app")
						string += ".exe";

					else if (config_type_obj.asString() == "dynamic")
						string += ".dll";

					else if (config_type_obj.asString() == "static")
						string += ".lib";

					else
						string += ".exe";
				}
				string += "</TargetExt>";
				string += "\n";

				string += xml_string(4, ' ') + "<IncludePath>";
				{
					if (!include_dirs_obj.empty())
					{
						for (const auto& dir : include_dirs_obj)
						{
							string += dir.asString() + ";";
						}
					}

					string += "$(IncludePath)";
				}
				string += "</IncludePath>";
				string += "\n";

				string += xml_string(4, ' ') + "<LibraryPath>";
				{
					if (!library_dirs_obj.empty())
					{
						for (const auto& dir : library_dirs_obj)
						{
							string += dir.asString() + ";";
						}
					}

					string += "$(LibraryPath)";
				}
				string += "</LibraryPath>";
				string += "\n";
			}
			string += xml_string(2, ' ') + "</PropertyGroup>";
			string += "\n";
		}
	}
}

void builder::set_item_definition_groups(xml_string& string)
{
	Json::Value pch_obj          = g_mod.get()["settings"]["pch"];
	Json::Value warning_obj      = g_mod.get()["settings"]["warning"];
	Json::Value optimization_obj = g_mod.get()["settings"]["optimization"];
	Json::Value fll_obj          = g_mod.get()["settings"]["function_level_linking"];
	Json::Value defines_obj      = g_mod.get()["settings"]["defines"];
	Json::Value subsys_obj       = g_mod.get()["settings"]["sub_system"];
	Json::Value links_obj        = g_mod.get()["settings"]["links"];
	Json::Value link_options_obj = g_mod.get()["settings"]["link_options"];

	for (const auto& platform : platform_obj)
	{
		for (const auto& config : cfg_obj)
		{
			std::string cfg  = config.asString();
			std::string plat = platform.asString();

			string += xml_string(2, ' ') + "<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='" + cfg + "|" + plat + "'\">";
			string += "\n";
			{
				string += xml_string(4, ' ') + "<ClCompile>";
				string += "\n";
				{
					string += xml_string(6, ' ') + "<PrecompiledHeader>";
					{
						if (pch_obj.asString() == "stdafx.h")
							string += "Use";

						else
							string += "NotUsing";
					}
					string += "</PrecompiledHeader>";
					string += "\n";

					if (!pch_obj.empty())
					{
						if (pch_obj.asString() == "stdafx.h")
						{
							string += xml_string(6, ' ') + "<PrecompiledHeaderFile>" + "stdafx.h" + "</PrecompiledHeaderFile>";
							string += "\n";
						}
					}

					string += xml_string(6, ' ') + "<WarningLevel>";
					{
						if (warning_obj.asString() == "default")
							string += "Level3";

						else if (warning_obj.asString() == "high")
							string += "Level4";

						else if (warning_obj.asString() == "all")
							string += "EnableAllWarnings";

						else
							string += "TurnOffAllWarnings";
					}
					string += "</WarningLevel>";
					string += "\n";

					string += xml_string(6, ' ') + "<Optimization>";
					{
						if (optimization_obj.asString() == "speed")
							string += "MaxSpeed";

						else if (optimization_obj.asString() == "size")
							string += "MinSpace";

						else if (optimization_obj.asString() == "full")
							string += "Full";

						else
							string += "Disabled";
					}
					string += "</Optimization>";
					string += "\n";

					string += xml_string(6, ' ') + "<FunctionLevelLinking>";
					{
						if (fll_obj.asString() == "on")
							string += "true";
						else
							string += "false";
					}
					string += "</FunctionLevelLinking>";
					string += "\n";

					string += xml_string(6, ' ') + "<IntrinsicFunctions>" + "true" + "</IntrinsicFunctions>";
					string += "\n";

					string += xml_string(6, ' ') + "<MinimalRebuild>" + "false" + "</MinimalRebuild>";
					string += "\n";

					string += xml_string(6, ' ') + "<LanguageStandard>" + "stdcpplatest" + "</LanguageStandard>";
					string += "\n";

					string += xml_string(6, ' ') + "<StringPooling>" + "true" + "</StringPooling>";
					string += "\n";

					string += xml_string(6, ' ') + "<SDLCheck>" + "true" + "</SDLCheck>";
					string += "\n";

					string += xml_string(6, ' ') + "<ConformanceMode>" + "true" + "</ConformanceMode>";
					string += "\n";

					string += xml_string(6, ' ') + "<PreprocessorDefinitions>";
					{
						if (!defines_obj.empty())
						{
							for (const auto& dir : defines_obj)
							{
								string += dir.asString() + ";";
							}
						}

						string += "%(PreprocessorDefinitions)";
					}
					string += "</PreprocessorDefinitions>";
					string += "\n";

					string += xml_string(6, ' ') + "<AdditionalOptions>" + "/std:c++latest %(AdditionalOptions)" + "</AdditionalOptions>";
					string += "\n";
				}
				string += xml_string(4, ' ') + "</ClCompile>";
				string += "\n";

				string += xml_string(4, ' ') + "<Link>";
				string += "\n";
				{
					string += xml_string(6, ' ') + "<SubSystem>";
					{
						if (subsys_obj.asString() == "console")
							string += "Console";

						else if (subsys_obj.asString() == "windowed" || subsys_obj.asString() == "lib" ||
							subsys_obj.asString() == "static_lib")
							string += "Windows";

						else
							string += "Console";
					}
					string += "</SubSystem>";
					string += "\n";

					string += xml_string(6, ' ') + "<EnableCOMDATFolding>" + "true" + "</EnableCOMDATFolding>";
					string += "\n";

					if (!optimization_obj.empty())
					{
						if (optimization_obj.asString() == "speed" ||
							optimization_obj.asString() == "size" ||
							optimization_obj.asString() == "full")
						{
							string += xml_string(6, ' ') + "<OptimizeReferences>" + "true" + "</OptimizeReferences>";
							string += "\n";
						}
					}

					if (!links_obj.empty())
					{
						string += xml_string(6, ' ') + "<AdditionalDependencies>";
						{
							for (const auto& dir : links_obj)
							{
								string += dir.asString() + ";";
							}
							string += "%(AdditionalDependencies)";
						}
						string += "</AdditionalDependencies>";
						string += "\n";
					}

					if (!link_options_obj.empty())
					{
						string += xml_string(6, ' ') + "<AdditionalOptions>" + link_options_obj.asString().append(" %(AdditionalOptions)") + "</AdditionalOptions>";
						string += "\n";
					}
				}
				string += xml_string(4, ' ') + "</Link>";
				string += "\n";
			}
			string += xml_string(2, ' ') + "</ItemDefinitionGroup>";
			string += "\n";
		}
	}
}

void builder::set_cpp_files(xml_string& string)
{
	Json::Value files_obj = g_mod.get()["settings"]["files"];

	bool is_has = false;

	for (const auto& f : files_obj)
		if (is_cpp(f))
			is_has = true;

	if (!is_has)
		return;

	string += xml_string(2, ' ') + "<ItemGroup>";
	string += "\n";
	{
		for (const auto& f : files_obj)
		{
			if (is_cpp(f))
			{
				string += xml_string(4, ' ') + "<ClCompile Include=\"" + f.asString() + "\" />";
				string += "\n";
			}
		}
	}
	string += xml_string(2, ' ') + "</ItemGroup>";
	string += "\n";
}

void builder::set_header_files(xml_string& string)
{
	Json::Value files_obj = g_mod.get()["settings"]["files"];

	bool is_has = false;

	for (const auto& f : files_obj)
		if (is_hpp(f))
			is_has = true;

	if (!is_has)
		return;

	string += xml_string(2, ' ') + "<ItemGroup>";
	string += "\n";
	{
		for (const auto& f : files_obj)
		{
			if (is_hpp(f))
			{
				string += xml_string(4, ' ') + "<ClCompile Include=\"" + f.asString() + "\" />";
				string += "\n";
			}
		}
	}
	string += xml_string(2, ' ') + "</ItemGroup>";
	string += "\n";
}

void builder::set_library_files(xml_string& string)
{
	Json::Value files_obj = g_mod.get()["settings"]["files"];

	bool is_has = false;

	for (const auto& f : files_obj)
		if (is_lib(f))
			is_has = true;

	if (!is_has)
		return;

	string += xml_string(2, ' ') + "<ItemGroup>";
	string += "\n";
	{
		for (const auto& f : files_obj)
		{
			if (is_lib(f))
			{
				string += xml_string(4, ' ') + "<Library Include=\"" + f.asString() + "\" />";
				string += "\n";
			}
		}
	}
	string += xml_string(2, ' ') + "</ItemGroup>";
	string += "\n";
}

void builder::set_unknown_files(xml_string& string)
{
	Json::Value files_obj = g_mod.get()["settings"]["files"];

	bool is_has = false;

	for (const auto& f : files_obj)
		if (!is_cpp(f) && !is_hpp(f) && !is_lib(f))
			is_has = true;

	if (!is_has)
		return;

	string += xml_string(2, ' ') + "<ItemGroup>";
	string += "\n";
	{
		for (const auto& f : files_obj)
		{
			if (!is_cpp(f) && !is_hpp(f) && !is_lib(f))
			{
				string += xml_string(4, ' ') + "<None Include=\"" + f.asString() + "\" />";
				string += "\n";
			}
		}
	}
	string += xml_string(2, ' ') + "</ItemGroup>";
	string += "\n";
}

void builder::set_end_base(xml_string& string)
{
	string += "</Project>";
}

static bool is_cpp(const Json::Value& object)
{
	return
		g_utils.get_file_extension(object.asString()) == "c" ||
		g_utils.get_file_extension(object.asString()) == "cc" ||
		g_utils.get_file_extension(object.asString()) == "cpp";
}

static bool is_hpp(const Json::Value& object)
{
	return
		g_utils.get_file_extension(object.asString()) == "h" ||
		g_utils.get_file_extension(object.asString()) == "hh" ||
		g_utils.get_file_extension(object.asString()) == "hpp";
}

static bool is_lib(const Json::Value& object)
{
	return g_utils.get_file_extension(object.asString()) == "lib";
}

static void fast_link(xml_string& string, const xml_string& param)
{
	string += xml_string(2, ' ') + param;
	string += "\n";
}

static void fast_link_closed(xml_string& string, const xml_string& param, const xml_string& param1)
{
	string += xml_string(2, ' ') + param;
	string += "\n";

	string += xml_string(2, ' ') + param1;
	string += "\n";
}