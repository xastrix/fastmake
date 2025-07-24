#pragma once

#include <string>
#include <vector>

namespace utils
{
	/**
	 * Replaces words in a_list with corresponding words from b_list by index
	 * @param the input string where replacements will occur
	 * @param the list of words to be replaced
	 * @param the list of words to replace with
	 * @return string
	*/
	std::string replace_patterns(const std::string& input,
		const std::vector<std::string>& a_list,
		const std::vector<std::string>& b_list);

	/**
	 * Remove character from string
	 * @param string
	 * @param character
	 * @return string
	*/
	std::string remove_char(std::string string, const std::string& chars);

	/**
	 * Generate guid using WINAPI
	 * @return string
	*/
	std::string get_guid();

	/**
	 * File extension
	 * @param filename
	 * @return string
	*/
	std::string get_file_extension(const std::string& filename);
}