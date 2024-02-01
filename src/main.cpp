#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <expected>

enum class error_type
{
	file_not_found,
	file_not_open,
	project_alredy_exist,
};

enum class success_type
{
	file_created,
	file_opened,
};

const std::string_view c_make_lists_content = R"(
cmake_minimum_required(VERSION 3.5.0)

project(CCppGeneratorProject VERSION 0.0.1)

#set compiler
set(CMAKE_CXX_COMPILER g++)

#set cpp version
set(CMAKE_CXX_STANDARD 23)


set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-value -Wno-unused-result -Wno-unused-label -Wno-unused-local-typedefs -Wno-unused-macros -Wno-unused-const-variable -Wno-unused-const-variable -Wno-unused-const-parameter -Wno-unused-const-variable -Wno-unused-cons ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")



macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND dirlist ${child})
      endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
foreach(subdir ${SUBDIRS})
    message(STATUS "subdir: ${subdir}")
    file(GLOB SRC_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/src/${subdir}/*.cpp)
endforeach()


include_directories(./includes)
add_executable(${PROJECT_NAME} ${SRC_FILES})



message(STATUS "Compiling ${PROJECT_NAME}")


)";

const std::string_view c_main_content = R"(
	#include <iostream>

	int main(int argc, char* argv[])
	{
		std::cout << "Hello, World" << std::endl;
	}
)";

const std::string_view c_cmake_lists_path = R"(CMakeLists.txt)";
const std::string_view c_main_path = R"(src/main.cpp)";
const std::vector dirs = {"src", "build", "includes"};

class File
{
public:
	File(const std::filesystem::path &path, const std::string &project_name)
		: m_path(path)
	{
		std::cout << "File: " << m_path << std::endl;
	}

private:
	std::filesystem::path m_path;

public:
	constexpr std::expected<success_type, error_type> genereate_project(const std::string &project_name) const
	{
		// parent dir is current dir + project name
		std::filesystem::path parent_dir = m_path;
		parent_dir /= project_name;

		// check if project alredy exist
		if (std::filesystem::exists(parent_dir))
			return std::unexpected(error_type::project_alredy_exist);
		// create parent dir
		std::filesystem::create_directory(parent_dir);

		// create dirs
		for (auto const &dir : dirs)
		{
			std::filesystem::path path = parent_dir;
			path /= dir;
			std::filesystem::create_directory(path);
		}

		std::string c_make_lists_content_local = c_make_lists_content.data();
		std::string_view world_to_replace = "CCppGeneratorProject";
		std::string_view world_to_replace_with = project_name;
		// replace world
		while (true)
		{
			auto pos = c_make_lists_content_local.find(world_to_replace);
			if (pos == std::string::npos)
				break;
			c_make_lists_content_local.replace(pos, world_to_replace.size(), world_to_replace_with);
		}

		// files path
		std::filesystem::path cmake_lists_path = parent_dir;
		cmake_lists_path /= c_cmake_lists_path.data();

		std::filesystem::path main_path = parent_dir;
		main_path /= c_main_path.data();

		// create files
		std::fstream cmake_file(cmake_lists_path, std::ios::out);
		std::fstream main_file(main_path, std::ios::out);

		if (!cmake_file.is_open() || !main_file.is_open())
			return std::unexpected(error_type::file_not_open);

		cmake_file << c_make_lists_content_local.data();
		main_file << c_main_content.data();

		// close
		cmake_file.close();
		main_file.close();

		return success_type::file_created;
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter project name" << std::endl;
		return 0;
	}

	std::string project_name = argv[1];
	File file(std::filesystem::current_path(), project_name);
	auto result = file.genereate_project(project_name);

	if (result)
		std::cout << "Project created" << std::endl;
	else
	{
		switch (result.error())
		{
		case error_type::file_not_found:
			std::cout << "File not found" << std::endl;
			break;
		case error_type::file_not_open:
			std::cout << "File not open" << std::endl;
			break;
		case error_type::project_alredy_exist:
			std::cout << "Project alredy exist with name: " << project_name << std::endl;
			break;
		default:
			std::cout << "Project not created" << std::endl;
			break;
		}
	}
	return 0;
}