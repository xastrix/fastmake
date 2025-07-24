#include "client.h"

int main(int argc, const char** argv)
{
	setlocale(LC_ALL, "");
	
	if (argc < 2) {
		printf("usage: fastmake <path>\n");
		printf("to get more information read's the docs\n");

		char* files[MAX_FILES];
		int   file_num = 0;

		fs::get_directory_files(".", files, &file_num, fmRecursive);

		for (int i = 0; i < file_num; i++)
		{
			if (fs::exists(files[i]).as(objectExists))
			{
				if (strstr(files[i], SOURCE_PROJECT_CONFIGURATION_FILENAME) != 0)
				{
					std::string file_path = std::filesystem::path{ files[i] }.parent_path().string();
					std::string path, target_path = file_path.empty() ? "." : file_path;

					if (fs::find_files_in_directories(target_path, SOURCE_PROJECT_CONFIGURATION_FILENAME, path))
					{
						if (g_mod.initialize(target_path))
						{
							printf("\n");
							printf("A project '%s' found!\n", g_mod.get_project_name().c_str());
							printf("type \"fastmake %s\" to build project\n", std::filesystem::path{ path }.parent_path().string().c_str());
						}
					}
				}
			}
			else if (fs::exists(files[i]).as(dirExists))
			{
				int new_num = 0;
				fs::get_directory_files(files[i], files, &new_num, fmRecursive);
				file_num += new_num;
			}
		}

		return 1;
	}

	if (!g_mod.initialize(argv[1])) {
		printf("%s\n", g_mod.get_err().c_str());
		return 1;
	}

	auto start = std::chrono::high_resolution_clock::now();

	switch (g_builder.run()) {
	case be_fail_access: {
		printf("Error: Failed to create Visual Studio project files (Don't have permissions)\n");
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
	case be_files_empty: {
		printf("Error: Files are empty\n");
		break;
	}
	case be_ok: {
		printf("-- Done (%.0f ms)\n", std::chrono::duration<double, std::milli> {
			std::chrono::high_resolution_clock::now() - start }.count());
	}
	}

	return 0;
}