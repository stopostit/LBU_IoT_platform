#
# Copyright (C) 2006-2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=dragino_listener
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_MAINTAINER:=Maxime Caisez
PKG_LICENSE:=GPL-2

TARGET_LDFLAGS+= \
  -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

define Package/dragino_listener
		SECTION:=utils
		CATEGORY:=Utilities
		DEPENDS:=+libcurl +libssl
		TITLE:=Dragino gateway listener with CURL HTTPS POST requests
		URL:=https://github.com/stopostit/TODO
		MENU:=1
endef

define Package/dragino_listener/description
		TODO
endef

define Build/prepare
		mkdir -p $(PKG_BUILD_DIR)
		$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Configure
endef


define Build/Compile
		$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/dragino_listener/install
		$(INSTALL_DIR) $(1)/bin
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/dragino_listener/bin
		$(INSTALL_DIR) $(1)/etc/config
		$(INSTALL_CONF) $(PKG_BUILD_DIR)/dragino_listener.conf $(1)/etc/config/dragino_listener
endef
 $(eval $(call BuildPackage,dragino_listener))
