add_executable(surf
    surf.cpp
    app/game.cpp
    models/playerModel.cpp
    viewModels/gameViewModel.cpp
    viewModels/spriteViewModel.cpp
    viewModels/textureViewModel.cpp
    viewModels/animationViewModel.cpp
    viewModels/ObstacleItemViewModel.cpp
    viewModels/fontViewModel.cpp
    viewModels/playerViewModel.cpp
    viewModels/commands/gameCommands.cpp
    viewModels/commands/playerCommands.cpp
    viewModels/commands/obstacleCommands.cpp
    viewModels/commands/spriteCommands.cpp
    viewModels/commands/animationCommands.cpp
    views/gameView.cpp
)

target_link_libraries(surf PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(surf PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(surf PRIVATE RELEASE)
else()
    target_compile_definitions(surf PRIVATE DEBUG)
endif()

# 复制 assets 文件夹到构建目录
# assets 文件夹与 src 文件夹同级
add_custom_command(
    TARGET surf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
