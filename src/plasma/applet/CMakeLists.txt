project(plasma-applet-owncloud)

include (KDE4Defaults)

set(owncloud_SRCS
    owncloudapplet.cpp
    ../lib/owncloudsettings.cpp
    ../lib/owncloudfolder.cpp
)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

kde4_add_plugin(plasma_applet_owncloud ${owncloud_SRCS})
target_link_libraries(plasma_applet_owncloud ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_applet_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-owncloud.desktop DESTINATION ${SERVICES_INSTALL_DIR})
