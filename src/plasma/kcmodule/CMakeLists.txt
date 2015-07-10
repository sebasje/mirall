set(kcm_owncloud_PART_SRCS
    kcmowncloud.cpp
    owncloudconfig.cpp
    ../lib/directorylister.cpp
    ../lib/dirmodel.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
    ../lib/minijob.cpp
    ../lib/job.cpp
    ../lib/createfolderjob.cpp
    ../lib/syncprogress.cpp
)
qt5_add_dbus_interface(kcm_owncloud_PART_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )

#KDE4_ADD_UI_FILES(kcm_owncloud_PART_SRCS owncloudconfig.ui )

add_library(kcm_owncloud MODULE ${kcm_owncloud_PART_SRCS})

target_link_libraries(kcm_owncloud
    Qt5::Declarative
    KF5::KCMUtils
    KF5::Plasma
    ${KDECLARATIVE_LIBRARIES}
    KF5::KIOWidgets
)

install(TARGETS kcm_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES owncloudconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES settings-synchronization.desktop DESTINATION ${SERVICES_INSTALL_DIR})

