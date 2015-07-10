project(plasma-applet-owncloud)

include (KDE4Defaults)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${plasma_applet_owncloud_BINARY_DIR}
  )

set(owncloud_SRCS
    owncloudapplet.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
    ../lib/directorylister.cpp
    ../lib/dirmodel.cpp
    ../lib/minijob.cpp
    ../lib/job.cpp
    ../lib/createfolderjob.cpp
    ../lib/syncprogress.cpp
  )

qt5_add_dbus_interface(owncloud_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

add_library(plasma_applet_owncloud MODULE ${owncloud_SRCS})
target_link_libraries(plasma_applet_owncloud KF5::Plasma Qt5::Declarative  KF5::KIOWidgets)

install(TARGETS plasma_applet_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-owncloud.desktop DESTINATION ${SERVICES_INSTALL_DIR})
