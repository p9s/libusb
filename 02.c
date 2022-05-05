/*
ssize_t libusb_get_device_list(libusb_context *ctx, libusb_device ***list);
    获取所有usb设备的列表，通过 ***list 返回这个列表。
    使用完毕需要使用 libusb_free_device_list() 来释放列表。
    返回值表示列表中的设备个数。该列表实际上要大一个元素，因为它以NULL结尾。
    如果返回值小于0，那么返回值代表错误码列表中的某个错误。

void libusb_free_device_list(libusb_device **list, int unref_devices);
    释放由 libusb_get_device_list() 获取的usb设备列表。
    如果设置了 unref_devices 参数，则列表中每个设备的引用计数将减1。

uint8_t libusb_get_bus_number(libusb_device *dev);
    获取某个USB设备所在的总线号。
    例如 "/dev/bus/usb/001/003" 中的 "001" 就是总线号。

uint8_t libusb_get_device_address(libusb_device *dev);
    获取某个USB设备在总线上的地址号。
    例如 "/dev/bus/usb/001/003" 中的 "003" 就是总线上的地址号。
    每次插入其地址号会重新分配。

uint8_t libusb_get_port_number(libusb_device *dev);
    获取某个USB设备所在的端口号。
    端口号可理解为USB物理插孔编号。
    返回0表示该设备不可用。

int libusb_get_port_numbers(libusb_device *dev, uint8_t *port_numbers, int port_numbers_len);
    获取某个USB设备从根端口号到自己所在的端口号之间所路过的所有端口号。
    从版本 1.0.16, LIBUSB_API_VERSION >= 0x01000102 开始支持。
    USB协议限制端口号路径最大深度为7。
    返回端口号数量。
    如果 *port_numbers 容量不足返回 LIBUSB_ERROR_OVERFLOW。

int libusb_get_port_path(libusb_context *ctx, libusb_device *dev, uint8_t *port_numbers, uint8_t port_numbers_len);
    请改用 libusb_get_port_numbers()。

int libusb_get_device_speed(libusb_device *dev);
    获取某个USB设备的协商连接速度。

USB设备速度列表：
    LIBUSB_SPEED_UNKNOWN     0  操作系统不报告或不知道设备速度。
    LIBUSB_SPEED_LOW         1  设备正在低速运行   (1.5MBit/s).
    LIBUSB_SPEED_FULL        2  设备正在全速运行   (12MBit/s).
    LIBUSB_SPEED_HIGH        3  设备正在高速运行   (480MBit/s).
    LIBUSB_SPEED_SUPER       4  设备正在超高速运行 (5000MBit/s).
    LIBUSB_SPEED_SUPER_PLUS  5  设备正在超超速运行 (10000MBit/s).

libusb_device* libusb_get_parent(libusb_device *dev);
    获取父设备，如果不存在父设备就返回NULL。
    注意，返回的父设备依然在 libusb_get_device_list() 获取的设备列表中，并没有创建新的实例。

libusb_device* libusb_ref_device(libusb_device *dev);
    对USB设备增加引用计数，返回相同的USB设备。
    可以理解为复制一个USB设备，但绝不是真的复制。

void libusb_unref_device(libusb_device *dev);
    对USB设备减少引用计数，当引用计数为0时会释放USB设备实例。
    可以理解为释放一个USB设备。
    实测当引用计数为0时好像并没有真的释放设备实例。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libusb.h"

static char* getPortsString(libusb_device *dev){
    static char partsString[64];
    
    uint8_t ports[16];
    int count = libusb_get_port_numbers(dev, ports, 16);
    if(count > 0){
        int i, offset = 0;
        for(i=0; i<count; i++){
            offset += sprintf(partsString+offset, "->%03u", ports[i]);
        }
        return partsString;
    }else{
        return "";
    }
}

static void dumpdevice(libusb_context *libusbCtx, libusb_device *dev){
    static char* speedName[] = {
        "未知速度", 
        "低速(1.5MBit/s)", 
        "全速(12MBit/s)", 
        "高速(480MBit/s)", 
        "超高速(5000MBit/s)", 
        "超超速(10000MBit/s)"
    };
    
    libusb_device *parent = libusb_get_parent(dev);
    printf("    bus     = %03u\n",   libusb_get_bus_number(dev));
    printf("    address = %03u\n",   libusb_get_device_address(dev));
    printf("    port    = %03u\n",   libusb_get_port_number(dev));
    printf("    ports   = root%s\n", getPortsString(dev));
    printf("    speed   = %s\n",     speedName[libusb_get_device_speed(dev)]);
    printf("    parent  = %s\n",     parent?"":"NULL");
    if(parent){
        printf("        bus     = %03u\n",   libusb_get_bus_number(parent));
        printf("        address = %03u\n",   libusb_get_device_address(parent));
        printf("        port    = %03u\n",   libusb_get_port_number(parent));
        printf("        ports   = root%s\n", getPortsString(parent));
        printf("        speed   = %s\n",     speedName[libusb_get_device_speed(parent)]);
    }
    putchar('\n');
}

int main(int argc, char* argv[]){
    libusb_context *libusbCtx = NULL;
    int errCode = libusb_init(&libusbCtx);
    if(errCode) return -1;
    
    libusb_device *dev0 = NULL;
    libusb_device **devList = NULL;
    ssize_t count = libusb_get_device_list(libusbCtx, &devList);
    printf("libusb_get_device_list() = %d\n", (int)count);
    if(count > 0){
        // 增加 devList[0] 所指设备的引用计数。
        dev0 = libusb_ref_device(devList[0]);
        
        // 打印设备列表上所有设备的信息
        ssize_t i;
        for(i=0; i<count; i++){
            dumpdevice(libusbCtx, devList[i]);
        }
        
        // 释放设备列表上的所有设备，devList[0] 除外。
        printf("libusb_free_device_list()\n");
        libusb_free_device_list(devList, 1);
        devList = NULL;
    }
    
    if(dev0){
        // 打印 dev0 所指设备的信息，也是原 devList[0] 所指设备的信息。
        dumpdevice(libusbCtx, dev0);
        
        // 引用计数减一。这次减一后引用计数就为0了。
        // 此处的 dumpdevice() 应该会出现段错误，然而并没有。
        libusb_unref_device(dev0);
        dumpdevice(libusbCtx, dev0);
        
        // 还是没有段错误！
        libusb_unref_device(dev0);
        dumpdevice(libusbCtx, dev0);
        
        // 还是没有段错误！
        libusb_unref_device(dev0);
        dumpdevice(libusbCtx, dev0);
    }
    
    libusb_exit(libusbCtx);
    
    // 调用 libusb_exit() 后原 devList[0] 所指设备依然可访问！
    if(dev0){
        // 还是没有段错误！
        libusb_unref_device(dev0);
        dumpdevice(libusbCtx, dev0);
        
        // 还是没有段错误！
        libusb_unref_device(dev0);
        dumpdevice(libusbCtx, dev0);
    }
    
    return 0;
}

