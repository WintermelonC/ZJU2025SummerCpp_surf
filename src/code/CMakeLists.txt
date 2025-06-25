add_executable(example sfml_example.cpp)

target_link_libraries(example PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

# 复制 assets 文件夹到构建目录
# assets 文件夹与 code 文件夹同级
add_custom_command(
    TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets
    ${CMAKE_BINARY_DIR}/src/assets
)
