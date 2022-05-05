// 设备描述符
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

// 配置描述符：
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

// 接口:
struct libusb_interface {
    const struct libusb_interface_descriptor *altsetting;
    int num_altsetting;
};

// 接口描述符：
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

// 端点描述符：
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

