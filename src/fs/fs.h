#pragma once

#include <filesystem>
#include <string>

#define FS_FORBIDDEN_CHARS "<>:\"/\\|?*"

struct fs {
	/**
	 * Create a object (file)
	 * @param path where the file will be stored
	 * @param data of content in file
	 * @return true/false
	*/
	bool create_object(const std::string& path, const std::string& data);

	/**
	 * Check if file is exists
	 * @param path
	 * @return true/false
	*/
	bool exists(const std::string& path);

	/**
	 * Get content from file
	 * @param path
	 * @return string
	*/
	std::string get_file_contents(const std::string& path);

	/**
	 * Finds files in a directory that match a given keyword
	 * @param the name of the directory to search in
	 * @param the keyword to search for in the filenames
	 * @param a reference to a string that will hold the path of the found file matching the keyword
	 * @return true/false
	*/
	bool find_files_in_directories(const std::string& dirname, const std::string& keyword, std::string& path);
};

extern fs g_fs;