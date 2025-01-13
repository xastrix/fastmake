#include "fs.h"

#include <windows.h>

fs_exists fs::exists(const std::string& path)
{
	struct stat      s;
	struct fs_exists o;

	o._status = noneExists;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR) {
			o._status = dirExists;
		}
		else if (s.st_mode & S_IFREG) {
			o._status = objectExists;
		}
	}

	return o;
}

bool fs::create_object(const std::string& path, const std::string& data)
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

std::string fs::get_file_contents(const std::string& path)
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

bool fs::find_files_in_directories(const std::string& dirname, const std::string& keyword, std::string& path)
{
	bool ret = false;

	if (!std::filesystem::is_directory(dirname))
		return ret;

	for (const auto& entry : std::filesystem::directory_iterator(dirname))
	{
		if (entry.is_regular_file())
		{
			if (entry.path().filename().string() == keyword)
			{
				path = entry.path().string();
				ret = true;
				break;
			}
		}
	}

	return ret;
}

void fs::get_directory_files(const std::string& dirname, char** files, int* num, find_mode mode)
{
	WIN32_FIND_DATA data;
	HANDLE h;

	char path[MAX_PATH];

	if (dirname == ".")
		sprintf(path, "*.*");
	else
		sprintf(path, "%s\\*.*", dirname.c_str());

	h = FindFirstFileA(path, &data);

	do {
		if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0)
			continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && mode == fmFiles)
			continue;

		char path[MAX_PATH];

		if (dirname == ".")
			sprintf(path, "%s", data.cFileName);
		else
			sprintf(path, "%s\\%s", dirname.c_str(), data.cFileName);

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if (mode == fmRecursive) {
				get_directory_files(path, files, num, fmRecursive);
			}
			continue;
		}

		if (*num < MAX_FILES) {
			files[*num] = new char[strlen(path) + 1];
			if (files[*num] != NULL) {
				strcpy_s(files[*num], strlen(path) + 1, path);
				(*num)++;
			}
		}

	} while (FindNextFileA(h, &data) != 0);

	FindClose(h);
}