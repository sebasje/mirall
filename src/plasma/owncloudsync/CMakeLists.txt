project(owncloudsyncd)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/../../gui/
  ${owncloudsyncdaemon_BINARY_DIR}
  ../..
  ${QTKEYCHAIN_INCLUDE_DIR}
  )

set(owncloudsyncd_SRCS
  owncloudsync.cpp
  owncloudsyncdaemon.cpp
  kfilesync.cpp
  ${LIBSYNC_ADDITIONAL_SRCS}
  main.cpp
  )

# IF( NOT WIN32 AND NOT APPLE )
# set(owncloudsyncd_SRCS ${owncloudsyncd_SRCS} folderwatcher_linux.cpp)
# ENDIF()
# IF( WIN32 )
# set(owncloudsyncd_SRCS ${owncloudsyncd_SRCS} folderwatcher_win.cpp)
# ENDIF()
# IF( APPLE )
# list(APPEND owncloudsyncd_SRCS folderwatcher_mac.cpp)
# ENDIF()



qt5_add_dbus_adaptor(owncloudsyncd_SRCS
  org.kde.owncloudsync.xml
  owncloudsync.h
  OwncloudSync)

add_executable(owncloudsyncd ${owncloudsyncd_SRCS})

ADD_LIBRARY(owncloudsyncd_SRCS STATIC IMPORTED)

target_link_libraries(owncloudsyncd
    Qt5::Core
    Qt5::DBus
    Qt5::Qml
    #Qt5::Quick
    KF5::KIOCore
    KF5::KDELibs4Support
    owncloudsync
)


install(
  FILES owncloudsync.desktop
  DESTINATION ${AUTOSTART_INSTALL_DIR})

install(
  TARGETS owncloudsyncd
  ${INSTALL_TARGETS_DEFAULT_ARGS})
