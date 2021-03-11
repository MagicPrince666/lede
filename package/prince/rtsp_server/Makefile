include $(TOPDIR)/rules.mk

PKG_NAME:=rtsp_server
PKG_VERSION:=huangliquan-1.0
PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/rtsp_server
	SECTION:=base
	CATEGORY:=Utilities
	TITLE:=rtsp server
	SUBMENU:=prince
	DEPENDS:= +libstdcpp +libpthread +libusb-1.0
endef

define Package/rtsp_server/description
	rtsp server to h264 video
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/rtsp_server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rtsp_server $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/video_server.sh $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rtsp_client $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rtsp_server))
