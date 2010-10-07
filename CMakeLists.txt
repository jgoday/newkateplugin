project(newkateplugin)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

# The FindKDE4.cmake module sets _KDE4_PLATFORM_DEFINITIONS with
# definitions like _GNU_SOURCE that are needed on each platform.
set(CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS})

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(newkateplugin_UI ui/nldialog.ui ui/nlopendialog.ui)

set(newkateplugin_SRCS
    newkateplugin.cpp

    nlconfigpage.cpp
    nldialog.cpp
    nlopendialog.cpp
    nlmodel.cpp
    nlbookmarkmodel.cpp
    nlbookmarkview.cpp
)

kde4_add_ui_files(newkateplugin_SRCS ${newkateplugin_UI})

kde4_add_plugin(newkateplugin ${newkateplugin_SRCS})
target_link_libraries(newkateplugin  ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KPARTS_LIBS} kateinterfaces)

install(TARGETS newkateplugin  DESTINATION ${PLUGIN_INSTALL_DIR} )
install(FILES newkateplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR})
