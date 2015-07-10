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
    ../lib/dirmodel.cpp
    ../lib/minijob.cpp
    ../lib/job.cpp
    ../lib/createfolderjob.cpp
    ../lib/syncprogress.cpp
)

qt5_add_dbus_interface(owncloudsettings_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )


add_library(active_settings_owncloud MODULE ${owncloudsettings_SRCS})
target_link_libraries(active_settings_owncloud KF5::Plasma Qt5::Declarative KF5::KIOWidgets )
install(TARGETS active_settings_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})

