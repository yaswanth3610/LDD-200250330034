cmd_/home/yesh/DeviceDrivers/ioctldemo/ioctldemo.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/yesh/DeviceDrivers/ioctldemo/ioctldemo.ko /home/yesh/DeviceDrivers/ioctldemo/ioctldemo.o /home/yesh/DeviceDrivers/ioctldemo/ioctldemo.mod.o;  true