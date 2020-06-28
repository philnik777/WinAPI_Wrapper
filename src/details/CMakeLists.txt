project(__winapi_src_details)

set(SUBPROJECTS ${SUBPROJECTS} __winapi_src_details PARENT_SCOPE)

add_library(__winapi_src_details
	Common.cpp
	CreateWindow.cpp
	ID_Allocator.cpp
	Library.cpp
	RegisterWindow.cpp
)

target_include_directories(__winapi_src_details PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/WinAPI/details/)
