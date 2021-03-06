AS = nasm
AS_FLAGS = -fbin -O3

AS_INCLUDE = $(shell find . -type f -name "*.inc")

build: dirent test/fat32.img
	@ printf '\e[1m\e[36m[%s]\e[0m | Done!\n' TINYBOOT

clean:
	-rm -rf bin test mnt

test/fat32.img: bin/tinyboot.bin
	@ dd if=/dev/zero of=$@ count=131072
	@ mkfs.fat -R 4 -F32 $@
	@ dd conv=notrunc bs=1 count=3 skip=0 seek=0 if=$< of=$@
	@ dd conv=notrunc bs=1 count=1920 skip=128 seek=128 if=$< of=$@
	@ sudo mount $@ mnt
	@ sudo sh -c "echo \"Hello, world!\" > mnt/config.txt"
	@ sudo sh -c "echo \"Wait, I'M IN A HELP MENU!\\n(newline?)\" > mnt/help.txt"
	@ sleep 0.5
	@ sudo umount $@

dirent:
	@ mkdir -p bin test mnt

bin/tinyboot.bin: src/boot.asm $(AS_INCLUDE)
	@ printf '\e[1m\e[36m[%s]\e[0m | %s -> %s\n' AS $< $@
	$(AS) $(AS_FLAGS) $< -o $@
