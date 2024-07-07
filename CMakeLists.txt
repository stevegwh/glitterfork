cmake_minimum_required(VERSION 3.0)
project(Glitter)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Glitter/Vendor/bullet)

add_subdirectory(Glitter/Vendor/entt)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include(FetchContent)
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.0.0"
        SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS}
                    ${JoltPhysics_SOURCE_DIR}/..
                    Glitter/Vendor/assimp/include/
                    Glitter/Vendor/bullet/src/
                    Glitter/Vendor/glad/include/
                    Glitter/Vendor/glm/
                    Glitter/Vendor/stb/
                    Glitter/Vendor/entt
                    ${SDL_INCLUDE_DIR})

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Glitter/src/*.hpp)
file(GLOB PROJECT_SOURCES Glitter/src/*.cpp)
file(GLOB PROJECT_SHADERS Glitter/Shaders/*.comp
                          Glitter/Shaders/*.frag
                          Glitter/Shaders/*.geom
                          Glitter/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})


add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} 
        assimp
        Jolt
        ${Boost_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${GLAD_LIBRARIES} 
        BulletDynamics 
        BulletCollision 
        LinearMath
        EnTT::EnTT)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Glitter/Shaders ${CMAKE_CURRENT_BINARY_DIR}/Glitter/Shaders
        DEPENDS ${PROJECT_SHADERS}
        COMMENT "symbolic link shader folder from source to build"
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/Glitter/resources
        DEPENDS ${PROJECT_SHADERS}
        COMMENT "symbolic link resources folder from source to build"
)
