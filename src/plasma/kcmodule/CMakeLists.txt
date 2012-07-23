set(kcm_owncloud_PART_SRCS
   kcmowncloud.cpp
   owncloudconfig.cpp
)

KDE4_ADD_UI_FILES(kcm_owncloud_PART_SRCS owncloudconfig.ui )

kde4_add_plugin(kcm_owncloud ${kcm_owncloud_PART_SRCS})

target_link_libraries(kcm_owncloud ${KDE4_KCMUTILS_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS kcm_owncloud DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES owncloudconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})

