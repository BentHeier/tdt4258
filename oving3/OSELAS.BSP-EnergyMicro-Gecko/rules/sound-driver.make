# -*-makefile-*-
#
# Copyright (C) 2016 by <Oss>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_SOUND_DRIVER) += sound-driver

#
# Paths and names and versions
#
SOUND_DRIVER_VERSION	:= 1
SOUND_DRIVER		:= sound-driver-$(SOUND_DRIVER_VERSION)
SOUND_DRIVER_URL		:= lndir://$(PTXDIST_WORKSPACE)/local_src/$(SOUND_DRIVER)
SOUND_DRIVER_DIR		:= $(BUILDDIR)/$(SOUND_DRIVER)
SOUND_DRIVER_LICENSE	:= unknown

ifdef PTXCONF_SOUND_DRIVER
$(STATEDIR)/kernel.targetinstall.post: $(STATEDIR)/sound-driver.targetinstall
endif

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-driver.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-driver.compile:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(SOUND_DRIVER_DIR) \
		modules
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-driver.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-driver.targetinstall:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(SOUND_DRIVER_DIR) \
		modules_install
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/sound-driver.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, SOUND_DRIVER)

# vim: syntax=make
