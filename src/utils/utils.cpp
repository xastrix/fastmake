#include "utils.h"

#include <windows.h>
#include <cctype>
#include <algorithm>

std::string utils::replace_patterns(const std::string& input,
	const std::vector<std::string>& a_list,
	const std::vector<std::string>& b_list) {
	std::string result = input;

	for (int i = 0; i < a_list.size(); ++i) {
		std::string::size_type pos = 0;
		while ((pos = result.find(a_list[i], pos)) != std::string::npos) {
			result.replace(pos, a_list[i].length(), b_list[i]);
			pos += b_list[i].length();
		}
	}

	return result;
}

std::string utils::remove_char(std::string string, const std::string& chars)
{
	string.erase(std::remove_if(string.begin(), string.end(), [&chars](char c) {
		return chars.find(c) != std::string::npos;
	}), string.end());

	return string;
}

std::string utils::get_guid()
{
	GUID guid = {};
	RPC_CSTR guid_string = {};

	UuidCreate(&guid);
	UuidToStringA(&guid, &guid_string);

	std::string res = reinterpret_cast<const char*>(guid_string);

	RpcStringFreeA(&guid_string);

	for (char& c : res)
		c = std::toupper(static_cast<unsigned char>(c));

	return res;
}

std::string utils::get_file_extension(const std::string& filename)
{
	std::string::size_type dot_pos = filename.find_last_of('.');

	if (dot_pos != std::string::npos)
		return filename.substr(dot_pos + 1);

	return "";
}