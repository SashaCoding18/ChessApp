cmake_minimum_required(VERSION 3.10)
project(SCACCHI)

# Forza il percorso verso le librerie di MSYS2
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

set(CMAKE_CXX_STANDARD 23)

# In SFML 3 i componenti DEVONO avere la prima lettera maiuscola
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Network REQUIRED)

add_executable(SfmlApp main.cpp)
add_custom_command(
    TARGET SfmlApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Images"
            "$<TARGET_FILE_DIR:SfmlApp>/Images"
)

# Collega i target corretti di SFML 3
target_link_libraries(SfmlApp SFML::Graphics SFML::Window SFML::System SFML::Network)