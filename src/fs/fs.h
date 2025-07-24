#pragma once

#include <filesystem>
#include <string>

#define MAX_FILES          1024
#define FS_FORBIDDEN_CHARS "<>:\"/\\|?*"

typedef enum {
	fmFiles,
	fmRecursive,
} find_mode;

enum fs_exist_stat {
	noneExists,
	objectExists,
	dirExists,
};

struct fs_exists {
	fs_exist_stat _status;
	bool as(const fs_exist_stat status) {
		return _status == status;
	}
};

namespace fs
{
	/**
	 * Checking for the existence of an object (file/directory) on the specified path
	 * @param path to the object
	 * @return fs_exists structure with as() method
	*/
	fs_exists exists(const std::string& path);

	/**
	 * Create a object (file)
	 * @param path where the file will be stored
	 * @param data of content in file
	 * @return true/false
	*/
	bool create_object(const std::string& path, const std::string& data);

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

	/**
	 * Getting the list of files in the specified directory
	 * @param dirname
	 * @param allocated array for the list
	 * @param number of files found in the directory
	 * @param file search type (fmFiles - search for files only, fmRecursive - search for files and folders)
	*/
	void get_directory_files(const std::string& dirname, char** files, int* num, find_mode mode);
}