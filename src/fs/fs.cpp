#include "fs.h"

#include <windows.h>

bool FS::create_object(const std::string& path, const std::string& data)
{
	std::filesystem::path file_path = path;
	std::filesystem::path dir_path = file_path.parent_path();

	if (!std::filesystem::is_directory(dir_path))
		std::filesystem::create_directories(dir_path);

	HANDLE h = CreateFileA(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (h == INVALID_HANDLE_VALUE)
		return false;

	DWORD b_written;
	if (!WriteFile(h, data.c_str(), static_cast<DWORD>(data.size()), &b_written, 0)) {
		CloseHandle(h);
		return false;
	}

	CloseHandle(h);

	return true;
}

std::string FS::get_file_contents(const std::string& path)
{
	std::string buf;

	HANDLE h = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (h == INVALID_HANDLE_VALUE)
		return buf;

	LARGE_INTEGER fsize;
	if (!GetFileSizeEx(h, &fsize)) {
		CloseHandle(h);
		return buf;
	}

	std::vector<char> bytes(fsize.QuadPart);

	DWORD read;
	if (!ReadFile(h, bytes.data(), static_cast<DWORD>(fsize.QuadPart), &read, 0)) {
		CloseHandle(h);
		return buf;
	}

	buf.assign(bytes.data(), bytes.size());

	CloseHandle(h);

	return buf;
}

bool FS::find_files_in_directories(const std::string& dirname, const std::string& keyword, std::string& path)
{
	bool ret = false;

	if (!std::filesystem::is_directory(dirname))
		return ret;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(dirname))
	{
		if (entry.path().filename().string().find(keyword) != std::string::npos)
		{
			path = entry.path().string();
			ret = true;
			break;
		}
	}

	return ret;
}