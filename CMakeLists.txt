cmake_minimum_required(VERSION 3.25)
project(docker-stress-test VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)

file(GLOB_RECURSE DOCKER_STRESS_TEST_SOURCES "src/*.cpp" "src/*.hpp")
add_executable(docker-stress-test ${DOCKER_STRESS_TEST_SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_options(docker-stress-test PRIVATE "-O3" "-ffast-math" "-march=haswell")
endif ()

add_custom_command(TARGET docker-stress-test POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(docker-stress-test PRIVATE Crow::Crow)

install(TARGETS docker-stress-test RUNTIME DESTINATION bin)
install(DIRECTORY resources/ DESTINATION bin)