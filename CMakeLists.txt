cmake_minimum_required(VERSION 3.29)

project(AudioVis C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCE_FILES "Visualizer/src/*.cpp")

add_subdirectory(Visualizer/vendor/BASS)
add_subdirectory(Visualizer/vendor/Engine)
add_subdirectory(Visualizer/vendor/ordered_map)
add_subdirectory(Visualizer/vendor/tiny-file-dialogs)
add_subdirectory(Visualizer/vendor/yaml-cpp)

add_executable(Vis "${SOURCE_FILES}")
target_link_libraries(Vis PUBLIC BASS Engine ordered_map tiny-file-dialogs yaml-cpp)
target_include_directories(Vis PUBLIC Visualizer/src)
target_compile_features(Vis PRIVATE cxx_std_17)

add_custom_command(TARGET Vis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Vis> $<TARGET_RUNTIME_DLLS:Vis>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Visualizer/res $<TARGET_FILE_DIR:Vis>/res
    COMMAND_EXPAND_LISTS
)


