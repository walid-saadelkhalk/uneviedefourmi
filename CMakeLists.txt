cmake_minimum_required(VERSION 3.10)

# Définition du projet
project(AntColonySimulator)

# Utilisation du standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Détection de macOS
if(APPLE)
    message(STATUS "Compilation pour macOS")
    set(CMAKE_MACOSX_RPATH 1)  # Active RPATH sur macOS
endif()

# Ajout des fichiers sources
add_executable(AntColonySimulator 
    src/main.cpp 
    src/Features/Ants.cpp
    src/Features/AntsDijkstra.cpp  # Ajout de AntsDijkstra.cpp
)

# Ajout des répertoires d'en-têtes
target_include_directories(AntColonySimulator PRIVATE include)

# Activation des warnings utiles
target_compile_options(AntColonySimulator PRIVATE -Wall -Wextra -Wpedantic)

# Gestion des frameworks spécifiques à macOS si nécessaire
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    if(COCOA_LIBRARY)
        target_link_libraries(AntColonySimulator PRIVATE ${COCOA_LIBRARY})
    endif()
endif()
