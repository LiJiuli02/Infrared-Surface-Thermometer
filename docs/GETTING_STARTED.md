# Getting started and build recovery / 搭建与工程恢复

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## What you need / 所需条件

**EN:** Identify the actual MSP430 board and SI-111 sensor/front-end assembly. The family headers and archived `__MSP430F249__` definition suggest an F249-based historical build; confirm the physical chip before creating a project. Obtain the analog schematic, sensor calibration information, display latch wiring, power requirements, DS1302 module details and an appropriate programmer.

**中文：** 先确认实际 MSP430 开发板及 SI-111 传感器/前端组件。系列头文件和历史 `__MSP430F249__` 定义指向 F249 构建，但创建工程前仍需核对实际芯片。应取得模拟电路、传感器标定信息、数码管锁存连接、供电要求、DS1302 模块资料及兼容下载器。

**EN:** Historical compiler metadata under `Debug/Obj/` references IAR Embedded Workbench 6.4. The same metadata reports IAR C/C++ Compiler V5.50.1.20465 for MSP430 (Evaluation edition). These are recovery clues; the full IDE/linker configuration is not preserved. No `.ewp` or `.eww` file is present in the reviewed tree.

**中文：** `Debug/Obj/` 下的历史编译元数据包含 IAR Embedded Workbench 6.4 路径，同一元数据记录 IAR C/C++ Compiler V5.50.1.20465 for MSP430（Evaluation edition），可作为恢复线索，但完整 IDE 与链接配置未保留。核查目录中没有 `.ewp` 或 `.eww` 文件。

## Source-derived connections / 源码连接表

These describe software assignments, not a complete schematic. / 以下仅为软件引脚分配，不是完整电路图。

| Interface / 接口 | Pin / 引脚 | Source / 来源 |
| --- | --- | --- |
| Resistance/body-temperature channel / 电阻与本体温度通道 | P6.0 / A0 | `ADC_Func.c`, `main.c` |
| Conditioned target-signal channel / 调理后的目标信号 | P6.1 / A1 | `ADC_Func.c`, `main.c` |
| UART TX / 串口发送 | P3.4 | `UART0.c` |
| UART RX / 串口接收引脚复用 | P3.5 | `UART0.c`; receive handler is disabled / 接收中断未启用 |
| DS1302 reset/enable / 复位与使能 | P2.7 | `ds1302.h` |
| DS1302 clock / 时钟 | P2.5 | `ds1302.h` |
| DS1302 data / 数据 | P2.6 | `ds1302.h` |
| Display data / 数码管数据 | P4.0–P4.7 | `main.c` |
| Segment latch / 段锁存 | P5.5 | `main.c` |
| Digit latch / 位锁存 | P5.6 | `main.c` |

## Recover a build / 恢复构建

1. **EN:** Create a new IAR MSP430 C project for the verified device, or restore the original project if available. Add `main.c`, `ADC_Func.c`, `DataTrans.c`, `UART0.c` and `ds1302.c`.
   **中文：** 为核实过的器件新建 IAR MSP430 C 工程，或恢复原始工程，加入上述五个 C 文件。
2. **EN:** Add the repository root to include paths. Retain the source's interrupt-vector pragmas and compiler intrinsics. Configure the device linker file and runtime/math support; `DataTrans.c` calls `log`.
   **中文：** 将仓库根目录加入头文件路径，保留中断向量声明和编译器内建函数，设置器件链接文件及运行库/数学库。
3. **EN:** Confirm an 8 MHz XT2 source. `ClkInit` selects XT2 for MCLK and SMCLK without the divider described in one comment; `uart0.h` assumes 8 MHz for delay macros. Inspect the startup loop and warnings rather than relying on comments alone.
   **中文：** 确认 XT2 为 8 MHz。实际代码为 MCLK 和 SMCLK 选择 XT2，没有实现某条注释所述的分频；延时宏同样假设 8 MHz。应核对启动循环及告警。
4. **EN:** Resolve the reference-voltage and arithmetic questions in the validation guide before treating output as temperature data. Build from source, retain logs, then flash the verified board.
   **中文：** 将读数用于测温前，先解决验证文档中的参考电压与数值问题；从源码构建、保存日志，再烧录到已确认的板卡。
5. **EN:** Start with known, in-range electrical inputs and check ADC values before connecting the complete sensor assembly. Pin assignments do not establish allowable voltages.
   **中文：** 先使用已知且在允许范围内的电信号检查 ADC，再接入完整传感器组件。引脚表不能代替电压范围说明。

## Serial operation / 串口操作

**EN:** Use a compatible logic-level USB–UART adapter with common ground and open the correct port at nominal **9600 baud, 8 data bits, no parity, 1 stop bit**. The source selects UART pins but does not implement an active receive command interface.

**中文：** 使用电平兼容且共地的 USB 转串口模块，按标称 **9600 波特率、8 数据位、无校验、1 停止位** 打开端口。源码配置了接收引脚，但未实现启用的串口命令接口。

Illustrative output format, not a captured result / 格式示例，非实测结果：

~~~text
2026-11-04 12:34:56 25.00
~~~

**EN:** Each line is date/time, a space, temperature with two decimals, a trailing space and CRLF. At every boot, `Set_DS1302(wdata)` writes the hard-coded date/time in `main.c`; the clock will not retain a user-set time across firmware restarts unless that startup behavior is changed.

**中文：** 每行包含日期时间、空格、两位小数温度、尾部空格及 CRLF 换行。每次启动均调用 `Set_DS1302(wdata)` 写入固定时间；除非修改此行为，否则固件重启会覆盖用户设置的时钟。

## Troubleshooting / 常见问题

| Symptom / 现象 | Check / 排查 |
| --- | --- |
| Startup stalls / 启动停滞 | XT2 crystal, oscillator fault flag, loop-counter type / 晶振、故障标志及循环计数类型 |
| Garbled serial output / 串口乱码 | Actual SMCLK, baud divisor, port setting, ground / 实际时钟、分频参数、串口设置和共地 |
| Implausible temperature / 温度异常 | ADC references, gain/offset, resistor values, input bounds / ADC 参考、增益与偏置、电阻和输入范围 |
| Display flicker / 显示闪烁 | Latch wiring, scan timing, expensive ADC interrupt calculations / 锁存接线、扫描时序及 ADC 中断耗时 |
| Time resets / 时间重置 | Startup writes fixed `wdata` / 启动覆盖固定时间 |
