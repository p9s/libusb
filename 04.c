/*
先检查是否支持热插拔。
    用 LIBUSB_CAP_HAS_HOTPLUG 参数来调用 libusb_has_capability() 以确认是否支持热插拔。
    libusb_context *libusbCtx = NULL;
    libusb_init(&libusbCtx);
    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)){
        printf("支持热插拔\n");
    }
    libusb_exit(libusbCtx);

int libusb_hotplug_register_callback(libusb_context *ctx, ...);
    注册一个热插拔回调函数。

int libusb_hotplug_register_callback(
    libusb_context *ctx,        // libusb环境上下文
    int             events,     // 触发此回调的热插拔事件
    int             flags,      // 热插拔标志
    int             vendor_id,  // 要匹配的VID，  或设置为 LIBUSB_HOTPLUG_MATCH_ANY 来匹配所有。
    int             product_id, // 要匹配的PID，  或设置为 LIBUSB_HOTPLUG_MATCH_ANY 来匹配所有。
    int             dev_class,  // 要匹配的class，或设置为 LIBUSB_HOTPLUG_MATCH_ANY 来匹配所有。
    libusb_hotplug_callback_fn      cb_fn,          // 回调函数
    void                           *user_data,      // 传输给回调函数的用户数据。
    libusb_hotplug_callback_handle *callback_handle // 用于返回回调句柄。
);

// 触发回调的事件
enum libusb_hotplug_event{
    LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED = (1<<0), // 设备已插入，可以使用。
    LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT    = (1<<1)  // 设备已离开，不再可用。
}

// 热插拔标志
enum libusb_hotplug_flag{
    LIBUSB_HOTPLUG_ENUMERATE = (1<<0) // 枚举所有已插入的设备，若匹配也触发回调。
}

// 匹配标志
#define LIBUSB_HOTPLUG_NO_FLAGS   0 // 不使用任何标志
#define LIBUSB_HOTPLUG_MATCH_ANY -1 // 匹配所有

// 回调函数
typedef int (*libusb_hotplug_callback_fn)(
    libusb_context       *ctx,      // libusb环境上下文
    libusb_device        *device,   // 发生事件的设备
    libusb_hotplug_event  event,    // 插入或拔出事件
    void                 *user_data // 用户数据。
);

void libusb_hotplug_deregister_callback(libusb_context *ctx, libusb_hotplug_callback_handle callback_handle);
    取消热插拔回调的注册。

void* libusb_hotplug_get_user_data(libusb_context *ctx, libusb_hotplug_callback_handle callback_handle);    
    获取与热插拔回调关联的用户数据。
    用户数据就是 *user_data 参数所指数据。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libusb.h"

static char* getLocalTimeString(void){
    static char buffer[32];
    struct tm localDate;
    time_t second = time(NULL);
    localtime_r(&second, &localDate);
    strftime(buffer, 30, "<%H:%M:%S>", &localDate);
    return buffer;
}

// 热插拔回调函数
static int hotplugCallback(
    libusb_context       *libusbCtx, 
    libusb_device        *dev, 
    libusb_hotplug_event  event, 
    void                 *userData
){
    uint8_t bus = libusb_get_bus_number(dev);
    uint8_t addr = libusb_get_device_address(dev);
    struct libusb_device_descriptor deviceDescriptor;
    memset(&deviceDescriptor, 0, sizeof(struct libusb_device_descriptor));
    libusb_get_device_descriptor(dev, &deviceDescriptor);
    
    // '+'表示插入，'-'表示拔出。
    printf("%s %c /dev/bus/usb/%03u/%03u %04x:%04x usb%u.%u class:%02x\n", 
        getLocalTimeString(), 
        (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED ? '+' : '-'),
        bus, addr, 
        deviceDescriptor.idVendor, 
        deviceDescriptor.idProduct, 
        (deviceDescriptor.bcdUSB & 0x0f00) >> 8, 
        (deviceDescriptor.bcdUSB & 0x00f0) >> 4, 
        deviceDescriptor.bDeviceClass
    );
    
    return 0;
}

int main(int argc, char* argv[]){
    // 设置等待事件的次数
    int eventCount = 30;
    if(argc >= 2){
        eventCount = atoi(argv[1]);
        if(eventCount >= 10000) eventCount = 10000;
        else if(eventCount < 1) eventCount = 1;
    }
    
    // 初始化libusb环境上下文
    libusb_context *libusbCtx = NULL;
    int errCode = libusb_init(&libusbCtx);
    if(errCode) return -1;
    
    // 检查是否支持热插拔
    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)){
        printf("支持热插拔\n");
    }else{
        printf("不支持热插拔\n");
        libusb_exit(libusbCtx);
        return -2;
    }
    
    // 注册热插拔回调函数
    libusb_hotplug_callback_handle callbackHandle = 0;
    errCode = libusb_hotplug_register_callback(
        libusbCtx,                            // 环境上下文
        LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | // 插入事件
        LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,     // 拔出事件
        LIBUSB_HOTPLUG_ENUMERATE,             // 枚举已插入的设备
        LIBUSB_HOTPLUG_MATCH_ANY,             // 匹配所有VID
        LIBUSB_HOTPLUG_MATCH_ANY,             // 匹配所有PID
        LIBUSB_HOTPLUG_MATCH_ANY,             // 匹配所有CLass
        hotplugCallback,                      // 回调函数
        NULL,                                 // 调用回调函数时传递给回调函数的用户数据
        &callbackHandle                       // 返回回调句柄
    );
    if(errCode){
        printf("注册热插拔回调函数失败\n");
        libusb_exit(libusbCtx);
        return -3;
    }
    
    // 等待事件?
    while(eventCount-- > 0){
        libusb_handle_events_completed(libusbCtx, NULL);
    }
    
    // 取消热插拔回调
    libusb_hotplug_deregister_callback(libusbCtx, callbackHandle);
    libusb_exit(libusbCtx);
    return 0;
}

