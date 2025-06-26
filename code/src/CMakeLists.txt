add_executable(surf
    surf.cpp
    game.cpp
)

target_link_libraries(surf PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)
