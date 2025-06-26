add_executable(sfml_example sfml_example.cpp)

target_link_libraries(sfml_example PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

# 复制 assets 文件夹到构建目录
# assets 文件夹与 src 文件夹同级
add_custom_command(
    TARGET sfml_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
