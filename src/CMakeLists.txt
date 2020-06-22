cmake_minimum_required(VERSION 3.10)

project(__winapi_src)
set(SUBPROJECTS ${SUBPROJECTS} __winapi_src)
set(SUBINCLUDES ${SUBINCLUDES} src/details)

add_library(__winapi_src
	Button.cpp
	ListView.cpp
	Menu.cpp
	MenuBar.cpp
	TabControl.cpp
	Window.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
message("Source: " + ${CMAKE_CURRENT_SOURCE_DIR})
