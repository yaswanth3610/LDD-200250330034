cmd_/home/yesh/DeviceDrivers/waitQueues/waitqueue.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/yesh/DeviceDrivers/waitQueues/waitqueue.ko /home/yesh/DeviceDrivers/waitQueues/waitqueue.o /home/yesh/DeviceDrivers/waitQueues/waitqueue.mod.o;  true