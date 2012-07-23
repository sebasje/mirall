set(kcm_owncloud_PART_SRCS
    kcmowncloud.cpp
    owncloudconfig.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
)
qt4_add_dbus_interface(kcm_owncloud_PART_SRCS ../owncloudsync/org.kde.owncloudsync.xml owncloud_interface )

#KDE4_ADD_UI_FILES(kcm_owncloud_PART_SRCS owncloudconfig.ui )

kde4_add_plugin(kcm_owncloud ${kcm_owncloud_PART_SRCS})

target_link_libraries(kcm_owncloud
    ${QT_QTDECLARATIVE_LIBRARY}
    ${KDE4_KCMUTILS_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDECLARATIVE_LIBRARIES}
)

install(TARGETS kcm_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES owncloudconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})

