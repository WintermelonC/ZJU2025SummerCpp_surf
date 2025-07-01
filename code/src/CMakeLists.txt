add_executable(surf
    surf.cpp
    # 基础设施层文件
    infrastructure/eventBus.cpp
    infrastructure/assetManager.cpp
    infrastructure/utils.cpp
    infrastructure/animationController.cpp
    # ViewModel层文件
    viewModels/inputManager.cpp
    viewModels/gameViewModel.cpp
    # View层文件
    views/renderSystem.cpp
    views/spriteManager.cpp
    views/gameView.cpp
    # Model层文件
    models/entityModel.cpp
    models/gameModel.cpp
    models/playerModel.cpp
    # Service层文件
    services/playerService.cpp
    # 应用协调器
    mvvmGame.cpp
)

target_link_libraries(surf PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)
target_include_directories(surf PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
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
