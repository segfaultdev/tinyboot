#!/bin/sh

mkdir -p bin

cd src
nasm -fbin boot.asm -o ../bin/tinyboot.bin
cd ..

dd if=/dev/zero of=test/fat32.img count=131072
mkfs.fat -R 4 -F32 test/fat32.img
dd conv=notrunc bs=1 count=3 skip=0 seek=0 if=bin/tinyboot.bin of=test/fat32.img
dd conv=notrunc bs=1 count=1920 skip=128 seek=128 if=bin/tinyboot.bin of=test/fat32.img

sudo mount test/fat32.img mnt
sudo sh -c "echo \"Hello, world!\" > mnt/config.txt"
sudo umount test/fat32.img
