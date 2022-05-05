/*
int libusb_get_configuration(libusb_device_handle *dev_handle, int *config);
    获取当前活跃的配置的 bConfigurationValue 值，通过 *config 返回。
    如果设备处于未配置状态，此函数将通过 *config 返回0.
    成功返回0，如果设备已断开连接就返回 LIBUSB_ERROR_NO_DEVICE 。

int libusb_set_configuration(libusb_device_handle *dev_handle, int configuration);
    使设备指定的配置为活跃的配置。
    操作系统可能已经在设备上设置了活动配置，也可能未设置。
    在 claim 接口和执行其他操作之前，由应用程序确保选择了正确的配置。
    如果在已使用所选配置配置的设备上调用此函数，则此函数将充当轻量级设备重置：
    它将使用当前配置发出 SET_CONFIGURATION 请求，造成大多数usb设备相关的状态被重置
    (altsetting重置为0，endpoint内容清0，开关复位)。
    并非所有后端都支持从用户空间设置配置，通过返回 LIBUSB_ERROR_NOT_SUPPORTED 来表
    示不支持。由于这表明平台正在处理设备配置本身，因此通常可以安全地忽略此错误。
    如果应用程序已 claim 接口，则无法更改或重置配置。建议在 claim 接口之前设置配置。
    或者先调用 libusb_release_interface() 释放接口，不过要先确保 auto_detach_kernel_driver
    为0，不然刚释放又会自动连上相应的驱动程序。
    如果其他应用程序或驱动程序已 claim 接口，则无法更改或重置配置。
    configuration 为 -1 将使设备设置为未配置状态。USB规范规定使用0将设备设置为未配
    置状态，但是实际上存在 bConfigurationValue 为 0 的错误设备。
    应该始终使用此函数，而不是制定自己的 SET_CONFIGURATION 请求。这是因为底层操作系
    统需要知道这些更改何时发生。
    这是一个阻塞函数。
    返回值：
        LIBUSB_ERROR_NOT_FOUND      请求的配置不存在
        LIBUSB_ERROR_BUSY           接口已 claim
        LIBUSB_ERROR_NOT_SUPPORTED  后端不支持设置或更改配置
        LIBUSB_ERROR_NO_DEVICE      设备已断开连接

int libusb_set_auto_detach_kernel_driver(libusb_device_handle *dev_handle, int enable);
    启用或禁用 libusb 的自动内核驱动程序分离。
    启用此选项后，libusb 将在 claim 接口时自动 detach 接口上的内核驱动程序，并在
    释放 接口时又 attach 该驱动程序。
    默认情况下新打开的设备句柄上禁用自动内核驱动程序分离。
    在不支持 LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER 功能的平台上，此函数将返回
    LIBUSB_ERROR_NOT_SUPPORTED，libusb 将继续运行，就像从未调用过此函数一样。
    返回值：
        LIBUSB_SUCCESS              执行成功
        LIBUSB_ERROR_NOT_SUPPORTED  平台不支持

int libusb_claim_interface(libusb_device_handle *dev_handle, int interface_number); 
    声明给定设备句柄上的接口。
    在对其任何端点执行I/O之前，必须声明要使用的接口。
    尝试声明已声明的接口是合法的，在这种情况下，libusb只返回0，不做任何操作。
    如果将 auto_detach_kernel_driver 设置为1，则自动 detach 内核驱动程序。
    接口声明是纯逻辑操作，它不会导致通过总线发送任何请求。
    接口声明用于指示底层操作系统应用程序希望获得接口的所有权。
    这是一个非阻塞函数。
    返回值：
        LIBUSB_SUCCESS          执行成功
        LIBUSB_ERROR_NOT_FOUND  请求的接口不存在
        LIBUSB_ERROR_BUSY       另一个程序或驱动程序已声明该接口
        LIBUSB_ERROR_NO_DEVICE  设备已断开连接

int libusb_release_interface(libusb_device_handle *dev_handle, int interface_number);
    释放以前使用 libusb_claim_interface() 声明的接口。
    您应该在关闭设备句柄之前释放所有声明的接口。
    如果将 auto_detach_kernel_driver 设置为1，则自动 attach 内核驱动程序。
    将向设备发送 SET_INTERFACE 请求，将接口状态重置为第一个备用设置。
    这是一个阻塞函数。
    返回值：
        LIBUSB_SUCCESS          执行成功
        LIBUSB_ERROR_NOT_FOUND  请求的接口不存在
        LIBUSB_ERROR_NO_DEVICE  设备已断开连接

int libusb_set_interface_alt_setting(libusb_device_handle *dev_handle, int interface_number, int alternate_setting);    
    激活接口的备用设置。
    该接口必须是以前使用 libusb_claim_interface() 声明的。
    应该始终使用此功能，而不是制定自己的 SET_INTERFACE 请求。
    这是因为底层操作系统需要知道这些更改何时发生。
    这是一个阻塞函数。
    返回值：
        LIBUSB_SUCCESS          执行成功
        LIBUSB_ERROR_NOT_FOUND  未声明接口或请求的备用设置不存在
        LIBUSB_ERROR_NO_DEVICE  设备已断开连接

int libusb_clear_halt(libusb_device_handle *dev_handle, unsigned char endpoint);
    清除端点的停止或暂停状态。
    endpoint 是要清除状态的端点。
    暂停状态的端点在暂停状态停止之前无法接收或传输数据。
    在尝试清除暂停状态之前，应取消所有挂起的传输。
    这是一个阻塞函数。
    返回值：
        LIBUSB_SUCCESS          执行成功
        LIBUSB_ERROR_NOT_FOUND  端点不存在
        LIBUSB_ERROR_NO_DEVICE  设备已断开连接

int libusb_kernel_driver_active(libusb_device_handle *dev_handle, int interface_number);
    确定接口上的内核驱动程序是否处于活动状态。
    如果内核驱动程序处于活动状态，则无法声明接口，并且libusb将无法执行I/O。
    此功能在Windows上不可用。
    返回值：
        0                           无活跃的内核驱动
        1                           有活跃的内核驱动
        LIBUSB_ERROR_NO_DEVICE      设备已断开连接
        LIBUSB_ERROR_NOT_SUPPORTED  平台不支持

int libusb_detach_kernel_driver(libusb_device_handle *dev_handle, int interface_number);
    分离接口上的内核驱动。
    返回值：
        LIBUSB_SUCCESS              执行成功
        LIBUSB_ERROR_NOT_FOUND      无活跃的内核驱动
        LIBUSB_ERROR_INVALID_PARAM  接口不存在
        LIBUSB_ERROR_NO_DEVICE      设备已断开连接
        LIBUSB_ERROR_NOT_SUPPORTED  平台不支持

int libusb_attach_kernel_driver(libusb_device_handle *dev_handle, int interface_number);
    附上接口上的内核驱动。
    返回值：
        LIBUSB_SUCCESS              执行成功
        LIBUSB_ERROR_NOT_FOUND      无活跃的内核驱动
        LIBUSB_ERROR_INVALID_PARAM  接口不存在
        LIBUSB_ERROR_NO_DEVICE      设备已断开连接
        LIBUSB_ERROR_NOT_SUPPORTED  平台不支持
        LIBUSB_ERROR_BUSY           无法附加驱动程序，因为接口已声明
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libusb.h"

static void dumpdevice(libusb_context *libusbCtx, libusb_device *dev, uint16_t vid, uint16_t pid){
    uint8_t bus = libusb_get_bus_number(dev);
    uint8_t addr = libusb_get_device_address(dev);
    struct libusb_device_descriptor deviceDescriptor;
    memset(&deviceDescriptor, 0, sizeof(struct libusb_device_descriptor));
    libusb_get_device_descriptor(dev, &deviceDescriptor);
    
    printf("/dev/bus/usb/%03u/%03u %04x:%04x usb%u.%u class:%02x\n", 
        bus, addr, 
        deviceDescriptor.idVendor, 
        deviceDescriptor.idProduct, 
        (deviceDescriptor.bcdUSB & 0x0f00) >> 8, 
        (deviceDescriptor.bcdUSB & 0x00f0) >> 4, 
        deviceDescriptor.bDeviceClass
    );
    
    libusb_device_handle *devHandle = NULL;
    libusb_open(dev, &devHandle);
    if(devHandle){
        // 获取当前活跃的配置的 bConfigurationValue 值。
        int activeConfig = 99999;
        int errCode = libusb_get_configuration(devHandle, &activeConfig);
        printf("   libusb_get_configuration()=%d, activeConfig=%d\n", errCode, activeConfig);
        
        if((vid == deviceDescriptor.idVendor) && (pid == deviceDescriptor.idProduct)){
            // 启用内核驱动自动 detach 和自动 attach
            errCode = libusb_set_auto_detach_kernel_driver(devHandle, 1);
            printf("      libusb_set_auto_detach_kernel_driver(1)=%d\n", errCode);
            
            // 声明占用接口0
            errCode = libusb_claim_interface(devHandle, 0);
            printf("      libusb_claim_interface(0)=%d\n", errCode);
            
            // 是否有活跃的内核驱动
            // errCode = libusb_kernel_driver_active(devHandle, 0);
            // printf("      libusb_kernel_driver_active(0)=%d\n", errCode);
            
            // 分离接口上的内核驱动
            // errCode = libusb_detach_kernel_driver(devHandle, 0);
            // printf("      libusb_detach_kernel_driver(0)=%d\n", errCode);
            
            // 附上接口上的内核驱动
            // errCode = libusb_attach_kernel_driver(devHandle, 0);
            // printf("      libusb_attach_kernel_driver(0)=%d\n", errCode);
            
            // 释放对接口0的声明
            errCode = libusb_release_interface(devHandle, 0);
            printf("      libusb_release_interface(0)=%d\n", errCode);
        }
        
        libusb_close(devHandle);
    }
}

int main(int argc, char* argv[]){
    libusb_context *libusbCtx = NULL;
    int errCode = libusb_init(&libusbCtx);
    if(errCode) return -1;
    
    uint16_t vid = 0;
    uint16_t pid = 0;
    if(argc >= 2){
        sscanf(argv[1], "%04hx:%04hx", &vid, &pid);
        printf("VID:PID = %04x:%04x\n", vid, pid);
    }
    
    // 枚举所有USB设备
    libusb_device **devList = NULL;
    ssize_t count = libusb_get_device_list(libusbCtx, &devList);
    if(count > 0){
        ssize_t i;
        for(i=0; i<count; i++){
            dumpdevice(libusbCtx, devList[i], vid, pid);
        }
        libusb_free_device_list(devList, 1);
        devList = NULL;
    }
    
    libusb_exit(libusbCtx);
    return 0;
}

