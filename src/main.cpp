#include "builder/builder.h"

int main(int argc, const char** argv)
{
	if (argc < 2) {
		printf("usage: fastmake <path>\n");
		printf("                 path - Directory with %s\n", SOURCE_PROJECT_CONFIGURATION_FILENAME);
		printf("to get more information read's the docs\n");
		printf("docs: https://github.com/xastrix/fastmake/tree/master/docs\n");
		return 1;
	}

	if (!g_mod.initialize(argv[1])) {
		printf("%s\n", g_mod.get_err().c_str());
		return 1;
	}

	auto start = std::chrono::high_resolution_clock::now();

	switch (g_builder.run(g_mod.get_project_path())) {
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
	case be_ok: {
		std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;

		printf("\n");
		printf("-- Done (%.0f ms)\n", duration.count());
	}
	}

	return 0;
}