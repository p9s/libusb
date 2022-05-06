/*
int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc);
    获取设备的设备描述符。
    从版本 1.0.16、LIBUSB_API_VERSION>=0x01000102 开始此函数始终成功。

设备描述符：
struct libusb_device_descriptor {
    uint8_t   bLength;             // 本描述符的字节数，恒等于18。
    uint8_t   bDescriptorType;     // 本描述符类型，恒等于0x01。
    uint16_t  bcdUSB;              // USB规范版本号。
    uint8_t   bDeviceClass;        // USB标准化组织的类代码。
    uint8_t   bDeviceSubClass;     // USB标准化组织指定的子类代码，由 bDeviceClass 值限定。
    uint8_t   bDeviceProtocol;     // USB标准化组织指定的协议代码，由 bDeviceClass 和 bDeviceSubClass 值限定。
    uint8_t   bMaxPacketSize0;     // 端点0的最大数据包大小。
    uint16_t  idVendor;            // USB标准化组织指定的供应商ID。
    uint16_t  idProduct;           // USB标准化组织指定的产品ID。
    uint16_t  bcdDevice;           // 设备版本号。
    uint8_t   iManufacturer;       // 描述制造商的字符串描述符的索引。
    uint8_t   iProduct;            // 描述产品的字符串描述符的索引。
    uint8_t   iSerialNumber;       // 包含设备序列号的字符串描述符的索引。
    uint8_t   bNumConfigurations;  // 可能的配置数量。
};

int libusb_get_active_config_descriptor(libusb_device *dev, struct libusb_config_descriptor **config);
    获取设备的当前正在使用的配置描述符。
    通过 **config 返回配置描述符，通过 libusb_free_config_descriptor() 释放配置描述符。
    如果设备处于未配置状态，返回 LIBUSB_ERROR_NOT_FOUND 。

int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config);
    根据索引获取设备的配置描述符。
    通过 **config 返回配置描述符，通过 libusb_free_config_descriptor() 释放配置描述符。
    如果配置不存在，返回 LIBUSB_ERROR_NOT_FOUND 。

int libusb_get_config_descriptor_by_value(libusb_device *dev, uint8_t bConfigurationValue, struct libusb_config_descriptor **config);
    获取具有特定 bConfigurationValue 的配置描述符。
    通过 **config 返回配置描述符，通过 libusb_free_config_descriptor() 释放配置描述符。
    如果配置不存在，返回 LIBUSB_ERROR_NOT_FOUND 。

void libusb_free_config_descriptor(struct libusb_config_descriptor *config);
    释放这三个函数获取的配置描述符：
    libusb_get_config_descriptor()
    libusb_get_active_config_descriptor()
    libusb_get_config_descriptor_by_value()

配置描述符：
struct libusb_config_descriptor {
    uint8_t   bLength;             // 本描述符的字节数，恒等于9。
    uint8_t   bDescriptorType;     // 本描述符类型，恒等于0x02。
    uint16_t  wTotalLength;        // 本配置总大小。
    uint8_t   bNumInterfaces;      // 支持的接口数。
    uint8_t   bConfigurationValue; // 标识符值。
    uint8_t   iConfiguration;      // 描述此配置的字符串描述符的索引。
    uint8_t   bmAttributes;        // 配置特征。
    uint8_t   MaxPower;            // 最大电流。高速模式单位是2毫安，超高速模式单位是8毫安。
    
    const struct libusb_interface *interface;    // 支持的接口数组，数组的长度由 bNumInterfaces 字段确定。
    const unsigned char           *extra;        // 其他描述符。libusb将不认识的描述符储存在这里。
    int                            extra_length; // 其他描述符总长度。
};

接口:
struct libusb_interface {
    const struct libusb_interface_descriptor *altsetting;
    int num_altsetting;
};

接口描述符
struct libusb_interface_descriptor {
    uint8_t  bLength;            // 本描述符的字节数，恒等于9。
    uint8_t  bDescriptorType;    // 本描述符类型，恒等于0x04。
    uint8_t  bInterfaceNumber;   // 此接口的编号。
    uint8_t  bAlternateSetting;  // 此接口备用设置值。
    uint8_t  bNumEndpoints;      // 此接口使用的端点数(不包括控制端点)。
    uint8_t  bInterfaceClass;    // 此接口的类代码
    uint8_t  bInterfaceSubClass; // 此接口的子类代码，由 bInterfaceClass 值限定。
    uint8_t  bInterfaceProtocol; // 此接口的协议代码，由 bInterfaceClass 和 bInterfaceSubClass 值限定。
    uint8_t  iInterface;         // 描述此接口的字符串描述符的索引。
    
    const struct libusb_endpoint_descriptor *endpoint;     // 端点描述符数组，长度由 bNumEndpoints 字段确定。
    const unsigned char                     *extra;        // 其他描述符。libusb将不认识的描述符储存在这里。
    int                                      extra_length; // 其他描述符总长度。
};

端点描述符：
struct libusb_endpoint_descriptor {
    uint8_t   bLength;           // 本描述符的字节数，恒等于7。
    uint8_t   bDescriptorType;   // 本描述符类型，恒等于0x05。
    uint8_t   bEndpointAddress;  // 端点地址。
    uint8_t   bmAttributes;      // 使用 bConfigurationValue 配置端点时应用于该端点的属性。
    uint16_t  wMaxPacketSize;    // 此端点能够发送/接收的最大数据包大小。
    uint8_t   bInterval;         // 数据传输的间隔。
    
    uint8_t              bRefresh;      // 仅适用于音频设备：提供同步反馈的速率。
    uint8_t              bSynchAddress; // 仅适用于音频设备：同步端点的地址。
    const unsigned char *extra;         // 其他描述符。libusb将不认识的描述符储存在这里。
    int                  extra_length;  // 其他描述符总长度。
};

int libusb_open(libusb_device *dev, libusb_device_handle **dev_handle);
    打开一个设备用于 I/O 操作，通过 **dev_handle 返回设备打开后的句柄。
    该函数会增加 *dev 所指的USB设备的引用计数。
    返回值：
    LIBUSB_SUCCESS         成功
    LIBUSB_ERROR_NO_MEM    分配内存失败
    LIBUSB_ERROR_ACCESS    权限不足
    LIBUSB_ERROR_NO_DEVICE 没有这个设备

libusb_device_handle* libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vendor_id, uint16_t product_id);
    通过 VID+PID 打开一个设备用于 I/O 操作，返回设备打开后的句柄。
    该函数只打开第一个找到的设备，没找到或发生其他错误返回NULL。

void libusb_close(libusb_device_handle *dev_handle);
    关闭一个设备句柄。该函数会减少句柄所关联的设备的引用计数。

libusb_device* libusb_get_device(libusb_device_handle *dev_handle);
    通过设备句柄返回设备实例。
    该函数不会修改返回的设备的引用计数。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libusb.h"

static char* getPortsString(libusb_device *dev){
    static char partsString[64] = {'r','o','o','t',0};
    
    uint8_t ports[16];
    int count = libusb_get_port_numbers(dev, ports, 16);
    if(count > 0){
        int i, offset = 4;
        for(i=0; i<count; i++){
            offset += sprintf(partsString+offset, "->%03u", ports[i]);
        }
    }
    return partsString;
}

#define MAXPOWER(speed, power) (((speed)>LIBUSB_SPEED_HIGH?8:2)*(power))

static void dumpdevice(libusb_context *libusbCtx, libusb_device *dev){
    static char* speedString[] = {
        "未知速度", 
        "低速(1500KBit/s)", 
        "全速(12000KBit/s)", 
        "高速(480000KBit/s)", 
        "超高速(5000000KBit/s)", 
        "超超速(10000000KBit/s)"
    };
    
    uint8_t bus   = libusb_get_bus_number(dev);
    uint8_t addr  = libusb_get_device_address(dev);
    int     speed = libusb_get_device_speed(dev);
    char   *path  = getPortsString(dev);
    printf("/dev/bus/usb/%03u/%03u %s %s\n", bus, addr, path, speedString[speed]);
    
    // 获取设备描述符
    struct libusb_device_descriptor deviceDescriptor;
    memset(&deviceDescriptor, 0, sizeof(struct libusb_device_descriptor));
    libusb_get_device_descriptor(dev, &deviceDescriptor);
    printf("    deviceDescriptor.bLength            = %u\n",   deviceDescriptor.bLength);
    printf("    deviceDescriptor.bDescriptorType    = %02X\n", deviceDescriptor.bDescriptorType);
    printf("    deviceDescriptor.bcdUSB             = %04X\n", deviceDescriptor.bcdUSB);
    printf("    deviceDescriptor.bDeviceClass       = %02X\n", deviceDescriptor.bDeviceClass);
    printf("    deviceDescriptor.bDeviceSubClass    = %02X\n", deviceDescriptor.bDeviceSubClass);
    printf("    deviceDescriptor.bDeviceProtocol    = %02X\n", deviceDescriptor.bDeviceProtocol);
    printf("    deviceDescriptor.bMaxPacketSize0    = %u\n",   deviceDescriptor.bMaxPacketSize0);
    printf("    deviceDescriptor.idVendor           = %04X\n", deviceDescriptor.idVendor);
    printf("    deviceDescriptor.idProduct          = %04X\n", deviceDescriptor.idProduct);
    printf("    deviceDescriptor.bcdDevice          = %04X\n", deviceDescriptor.bcdDevice);
    printf("    deviceDescriptor.bNumConfigurations = %u\n",   deviceDescriptor.bNumConfigurations);
    
    // 获取活跃的配置描述符的 bConfigurationValue
    uint8_t active_bConfigurationValue = 0;
    struct libusb_config_descriptor *configDescriptor = NULL;
    libusb_get_active_config_descriptor(dev, &configDescriptor);
    if(configDescriptor){
        active_bConfigurationValue = configDescriptor->bConfigurationValue;
        libusb_free_config_descriptor(configDescriptor);
        configDescriptor = NULL;
    }
    
    // 获取所有配置描述符
    uint8_t configIndex;
    for(configIndex=0; configIndex<deviceDescriptor.bNumConfigurations; configIndex++){
        configDescriptor = NULL;
        libusb_get_config_descriptor(dev, configIndex, &configDescriptor);
        if(configDescriptor){
            // 活跃的配置每个字段前面加一个*号
            if(configDescriptor->bConfigurationValue == active_bConfigurationValue){
                printf("      >*configDescriptor->bLength             = %u\n",       configDescriptor->bLength);
                printf("       *configDescriptor->bDescriptorType     = %02X\n",     configDescriptor->bDescriptorType);
                printf("       *configDescriptor->wTotalLength        = %u\n",       configDescriptor->wTotalLength);
                printf("       *configDescriptor->bNumInterfaces      = %u\n",       configDescriptor->bNumInterfaces);
                printf("       *configDescriptor->bConfigurationValue = %02X\n",     configDescriptor->bConfigurationValue);
                printf("       *configDescriptor->iConfiguration      = %u\n",       configDescriptor->iConfiguration);
                printf("       *configDescriptor->bmAttributes        = %02X\n",     configDescriptor->bmAttributes);
                printf("       *configDescriptor->MaxPower            = %u(%umA)\n", configDescriptor->MaxPower, MAXPOWER(speed,configDescriptor->MaxPower));
            }else{
                printf("      > configDescriptor->bLength             = %u\n",       configDescriptor->bLength);
                printf("        configDescriptor->bDescriptorType     = %02X\n",     configDescriptor->bDescriptorType);
                printf("        configDescriptor->wTotalLength        = %u\n",       configDescriptor->wTotalLength);
                printf("        configDescriptor->bNumInterfaces      = %u\n",       configDescriptor->bNumInterfaces);
                printf("        configDescriptor->bConfigurationValue = %02X\n",     configDescriptor->bConfigurationValue);
                printf("        configDescriptor->iConfiguration      = %u\n",       configDescriptor->iConfiguration);
                printf("        configDescriptor->bmAttributes        = %02X\n",     configDescriptor->bmAttributes);
                printf("        configDescriptor->MaxPower            = %u(%umA)\n", configDescriptor->MaxPower, MAXPOWER(speed,configDescriptor->MaxPower));
            }
            
            // 打印所有接口描述符
            int numAltsetting;
            int altsettingIndex;
            uint8_t endpointIndex;
            uint8_t interfaceIndex;
            struct libusb_endpoint_descriptor *endpointDescriptor;
            struct libusb_interface_descriptor *interfaceDescriptor;
            for(interfaceIndex=0; interfaceIndex<configDescriptor->bNumInterfaces; interfaceIndex++){
                numAltsetting = configDescriptor->interface[interfaceIndex].num_altsetting;
                printf("          > interface[%u]:numberOfAltsetting = %d\n", interfaceIndex, numAltsetting);
                for(altsettingIndex=0; altsettingIndex<numAltsetting; altsettingIndex++){
                    interfaceDescriptor = (void*)&(configDescriptor->interface[interfaceIndex].altsetting[altsettingIndex]);
                    printf("              > interfaceDescriptor->bLength            = %u\n",   interfaceDescriptor->bLength);
                    printf("                interfaceDescriptor->bDescriptorType    = %02X\n", interfaceDescriptor->bDescriptorType);
                    printf("                interfaceDescriptor->bInterfaceNumber   = %u\n",   interfaceDescriptor->bInterfaceNumber);
                    printf("                interfaceDescriptor->bAlternateSetting  = %u\n",   interfaceDescriptor->bAlternateSetting);
                    printf("                interfaceDescriptor->bNumEndpoints      = %u\n",   interfaceDescriptor->bNumEndpoints);
                    printf("                interfaceDescriptor->bInterfaceClass    = %02X\n", interfaceDescriptor->bInterfaceClass);
                    printf("                interfaceDescriptor->bInterfaceSubClass = %02X\n", interfaceDescriptor->bInterfaceSubClass);
                    printf("                interfaceDescriptor->bInterfaceProtocol = %02X\n", interfaceDescriptor->bInterfaceProtocol);
                    printf("                interfaceDescriptor->iInterface         = %u\n",   interfaceDescriptor->iInterface);
                    
                    // 打印所有端点描述符
                    if((interfaceDescriptor->bNumEndpoints) && (interfaceDescriptor->endpoint)){
                        for(endpointIndex=0; endpointIndex<interfaceDescriptor->bNumEndpoints; endpointIndex++){
                            endpointDescriptor = (void*)&(interfaceDescriptor->endpoint[endpointIndex]);
                            printf("                  > endpointDescriptor->bLength             = %u\n",   endpointDescriptor->bLength);
                            printf("                    endpointDescriptor->bDescriptorType     = %02X\n", endpointDescriptor->bDescriptorType);
                            printf("                    endpointDescriptor->bEndpointAddress    = %02X\n", endpointDescriptor->bEndpointAddress);
                            printf("                    endpointDescriptor->bmAttributes        = %02X\n", endpointDescriptor->bmAttributes);
                            printf("                    endpointDescriptor->wMaxPacketSize      = %u\n",   endpointDescriptor->wMaxPacketSize);
                            printf("                    endpointDescriptor->bInterval           = %u\n",   endpointDescriptor->bInterval);
                        }
                    }
                }
            }
            
            libusb_free_config_descriptor(configDescriptor);
            configDescriptor = NULL;
        }
    }
    
    libusb_device_handle *devHandle = NULL;
    libusb_open(dev, &devHandle);
    if(devHandle){
        libusb_close(devHandle);
    }
    putchar('\n');
}

int main(int argc, char* argv[]){
    libusb_context *libusbCtx = NULL;
    int errCode = libusb_init(&libusbCtx);
    if(errCode) return -1;
    
    // 枚举所有USB设备
    libusb_device **devList = NULL;
    ssize_t count = libusb_get_device_list(libusbCtx, &devList);
    if(count > 0){
        ssize_t i;
        for(i=0; i<count; i++){
            dumpdevice(libusbCtx, devList[i]);
        }
        libusb_free_device_list(devList, 1);
        devList = NULL;
    }
    
    libusb_exit(libusbCtx);
    return 0;
}


