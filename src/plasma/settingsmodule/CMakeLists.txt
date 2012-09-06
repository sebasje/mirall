project( active_settings_owncloud )
#find_package(KDE4 REQUIRED)
#include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(../lib/)


set(owncloudsettings_SRCS
    owncloudsettingsplugin.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
    ../lib/directorylister.cpp
    ../lib/minijob.cpp
    ../lib/job.cpp
    ../lib/createfolderjob.cpp
)
qt4_add_dbus_interface(owncloudsettings_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )


kde4_add_plugin(active_settings_owncloud ${owncloudsettings_SRCS})
target_link_libraries(active_settings_owncloud ${KDE4_PLASMA_LIBS} ${QT_QTDECLARATIVE_LIBRARY})
install(TARGETS active_settings_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})

