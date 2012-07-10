project( active_settings_owncloud )
#find_package(KDE4 REQUIRED)
#include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

#include_directories(${KDE4_INCLUDES} ../../../../components/settings)


set(owncloudsettings_SRCS
    owncloudsettingsplugin.cpp
    owncloudsettings.cpp
)


kde4_add_plugin(active_settings_owncloud ${owncloudsettings_SRCS})
target_link_libraries(active_settings_owncloud ${KDE4_PLASMA_LIBS} ${QT_QTDECLARATIVE_LIBRARY})
install(TARGETS active_settings_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})

install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.active.settings.owncloud)
install(FILES package/metadata.desktop DESTINATION ${SERVICES_INSTALL_DIR} RENAME plasma-package-org.kde.active.settings.owncloud.desktop)

