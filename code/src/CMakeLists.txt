add_executable(surf
    surf.cpp
    game.cpp
    player.cpp
)

target_link_libraries(surf PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

# 复制 assets 文件夹到构建目录
# assets 文件夹与 src 文件夹同级
add_custom_command(
    TARGET surf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
