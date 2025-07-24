#pragma once

#include <json/json.h>
#include <string>

#define SOURCE_PROJECT_CONFIGURATION_FILENAME "fastmake-file"
#define UNKNOWN_PROJECT_PATTERN_STRING        "UnnamedProject"

struct mod {
	/**
	 * Initializes a module
	 * @param path to the directory where the JSON module is located
	 * @return true/false
	*/
	bool initialize(const std::string& path);

	/**
	 * Gets the project name from the JSON module file
	 * @return string
	*/
	std::string get_project_name();

	/**
	 * Gets the directory where the JSON module file is located
	 * @return string
	*/
	std::string get_project_path();

	/**
	 * Returns an error message if mod::initialize method returned false
	 * @return string
	*/
	std::string get_err();

	/**
	 * Gets the JSON object with settings from the module
	 * @return json object
	*/
	Json::Value get();
private:
	std::string  source_path;
	std::string  err_msg;
	Json::Value  json_module_data;
	Json::Reader json_module_reader;
};

extern mod g_mod;