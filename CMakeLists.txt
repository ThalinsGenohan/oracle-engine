cmake_minimum_required(VERSION 3.23)
project(oracles VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
	${SRCROOT}/glad.c
	${INCROOT}/stb_image.h
	${SRCROOT}/stb_image.cpp

	${INCROOT}/AssetManager.h
	${INCROOT}/Game.h
	${INCROOT}/GameObject.h
	${INCROOT}/Input.h
	${INCROOT}/Tilemap.h
	${INCROOT}/Tileset.h
	${INCROOT}/types.h
	${INCROOT}/Window.h

	${SRCROOT}/AssetManager.cpp
	${SRCROOT}/Game.cpp
	${SRCROOT}/GameObject.cpp
	${SRCROOT}/Input.cpp
	${SRCROOT}/main.cpp
	${SRCROOT}/Tilemap.cpp
	${SRCROOT}/Tileset.cpp
	${SRCROOT}/Window.cpp
	
	${INCROOT}/Graphics/Camera.h
	${INCROOT}/Graphics/FrameBuffer.h
	${INCROOT}/Graphics/Renderer.h
	
	${INCROOT}/Graphics/Texture2D.h

	${SRCROOT}/Graphics/Camera.cpp
	${SRCROOT}/Graphics/FrameBuffer.cpp
	${SRCROOT}/Graphics/Renderer.cpp
	
	${SRCROOT}/Graphics/Texture2D.cpp
)
source_group("" FILES ${SRC})

add_executable(oracles ${SRC})

target_include_directories(oracles PRIVATE ${INCROOT})

#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/assets/*)
#set_directory_properties(PROPERTIES CMAKE_CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/assets)
#file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_BINARY_DIR})

#add_custom_command(
#        TARGET oracles PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${CMAKE_SOURCE_DIR}/assets/
#                ${CMAKE_CURRENT_BINARY_DIR}/assets/
#)

set(ASSETS_SOURCE "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST "${CMAKE_CURRENT_BINARY_DIR}/assets")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ASSETS_SOURCE} ${ASSETS_DEST} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${ASSETS_SOURCE} => ${ASSETS_DEST}"
)

#set_property(
#        TARGET oracles
#        APPEND
#        PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/assets/
#)

add_subdirectory(lib/OtterML)
target_link_libraries(oracles OtterML::System OtterML::Graphics)
target_include_directories(oracles PUBLIC "lib/OtterML/include")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3)
find_package(glm)
find_package(spdlog)
find_package(yaml-cpp)

target_link_libraries(oracles glfw glm::glm spdlog::spdlog yaml-cpp)
