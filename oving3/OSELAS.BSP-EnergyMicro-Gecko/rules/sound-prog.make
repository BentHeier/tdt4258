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
PACKAGES-$(PTXCONF_SOUND_PROG) += sound-prog

#
# Paths and names
#
SOUND_PROG_VERSION	:= 1
SOUND_PROG_MD5		:=
SOUND_PROG		:= sound-prog
SOUND_PROG_URL		:= file://local_src/$(SOUND_PROG)
SOUND_PROG_DIR		:= $(BUILDDIR)/$(SOUND_PROG)
SOUND_PROG_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/sound-prog.extract:
#	@$(call targetinfo)
#	@$(call clean, $(SOUND_PROG_DIR))
#	@$(call extract, SOUND_PROG)
#	@$(call patchin, SOUND_PROG)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#SOUND_PROG_PATH	:= PATH=$(CROSS_PATH)
SOUND_PROG_CONF_TOOL	:= NO
SOUND_PROG_MAKE_ENV	:= $(CROSS_ENV)

#$(STATEDIR)/sound-prog.prepare:
#	@$(call targetinfo)
#	@$(call world/prepare, SOUND_PROG)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/sound-prog.compile:
#	@$(call targetinfo)
#	@$(call world/compile, SOUND_PROG)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/sound-prog.install:
#	@$(call targetinfo)
#	@$(call world/install, SOUND_PROG)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-prog.targetinstall:
	@$(call targetinfo)

	@$(call install_init, sound-prog)
	@$(call install_fixup, sound-prog,PRIORITY,optional)
	@$(call install_fixup, sound-prog,SECTION,base)
	@$(call install_fixup, sound-prog,AUTHOR,"<Oss>")
	@$(call install_fixup, sound-prog,DESCRIPTION,missing)

#
# TODO: Add here all files that should be copied to the target
# Note: Add everything before(!) call to macro install_finish
#
	@$(call install_copy, sound-prog, 0, 0, 0755, $(SOUND_PROG_DIR)/sound-prog, /usr/bin/sound-prog)

	@$(call install_finish, sound-prog)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/sound-prog.clean:
	@$(call targetinfo)
	@-cd $(SOUND_PROG_DIR) && \
		$(SOUND_PROG_ENV) $(SOUND_PROG_PATH) $(MAKE) clean
	@$(call clean_pkg, SOUND_PROG)

# vim: syntax=make
