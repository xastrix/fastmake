#include "mod.h"

#include "../utils/utils.h"
#include "../fs/fs.h"

mod g_mod;

bool mod::initialize(const std::string& path)
{
	if (!fs::find_files_in_directories(path, SOURCE_PROJECT_CONFIGURATION_FILENAME, source_path)) {
		err_msg = "Error: The settings file (" SOURCE_PROJECT_CONFIGURATION_FILENAME ") was not found in the directory '" + path + "'";
		return false;
	}

	std::string data{ fs::get_file_contents(source_path) };

	data = utils::replace_patterns(data,
		{ "{$name}" }, { g_mod.get_project_name() }
	);

	if (!json_module_reader.parse(data, json_module_data)) {
		err_msg = "Error: Failed to parse the settings file (" SOURCE_PROJECT_CONFIGURATION_FILENAME ")";
		return false;
	}

	return true;
}

std::string mod::get_project_name()
{
	return utils::remove_char(json_module_data["name"].asString(), FS_FORBIDDEN_CHARS);
}

std::string mod::get_project_path()
{
	return std::filesystem::path{ source_path }.parent_path().string() + "\\" + get_project_name();
}

std::string mod::get_err()
{
	return err_msg;
}

Json::Value mod::get()
{
	if (!json_module_data.empty())
		return json_module_data;

	return Json::Value{ Json::objectValue };
}