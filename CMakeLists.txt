cmake_minimum_required(VERSION 3.16)

project(BazyDanychJachtyApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)
find_package(MySQL)

# Optionally locate MySQL
# find_path(MySQL_INCLUDE_DIR NAMES mysql.h
#           HINTS "C:/Program Files/MySQL/MySQL Connector C 6.1/include")
# find_library(MySQL_LIBRARIES NAMES mysql
#              HINTS "C:/Program Files/MySQL/MySQL Connector C 6.1/lib")


# if(NOT MySQL_INCLUDE_DIR OR NOT MySQL_LIBRARIES)
#     message(FATAL_ERROR "MySQL Connector/C not found. Please install and set paths.")
# endif()

# include_directories(${MySQL_INCLUDE_DIR})
# link_libraries(${MySQL_LIBRARIES})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BazyDanychJachtyApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        databasemanager.h
        databasemanager.cpp
        login.h login.cpp login.ui


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BazyDanychJachtyApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BazyDanychJachtyApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BazyDanychJachtyApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(BazyDanychJachtyApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BazyDanychJachtyApp)
endif()
set_target_properties(BazyDanychJachtyApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BazyDanychJachtyApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BazyDanychJachtyApp)
endif()
