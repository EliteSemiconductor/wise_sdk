# Location of kconfig tools (you must have them installed)
KCONFIG_SCRIPT := kconfig-mconf

# Use ENV variable or default location
KCONFIG_CONFIG ?= .config

all:
	@echo "Try: make menuconfig"

menuconfig:
	$(KCONFIG_SCRIPT) Kconfig

defconfig:
	$(KCONFIG_SCRIPT) Kconfig --defconfig=defconfig

clean:
	rm -f .config .config.old

.PHONY: all menuconfig defconfig clean

