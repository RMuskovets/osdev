# qemu-system-i386 -kernel os_kernel -vga std -k en-us -m 1024M -hda \
# ext2_hda.img -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img -serial stdio


qemu-system-i386 -kernel os_kernel -vga std -k en-us -m 1024M -serial stdio -hda ext2_hda.img -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img