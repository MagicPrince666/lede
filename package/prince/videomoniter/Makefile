include $(TOPDIR)/rules.mk

PKG_NAME:=video_moniter
PKG_VERSION:=huangliquan-1.0
PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=base
	CATEGORY:=Utilities
	TITLE:=video transfer by wifi moniter
	SUBMENU:=prince
	DEPENDS:= +libpthread +libstdcpp +libpcap
endef

define Package/$(PKG_NAME)/description
	wifi transfer video firmware 
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/video_proxy $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xagaoa $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
