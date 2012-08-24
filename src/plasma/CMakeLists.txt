
find_package(KDE4 REQUIRED)
find_package(KDeclarative REQUIRED)

include (KDE4Defaults)
include_directories(${KDE4_INCLUDES} lib/)

add_subdirectory(applet)
add_subdirectory(owncloudsync)
add_subdirectory(settingsmodule)
add_subdirectory(kcmodule)

install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.active.settings.owncloud)
install(FILES package/metadata.desktop DESTINATION ${SERVICES_INSTALL_DIR} RENAME plasma-package-org.kde.active.settings.owncloud.desktop)

