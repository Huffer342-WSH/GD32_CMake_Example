# GD32 CMake example 

库包含子模块，请用clone下载或者到release下载


`git clone --recursive https://github.com/Huffer342-WSH/GD32_CMake_Example.git
`

一个串口收发简单例子。

[从零开始搭建工程](/doc//从零开始搭建工程.md)
## 使用到的工具

交叉编译器：[gcc-arm-none-eabi-10.3-2021.10-win32](https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.zip)

构建工具： [Ninja](https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip)

编辑器：vscode

调试接口连接： [openocd-xpack](https://github.com/xpack-dev-tools/openocd-xpack/releases)(第三方构建的OpenoCD)

调试器：cmsis-dap (淘宝购买的 猛龙DAP ，￥35拥有碾压野火DAP的性能,性能比肩jlink-v8)https://www.taobao.com/list/item/673370560794.htm

![Alt text](/doc/images/image-1.png)

单片机： GD32F310

## 编译与烧录

首先下载上文中提到的gcc-arm-none-eabi和openocd，构建器使用make或者ninja都可以，工程里的源文件多了以后ninja启动速度会更快一点。
准备好以上工具后将可执行文件所在目录的路径添加都PATH(Path)环境变量
然后在一下文件中修改路径
- .vscode\GD32_CMake_Example.code-workspace（必须改）
-  cmake\arm-none-eabi-gcc.cmake （可选，环境变量添加了就不用指定绝对路径了）
-  Core\CMakeLists.txt(该文件末尾将cmake install 指定成了使用openocd烧录)

### 命令行

进入工程文件夹目录中
``` 
#配置
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE:STRING=Debug  -G Ninja -S ./ -B ./build
#编译
cmake --build ./build --config Debug --target all --
#烧录
cmake --install ./build
```

### VSCode

1. 安装拓展ms-vscode.cmake-tools和marus25.cortex-debug
2. 把文件夹保存为工作区
3. 修改.workspace文件，在里面指定generator和cmake参数等内容
4. 修改相关文件里的路径
5. ctrl+p打开命令面板，CMake：Configure 配置
6. 点击vscode状态栏的build编译工程，或者命令面板里的CMake：Build
7. ctrl+p打开命令面板，选择CMake： Install烧录

![Alt text](/doc/images/image-2.png)

![Alt text](/doc/images/image-3.png)

## 注意事项

### 交叉编译工具链设置
cmake通过 -DCMAKE_TOOLCHAIN_FILE 参数指定工具链的配置文件。
cmake\arm-none-eabi-gcc.cmake文件中是交叉工具链的配置

### 链接脚本

链接器（Linker）负责将编译后的目标文件和所需的库文件合并在一起，生成最终的可执行文件或库文件。链接器脚本（Linker Script）是用来指导链接器在这个合并过程中如何排列和组织代码段、数据段、符号表等内容的文本文件。

GD32官方提供了IAR和Keil环境的支持包，但是没有适用于GNU工具链的链接脚本，但是我们可以参考同架构的STM32的了解脚本，通过STM32CubeIDE或者STM32CubeMX生产makefile工程，里面就有链接脚本。

链接脚本中需要包含**内存布局和分段**的定义，比如改工程中的链接脚本`gd32f3x0.ld`中

<details><summary> 点击展开 内存定义 </summary>
<p>


```
/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 8K
  FLASH    (rx)    : ORIGIN = 0x08000000,   LENGTH = 64K
}
```

它描述了存储器（内存）的布局和分配。这个链接脚本片段指定了两个不同的存储器段：RAM 和 FLASH。

1. **RAM：**
   - `ORIGIN = 0x20000000`：这是 RAM 段在内存中的起始地址。在这个例子中，RAM 的起始地址被设置为 `0x20000000`。
   - `LENGTH = 8K`：这是 RAM 段的长度。在这个例子中，RAM 的长度被设置为 8K（8192字节）。因此，RAM 的地址范围是从 `0x20000000` 到 `0x20001FFF`。
2. **FLASH：**
   - `ORIGIN = 0x08000000`：这是 FLASH 段在内存中的起始地址。在这个例子中，FLASH 的起始地址被设置为 `0x08000000`。
   - `LENGTH = 64K`：这是 FLASH 段的长度。在这个例子中，FLASH 的长度被设置为 64K（65536字节）。因此，FLASH 的地址范围是从 `0x08000000` 到 `0x0800FFFF`。

在这个链接脚本中，每个段都有一个或多个属性：

- **(xrw)**：这是段的属性，指定了段是否可执行（x，executable）、可读（r，readable）和可写（w，writable）。
- **(rx)**：同样是段的属性，这里指定了段是否可执行和可读，但不可写。

链接脚本的作用是告诉链接器如何分配和组织代码和数据在存储器中的位置。这是编译器和链接器合作的关键部分，确保生成的可执行文件在内存中正确加载和运行。在这个例子中，你的可执行程序的代码部分通常会被加载到 FLASH 存储器段，而数据（变量、堆栈等）则通常会被加载到 RAM 存储器段。

**RAM**和**FLASH**的地址和大小可以从用户手册中找到，如 GD32F3x0_User_Manual_CN_Rev2.7.pdf>1.系统及存储器架构>1.3存储器映射


![image-20230824143008529](doc\images\image-20230824143008529.png)

</p>
</details>

<details><summary> 点击展开 段定义 </summary>
<p>


```
SECTIONS{
  ...
}
需要包含以下部分

.isr_vector：
这部分定义了启动代码，其中包含中断向量表（Interrupt Vector Table）。中断向量表存储了各种中断和异常的处理函数的入口地址。.isr_vector 节在 FLASH 存储器段中。

.text：
这是程序的代码段，包括执行代码和一些初始化代码。在这个部分，.text 节存储了程序的代码，.glue_7 和 .glue_7t 是 ARM 和 Thumb 之间的粘合代码，.eh_frame 是与异常处理有关的信息。

.rodata：
这是只读数据段，用于存储常量和字符串等只读数据。

.ARM.extab：
这部分存储了与 ARM 异常处理相关的信息，可能包括异常类型、处理代码等。

.ARM：
这部分可能与异常处理索引表有关，存储了异常处理的索引信息。

.preinit_array、.init_array、.fini_array：
这些部分存储了初始化和终止函数的指针数组，用于在程序启动和退出时执行特定操作。

.data：
这是已初始化数据段，包括全局和静态变量的初始化数据。

.bss：
这是未初始化数据段，包括全局和静态变量的未初始化数据。这些变量在程序启动时会被自动初始化为零。

._user_heap_stack：
这部分用于定义堆和栈的位置，确保在分配内存时有足够的空间。

/DISCARD/：
这是用于从编译器库中丢弃不需要的部分的部分，以减小生成的可执行文件的大小。

.ARM.attributes：
这是用于存储与 ARM 架构有关的属性信息，例如硬件特性和指令集等。
```
</p>
</details>





### 启动文件

启动文件也是一段普通的代码。cortex-M系列的处理器在启动后会在内存中固定位置（0x00000000）找中断向量表，中断向量表中包含了各种中断和异常的初始处理函数的入口地址，当触发中断后MCU就回到向量表中找到函数入口地址，然后调用对应的中断服务函数。

在启动文件startup_gd32f3x0.s中，包含了复位服务函数Reset_Handler以及中断向量表，这个文件一般用汇编编写，但也有使用c语言编写的。
汇编语言中通过.section语句可以指定数据段或者代码段的存储位置。在链接文件中我们定义了.isr_vector段，位于FLASH中最开头的位置，在启动文件中通过.section	.isr_vector,"a",%progbits将向量表指定到这个段，从而实现了将向量表链接到我们想要的位置。

向量表中的内容由单片机厂商决定，可以在用户手册中找到向量地址的定义，比如我使用的GD32F3x0，可以在```GD32F3x0_User_Manual_CN_Rev2.7.pdf>6.中断/事件控制器（EXTI）> 6.3功能描述``` 中找到向量表的描述。在官方提供的固件库中也有适用于Keil和IAR的启动文件，在路径```Drivers\CMSIS\GD\GD32F3x0\Source```下，但是我们要使用的是GNU工具链汇编风格的启动文件，可以依据这两个文件修改得到


## 从零开始搭建工程


[从零开始搭建工程](/doc//从零开始搭建工程.md)