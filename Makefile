vendor_dir = vendor
build_dir = build
toolchain = ./toolchain_samd21g18a.cmake


all: $(build_dir)
	cd $(build_dir) && make

test:
	@echo "TODO: Implement Unit Tests"

$(build_dir):
	mkdir $(build_dir)
	cd $(build_dir) && cmake -DCMAKE_TOOLCHAIN_FILE=$(toolchain) $(PWD)

$(vendor_dir):
	mkdir $(vendor_dir)

distclean:
	rm -rf $(build_dir)

.PHONY: all test distclean