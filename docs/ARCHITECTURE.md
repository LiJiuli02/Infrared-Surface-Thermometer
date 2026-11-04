# Architecture and conversion model / 系统架构与换算模型

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

~~~mermaid
flowchart LR
    A["A0 / body-temperature channel 本体温度"] --> C["ADC12 / average 10 samples 十次平均"]
    B["A1 / conditioned target signal 目标信号"] --> C
    C --> D["Rt → Tk → Tt / 温度换算"]
    D --> E["5-position display / 五位数码管"]
    D --> F["UART / 温度文本"]
    G["DS1302 / 日期时间"] --> F
~~~

## Execution flow / 执行流程

**EN:** `main.c` configures ports, enables the watchdog interval interrupt, starts clocks/UART/ADC/RTC, writes a fixed RTC time, then repeatedly reads the clock and prints a temperature line. The ADC12 interrupt averages ten samples per channel and performs conversion. The watchdog interrupt scans one display position at a time; the comment describes a nominal 1.9 ms interrupt interval, but actual timing needs verification.

**中文：** `main.c` 设置端口、启用看门狗定时间隔中断、初始化时钟/串口/ADC/RTC、写入固定时钟，然后循环读取时间并输出温度。ADC12 中断对每通道十次采样求平均并换算温度，看门狗中断逐位扫描数码管。注释给出标称 1.9 ms 中断间隔，实际时序仍需验证。

## Conversion equations / 换算公式

The following restates the intended arithmetic in [DataTrans.c](../DataTrans.c), not an independently validated sensor model.

以下根据代码整理其预期计算含义，不代表经独立验证的传感器模型。

Let `c0` and `c1` be the averaged ADC counts. / 设 `c0` 与 `c1` 为平均 ADC 码值。

~~~text
Rt = 24900 × (4096 / c0 − 1)

Tk ≈ 100 / (1.129241e−3
             + 2.341077e−4 × ln(Rt)
             + 8.775468e−8 × ln(Rt)^3)

signal_uV = 1000 × (3300 × c1 / 4096 − 1980) / 275
Tt ≈ 100 × (signal_uV / 60) + Tk − 27315
~~~

**EN:** `Tk` represents body temperature in kelvin multiplied by 100; `Tt` represents target temperature in degrees Celsius multiplied by 100. The source uses scaled constants, integer storage and intermediate truncation, so the equations above do not guarantee bit-for-bit results. Gain 275, lift 1980 mV and sensitivity divisor 60 are code constants, not measured values established by this review.

**中文：** `Tk` 表示本体开尔文温度乘以 100，`Tt` 表示目标摄氏温度乘以 100。源码使用放大常数、整数存储及中间截断，上述公式不保证逐位等同于实际结果。增益 275、抬升 1980 mV 和灵敏度除数 60 均来自代码，不是本次核实的实测值。

**EN:** `ADC_Func.c` selects `SREF_0` for A0 and `SREF_1` with the 2.5 V internal-reference setting for A1. However, `Tt_Calculate` converts A1 counts using 3300 mV. This inconsistency must be reconciled with the circuit before calibration.

**中文：** A0 配置为 `SREF_0`；A1 为 `SREF_1`，并启用 2.5 V 内部参考。然而目标温度函数用 3300 mV 换算 A1 码值，应结合电路解决该不一致后再标定。

## Display and time formats / 显示与时间格式

**EN:** `Temp_Dis` uses five buffer entries: sign/blank, two integer digits and two decimals. The watchdog handler adds the decimal point to position 2. Two displayed decimals describe formatting resolution, not measurement accuracy.

**中文：** 显示缓冲为符号/空白、两位整数和两位小数；看门狗中断在位置 2 加小数点。两位小数仅表示显示分辨率，不能等同于测量精度。

**EN:** `Set_DS1302` accepts seven BCD bytes ordered second, minute, hour, day, month, weekday, year. `Get_DS1302` converts the values used by the main loop into decimal year, month, day, hour, minute, second. The naming/comments around these two orders are inconsistent, so follow their implementations.

**中文：** 写入函数接收七个 BCD 字节，顺序为秒、分、时、日、月、星期、年；读取函数将主循环所用数据转换为十进制年、月、日、时、分、秒。相关注释存在顺序差异，应以实现为准。

## Source map / 源码索引

| Source / 源码 | Responsibility / 职责 |
| --- | --- |
| [main.c](../main.c) | Startup, averaging, interrupts and reporting / 初始化、平均、中断和报告 |
| [ADC_Func.c](../ADC_Func.c) | Two-channel repeated conversion / 双通道重复转换 |
| [DataTrans.c](../DataTrans.c) | Conversion and display formatting / 换算与显示格式 |
| [UART0.c](../UART0.c), [uart0.h](../uart0.h) | UART and delay macros / 串口及延时宏 |
| [ds1302.c](../ds1302.c), [ds1302.h](../ds1302.h) | GPIO-driven RTC access / GPIO 模拟 RTC 通信 |
