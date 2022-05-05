/*
所需头文件是 libusb.h，可能位于 "/usr/include/libusb-1.0" 目录中。
所需动态库可能是 libusb-1.0.so，那么编译选项加 -lusb-1.0。
所需动态库可能位于 "/usr/lib/x86_64-linux-gnu" 这样的目录中。

int libusb_init(libusb_context **context);
    用来初始化 libusb 环境上下文，通过 **context 返回环境上下文指针。
    如果参数 **context 为 NULL 会初始化默认的环境上下文。
    成功返回 LIBUSB_SUCCESS，失败返回其他错误码。

错误码列表：
    LIBUSB_SUCCESS                0  成功
    LIBUSB_ERROR_IO              -1  输入输出错误
    LIBUSB_ERROR_INVALID_PARAM   -2  无效参数
    LIBUSB_ERROR_ACCESS          -3  访问被拒绝(权限不足)
    LIBUSB_ERROR_NO_DEVICE       -4  没有这个设备
    LIBUSB_ERROR_NOT_FOUND       -5  找不到实体
    LIBUSB_ERROR_BUSY            -6  资源繁忙
    LIBUSB_ERROR_TIMEOUT         -7  操作超时
    LIBUSB_ERROR_OVERFLOW        -8  溢出
    LIBUSB_ERROR_PIPE            -9  管道错误  
    LIBUSB_ERROR_INTERRUPTED    -10  系统调用中断(可能是由于信号原因)
    LIBUSB_ERROR_NO_MEM         -11  内存不足
    LIBUSB_ERROR_NOT_SUPPORTED  -12  此平台不支持或未实现操作
    LIBUSB_ERROR_OTHER          -99  其他错误

void libusb_exit(libusb_context *context);
    取消 libusb 环境上下文的初始化。
    如果 *context 为 NULL 则取消默认环境上下文的初始化。

const char* libusb_error_name(int errcode);
    返回错误码对应的名字，例如错误码是 LIBUSB_ERROR_ACCESS 就返回 "LIBUSB_ERROR_ACCESS"。
    返回的是常量字符串，一定不要free()，其内容一定不要修改。

const char* libusb_strerror(int errcode);
    返回错误代码的简短描述，此描述是UTF-8编码，其语言由 libusb_setlocale() 设置。
    返回的是常量字符串，一定不要free()，其内容一定不要修改。

int libusb_setlocale(const char *locale);
    设置消息语言，默认英语。*locale 使用 ISO 639-1 标准的语言代码。
    返回值：
    LIBUSB_SUCCESS              成功
    LIBUSB_ERROR_INVALID_PARAM  区域设置不符合要求
    LIBUSB_ERROR_NOT_FOUND      请求的语言不受支持

int libusb_has_capability(uint32_t capability);
    在运行时检查加载的库是否具有给定的功能。
    此调用应在 libusb_init() 之后执行。
    返回0表示不支持，非0表示支持。
    capability值列表：
    LIBUSB_CAP_HAS_CAPABILITY                 0x0000U  libusb_has_capability()API可用。
    LIBUSB_CAP_HAS_HOTPLUG                    0x0001U  此平台上提供热插拔支持。
    LIBUSB_CAP_HAS_HID_ACCESS                 0x0100U  库可以访问HID设备，而无需用户干预。
    LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER  0x0101U  如果操作系统内核设置了默认USB驱动程序，则库支持使用
                                                       libusb_detach_kernel_driver()分离默认USB驱动程序。

const struct libusb_version *libusb_get_version(void);
    返回libusb的版本信息。返回的指针一定不要free()，其内容一定不要修改。
    struct libusb_version {
        const uint16_t major;    // 主版本号
        const uint16_t minor;    // 副版本号
        const uint16_t micro;    // 微版本号？
        const uint16_t nano;     // 纳版本号？
        const char    *rc;       // 
        const char    *describe; // 
    };
*/

#include <stdio.h>
#include <stdlib.h>
#include "libusb.h"

static char* languages[] = {
    "aa", "阿法尔语",
    "fr", "法语",
    "zh", "汉语",
    "li", "林堡语",
    "se", "北萨米语",
    "ab", "阿布哈兹语",
    "fy", "弗里西亚语",
    "ln", "林加拉语",
    "sg", "桑戈语",
    "ae", "阿维斯陀语",
    "ga", "爱尔兰语",
    "lo", "老挝语",
    "sh", "塞尔维亚-克罗地亚语",
    "af", "阿非利堪斯语",
    "gd", "苏格兰盖尔语",
    "lt", "立陶宛语",
    "si", "僧加罗语",
    "ak", "阿坎语",
    "gl", "加利西亚语",
    "lu", "卢巴-加丹加语",
    "sk", "斯洛伐克语",
    "am", "阿姆哈拉语",
    "gn", "瓜拉尼语",
    "lv", "拉脱维亚语",
    "sl", "斯洛文尼亚语",
    "an", "阿拉贡语",
    "gu", "古吉拉特语",
    "mg", "马达加斯加语",
    "sm", "萨摩亚语",
    "ar", "阿拉伯语",
    "gv", "马恩岛语",
    "mh", "马绍尔语",
    "sn", "绍纳语",
    "as", "阿萨姆语",
    "ha", "豪萨语",
    "mi", "毛利语",
    "so", "索马里语",
    "av", "阿瓦尔语",
    "he", "希伯来语",
    "mk", "马其顿语",
    "sq", "阿尔巴尼亚语",
    "ay", "艾马拉语",
    "hi", "印地语",
    "ml", "马拉亚拉姆语",
    "sr", "塞尔维亚语",
    "az", "阿塞拜疆语",
    "ho", "希里莫图语",
    "mn", "蒙古语",
    "ss", "斯瓦特语",
    "ba", "巴什基尔语",
    "hr", "克罗地亚语",
    "mo", "摩尔达维亚语",
    "st", "南索托语",
    "be", "白俄罗斯语",
    "ht", "海地克里奥尔语",
    "mr", "马拉提语",
    "su", "巽他语",
    "bg", "保加利亚语",
    "hu", "匈牙利语",
    "ms", "马来语",
    "sv", "瑞典语",
    "bh", "比哈尔语",
    "hy", "亚美尼亚语",
    "mt", "马耳他语",
    "sw", "斯瓦希里语",
    "bi", "比斯拉玛语",
    "hz", "赫雷罗语",
    "my", "缅甸语",
    "ta", "泰米尔语",
    "bm", "班巴拉语",
    "ia", "国际语A",
    "na", "瑙鲁语",
    "te", "泰卢固语",
    "bn", "孟加拉语",
    "id", "印尼语",
    "nb", "书面挪威语",
    "tg", "塔吉克语",
    "bo", "藏语",
    "ie", "国际语E",
    "nd", "北恩德贝勒语",
    "th", "泰语",
    "br", "布列塔尼语",
    "ig", "伊博语",
    "ne", "尼泊尔语",
    "ti", "提格里尼亚语",
    "bs", "波斯尼亚语",
    "ii", "四川彝语",
    "ng", "恩敦加语",
    "tk", "土库曼语",
    "ca", "加泰隆语",
    "ik", "依努庇克语",
    "nl", "荷兰语",
    "tl", "塔加洛语",
    "ce", "车臣语",
    "io", "伊多语",
    "nn", "新挪威语",
    "tn", "塞茨瓦纳语",
    "ch", "查莫罗语",
    "is", "冰岛语",
    "no", "挪威语",
    "to", "汤加语",
    "co", "科西嘉语",
    "it", "意大利语",
    "nr", "南恩德贝勒语",
    "tr", "土耳其语",
    "cr", "克里语",
    "iu", "伊努伊特语",
    "nv", "纳瓦霍语",
    "ts", "宗加语",
    "cs", "捷克语",
    "ja", "日语",
    "ny", "尼扬贾语",
    "tt", "塔塔尔语",
    "cu", "教会斯拉夫语",
    "jv", "爪哇语",
    "oc", "奥克语",
    "tw", "特威语",
    "cv", "楚瓦什语",
    "ka", "格鲁吉亚语",
    "oj", "奥吉布瓦语",
    "ty", "塔希提语",
    "cy", "威尔士语",
    "kg", "刚果语",
    "om", "奥洛莫语",
    "ug", "维吾尔语",
    "da", "丹麦语",
    "ki", "基库尤语",
    "or", "奥利亚语",
    "uk", "乌克兰语",
    "de", "德语",
    "kj", "宽亚玛语",
    "os", "奥塞梯语",
    "ur", "乌尔都语",
    "dv", "迪维希语",
    "kk", "哈萨克语",
    "pa", "旁遮普语",
    "uz", "乌兹别克语",
    "dz", "不丹语",
    "kl", "格陵兰语",
    "pi", "巴利语",
    "ve", "文达语",
    "ee", "埃维语",
    "km", "高棉语",
    "pl", "波兰语",
    "vi", "越南语",
    "el", "现代希腊语",
    "kn", "卡纳达语",
    "ps", "普什图语",
    "vo", "沃拉普克语",
    "en", "英语",
    "ko", "朝鲜语、韩语",
    "pt", "葡萄牙语",
    "wa", "沃伦语",
    "eo", "世界语",
    "kr", "卡努里语",
    "qu", "凯楚亚语",
    "wo", "沃洛夫语",
    "es", "西班牙语",
    "ks", "克什米尔语",
    "rm", "利托-罗曼语",
    "xh", "科萨语",
    "et", "爱沙尼亚语",
    "ku", "库尔德语",
    "rn", "基隆迪语",
    "yi", "依地语",
    "eu", "巴斯克语",
    "kv", "科米语",
    "ro", "罗马尼亚语",
    "yo", "约鲁巴语",
    "fa", "波斯语",
    "kw", "康沃尔语",
    "ru", "俄语",
    "za", "壮语",
    "ff", "富拉语",
    "ky", "吉尔吉斯语",
    "rw", "基尼阿万达语",
    "fi", "芬兰语",
    "la", "拉丁语",
    "sa", "梵语",
    "zu", "祖鲁语",
    "fj", "斐济语",
    "lb", "卢森堡语",
    "sc", "撒丁语",
    "fo", "法罗语",
    "lg", "干达语",
    "sd", "信德语",
    "io", "伊多语",
    "wa", "瓦龙语",
    "li", "林堡语",
    "ii", "四川彝文",
    "an", "阿拉贡语",
    "ht", "海地克里奥耳语"
};

int main(int argc, char* argv[]){
    // 打印错误码名字。
    putchar('\n');
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_SUCCESS,             libusb_error_name(LIBUSB_SUCCESS));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_IO,            libusb_error_name(LIBUSB_ERROR_IO));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_INVALID_PARAM, libusb_error_name(LIBUSB_ERROR_INVALID_PARAM));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_ACCESS,        libusb_error_name(LIBUSB_ERROR_ACCESS));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_NO_DEVICE,     libusb_error_name(LIBUSB_ERROR_NO_DEVICE));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_NOT_FOUND,     libusb_error_name(LIBUSB_ERROR_NOT_FOUND));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_BUSY,          libusb_error_name(LIBUSB_ERROR_BUSY));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_TIMEOUT,       libusb_error_name(LIBUSB_ERROR_TIMEOUT));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_OVERFLOW,      libusb_error_name(LIBUSB_ERROR_OVERFLOW));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_PIPE,          libusb_error_name(LIBUSB_ERROR_PIPE));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_INTERRUPTED,   libusb_error_name(LIBUSB_ERROR_INTERRUPTED));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_NO_MEM,        libusb_error_name(LIBUSB_ERROR_NO_MEM));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_NOT_SUPPORTED, libusb_error_name(LIBUSB_ERROR_NOT_SUPPORTED));
    printf("errCode=%3d, errName=\"%s\"\n", LIBUSB_ERROR_OTHER,         libusb_error_name(LIBUSB_ERROR_OTHER));
    putchar('\n');
    
    // 打印错误码描述，语言默认为英语。
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_SUCCESS,             libusb_strerror(LIBUSB_SUCCESS));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_IO,            libusb_strerror(LIBUSB_ERROR_IO));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_INVALID_PARAM, libusb_strerror(LIBUSB_ERROR_INVALID_PARAM));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_ACCESS,        libusb_strerror(LIBUSB_ERROR_ACCESS));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NO_DEVICE,     libusb_strerror(LIBUSB_ERROR_NO_DEVICE));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NOT_FOUND,     libusb_strerror(LIBUSB_ERROR_NOT_FOUND));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_BUSY,          libusb_strerror(LIBUSB_ERROR_BUSY));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_TIMEOUT,       libusb_strerror(LIBUSB_ERROR_TIMEOUT));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_OVERFLOW,      libusb_strerror(LIBUSB_ERROR_OVERFLOW));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_PIPE,          libusb_strerror(LIBUSB_ERROR_PIPE));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_INTERRUPTED,   libusb_strerror(LIBUSB_ERROR_INTERRUPTED));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NO_MEM,        libusb_strerror(LIBUSB_ERROR_NO_MEM));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NOT_SUPPORTED, libusb_strerror(LIBUSB_ERROR_NOT_SUPPORTED));
    printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_OTHER,         libusb_strerror(LIBUSB_ERROR_OTHER));
    putchar('\n');
    
    // 打印错误码描述，语言设置为其他语言。
    // 本次实测结果仅支持 en-英语、fr-法语、nl-荷兰语、ru-俄语。
    int i, errCode;
    int number = sizeof(languages)/sizeof(char*);
    for(i=0; i<number; i+=2){
        errCode = libusb_setlocale(languages[i]);
        if(errCode == LIBUSB_SUCCESS){
            printf("libusb_setlocale(%s,%s): errCode=%3d, errName=\"%s\"\n", languages[i], languages[i+1], errCode, libusb_error_name(errCode));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_SUCCESS,             libusb_strerror(LIBUSB_SUCCESS));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_IO,            libusb_strerror(LIBUSB_ERROR_IO));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_INVALID_PARAM, libusb_strerror(LIBUSB_ERROR_INVALID_PARAM));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_ACCESS,        libusb_strerror(LIBUSB_ERROR_ACCESS));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NO_DEVICE,     libusb_strerror(LIBUSB_ERROR_NO_DEVICE));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NOT_FOUND,     libusb_strerror(LIBUSB_ERROR_NOT_FOUND));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_BUSY,          libusb_strerror(LIBUSB_ERROR_BUSY));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_TIMEOUT,       libusb_strerror(LIBUSB_ERROR_TIMEOUT));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_OVERFLOW,      libusb_strerror(LIBUSB_ERROR_OVERFLOW));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_PIPE,          libusb_strerror(LIBUSB_ERROR_PIPE));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_INTERRUPTED,   libusb_strerror(LIBUSB_ERROR_INTERRUPTED));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NO_MEM,        libusb_strerror(LIBUSB_ERROR_NO_MEM));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_NOT_SUPPORTED, libusb_strerror(LIBUSB_ERROR_NOT_SUPPORTED));
            printf("errCode=%3d, errString=\"%s\"\n", LIBUSB_ERROR_OTHER,         libusb_strerror(LIBUSB_ERROR_OTHER));
            putchar('\n');
        }
    }
    
    // 初始化libusb环境上下文
    libusb_context *libusbCtx = NULL;
    errCode = libusb_init(&libusbCtx);
    if(errCode) return -1;
    
    // 检查此平台的libusb支持的功能
    if(libusb_has_capability(LIBUSB_CAP_HAS_CAPABILITY)){
        printf("支持 LIBUSB_CAP_HAS_CAPABILITY \n");
    }
    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)){
        printf("支持 LIBUSB_CAP_HAS_HOTPLUG \n");
    }
    if(libusb_has_capability(LIBUSB_CAP_HAS_HID_ACCESS)){
        printf("支持 LIBUSB_CAP_HAS_HID_ACCESS \n");
    }
    if(libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER)){
        printf("支持 LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER \n");
    }
    
    // 打印libusb版本号
    const struct libusb_version *libusbVersion = libusb_get_version();
    if(libusbVersion){
        printf("libusbVersion = %u.%u.%u\n", libusbVersion->major, libusbVersion->minor, libusbVersion->micro);
    }
    
    // 取消libusb环境上下文的初始化
    libusb_exit(libusbCtx);
    return 0;
}
	
