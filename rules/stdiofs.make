# -*-makefile-*-
#
# Copyright (C) 2022 by Falk Werner
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_STDIOFS) += stdiofs

#
# Paths and names
#
STDIOFS_VERSION	:= 1.1.0
STDIOFS		:= stdiofs-$(STDIOFS_VERSION)
STDIOFS_SUFFIX	:= tar.gz
STDIOFS_URL	:= https://github.com/falk-werner/stdiofs/archive/refs/tags/v$(STDIOFS_VERSION).$(STDIOFS_SUFFIX)
STDIOFS_MD5     := 428ad41ad1e353f89b753f532be0a34a
STDIOFS_SOURCE	:= $(SRCDIR)/$(STDIOFS).$(STDIOFS_SUFFIX)
STDIOFS_DIR	:= $(BUILDDIR)/$(STDIOFS)
STDIOFS_LICENSE	:= MPL-2.0

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STDIOFS_SOURCE):
#	@$(call targetinfo)
#	@$(call get, STDIOFS)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#STDIOFS_CONF_ENV	:= $(CROSS_ENV)

#
# cmake
#
STDIOFS_CONF_TOOL	:= cmake
STDIOFS_CONF_OPT	:= $(CROSS_CMAKE_USR) \
-DWITHOUT_TEST=ON \
-DWITHOUT_PROVIDER=ON \
-DCMAKE_C_FLAGS:STRING="-D_FILE_OFFSET_BITS=64"

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/stdiofs.targetinstall:
	@$(call targetinfo)

	@$(call install_init, stdiofs)
	@$(call install_fixup, stdiofs, PRIORITY, optional)
	@$(call install_fixup, stdiofs, SECTION, base)
	@$(call install_fixup, stdiofs, AUTHOR, "Falk Werner")
	@$(call install_fixup, stdiofs, DESCRIPTION, missing)

	@$(call install_copy, stdiofs, 0, 0, 0755, -, /usr/bin/stdiofs)

	@$(call install_finish, stdiofs)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/stdiofs.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, STDIOFS)

# vim: syntax=make
