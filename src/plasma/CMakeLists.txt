project(plasma-owncloud)

cmake_minimum_required(VERSION 2.8.10.1)

################# Disallow in-source build #################

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message(FATAL_ERROR "plasma-owncloud requires an out of source build. Please create a separate build directory and run 'cmake path_to_plasma-mobile [options]' there.")
endif()

include(CPack)
include(FeatureSummary)
find_package(PkgConfig)

################# set KDE specific information #################

find_package(ECM 0.0.8 REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_SOURCE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core DBus Quick Test Gui)

#find_package(OWNCLOUD REQUIRED)
################# now find all used packages #################

set (QT_MIN_VERSION "5.4.0")

find_package(KF5 REQUIRED COMPONENTS
    WindowSystem CoreAddons Solid Config Auth DBusAddons
    I18n Plasma PlasmaQuick Declarative KIO KDELibs4Support
    )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/lib/
                     ${CMAKE_SOURCE_DIR}/src/libsync/
                     ${CMAKE_BINARY_DIR}/src/libsync/
                     ${CMAKE_SOURCE_DIR}/csync/src/
                     ${CMAKE_BINARY_DIR}/csync/
                     ${CMAKE_SOURCE_DIR}/src/gui/
                     ${CMAKE_SOURCE_DIR}/src/
                     ${CMAKE_SOURCE_DIR}/src/3rdparty/qjson/)
set(LIBSYNC_ADDITIONAL_SRCS
  ../../gui/folder.cpp
  ../../gui/folderman.cpp
  ../../gui/folderwatcher.cpp
  ../../gui/folderwatcher_linux.cpp
  ../../gui/accountstate.cpp
  ../../gui/accountmanager.cpp
  ../../gui/sslerrordialog.cpp
  ../../gui/creds/httpcredentialsgui.cpp
  ../../gui/creds/credentialsfactory.cpp
  ../../gui/creds/shibbolethcredentials.cpp
  ../../gui/creds/shibboleth/shibbolethwebview.cpp
  ../../gui/creds/shibboleth/shibbolethrefresher.cpp
  ../../gui/creds/shibboleth/shibbolethuserjob.cpp
  ../../gui/socketapi.cpp
  ../../gui/syncrunfilelog.cpp
  )


message("========================== BAH ${CMAKE_SOURCE_DIR}/../libsync/ ${CMAKE_SOURCE_DIR}/csync/src/")

#add_subdirectory(applet)
add_subdirectory(owncloudsync)
add_subdirectory(import)
#add_subdirectory(settingsmodule)
#add_subdirectory(kcmodule)


# plasma_install_package(package org.kde.active.settings packages package)


# install(FILES active-settings.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

# ----
# find_package(KDE4 REQUIRED)
# find_package(KDeclarative REQUIRED)
#
# include (KDE4Defaults)
# include_directories(${KDE4_INCLUDES} lib/)
#
# add_subdirectory(applet)
# add_subdirectory(owncloudsync)
# add_subdirectory(settingsmodule)
# add_subdirectory(kcmodule)
#
# install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.active.settings.owncloud)
# install(FILES package/metadata.desktop DESTINATION ${SERVICES_INSTALL_DIR} RENAME plasma-package-org.kde.active.settings.owncloud.desktop)
#
# install(FILES owncloudsyncd.desktop DESTINATION ${AUTOSTART_INSTALL_DIR})
