#pragma once

#include "../mod/mod.h"
#include "../fs/fs.h"
#include "../utils/utils.h"

#define VS_PROJECT_SOLUTION_EXTENSION                  ".sln"
#define VS_PROJECT_EXTENSION                           ".vcxproj"
#define VS_PROJECT_USER_EXTENSION VS_PROJECT_EXTENSION ".user"

using xml_string = std::string;

enum b_err {
	be_ok,
	be_fail_access,
	be_platforms_empty,
	be_configurations_empty,
	be_files_empty,
};

struct builder {
	/**
	 * Build visual studio project
	 * @return index of b_err
	*/
	b_err run();
private:
	bool build_solution_file(const std::string& path);
	bool build_vcxproj(const std::string& path);
	bool build_vcxproj_user(const std::string& path);
private:
	void set_base(xml_string& string);
	void set_globals(xml_string& string);
	void set_project_configuration(xml_string& string);
	void set_configurations(xml_string& string);
	void set_import_configurations(xml_string& string);
	void set_property_groups(xml_string& string);
	void set_item_definition_groups(xml_string& string);
	void set_source_files(xml_string& string);
	void set_header_files(xml_string& string);
	void set_library_files(xml_string& string);
	void set_other_files(xml_string& string);
	void set_end_base(xml_string& string);
private:
	Json::Value cfg_obj;
	Json::Value platform_obj;
	Json::Value files_obj;
	std::string project_guid;
	std::string project_name;
	std::string project_path;
};

extern builder g_builder;