vendor_dir = vendor
build_dir = build
toolchain = ~/Projects/SAMD21/toolchain_samd21g18a.cmake


all: $(vendor_dir)/cmsis-core-atmel-samcortexm0p-samd21 $(vendor_dir)/FreeRTOS $(build_dir)
	cd $(build_dir); make

_dependencies: | $(vendor_dir)/cmsis-core-atmel-samcortexm0p-samd21 $(vendor_dir)/FreeRTOS

$(build_dir):
	mkdir $(build_dir)
	cd $(build_dir); cmake -DCMAKE_TOOLCHAIN_FILE=$(toolchain) ..

$(vendor_dir):
	mkdir $(vendor_dir)


$(vendor_dir)/cmsis-core: | $(vendor_dir)
	cd $(vendor_dir); git clone -b v1.2.0 --depth 1 https://github.com/ARMmbed/cmsis-core.git

$(vendor_dir)/cmsis-core-atmel-samcortexm0p-samd21: | $(vendor_dir)/cmsis-core
	cd $(vendor_dir); git clone -b master --depth 1 https://github.com/ARMmbed/cmsis-core-atmel-samcortexm0p-samd21.git

$(vendor_dir)/FreeRTOS: | $(vendor_dir)
	cd $(vendor_dir); git clone -b V9.0.0 --depth 1 https://github.com/cjlano/freertos.git
	mv $(vendor_dir)/freertos/FreeRTOS $(vendor_dir)/FreeRTOS
	rm -rf $(vendor_dir)/freertos

distclean:
	rm -rf $(build_dir)

maintainer-clean: distclean
	rm -rf $(vendor_dir)

.PHONY: all distclean maintainer-clean