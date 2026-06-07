cmake_minimum_required(VERSION 3.10)

project(ServerScacchi)

# SFML 3 richiede C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. CERCA SFML 3 (Nota le maiuscole: Graphics, Window, System, Network)
find_package(SFML 3 COMPONENTS Graphics Window System Network REQUIRED)

# 3. ESEGUIBILE DEL RELAY SERVER (Il Server)
# Assicurati di aver creato il file relay_server.cpp
add_executable(RelayServer relay_server.cpp)
target_link_libraries(RelayServer SFML::Network SFML::System)