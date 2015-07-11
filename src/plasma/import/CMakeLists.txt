
set(owncloudimport_SRCS
    owncloudimport.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
    ../lib/directorylister.cpp
    ../lib/dirmodel.cpp
    ../lib/minijob.cpp
    ../lib/job.cpp
    ../lib/createfolderjob.cpp
    ../lib/syncprogress.cpp
)

qt5_add_dbus_interface(owncloudimport_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )

add_library(owncloudimport SHARED ${owncloudimport_SRCS})
target_link_libraries(owncloudimport
        Qt5::DBus
        Qt5::Quick
        Qt5::Qml
        KF5::I18n
        KF5::KIOCore
        KF5::KIOWidgets
#         KF5::Declarative
#         KF5::IconThemes
#         KF5::Service #for kplugininfo.h
#         KF5::WindowSystem
#         KF5::Plasma
#         KF5::PlasmaQuick
#         KF5::QuickAddons
)

#install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/components)

message("DESTINATION ${KDE_INSTALL_QMLDIR}")

install(TARGETS owncloudimport DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/owncloud)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/owncloud)
install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/owncloud)
