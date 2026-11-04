# Portfolio brief / 项目展示说明

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## Project summary / 项目简介

**EN:** An embedded measurement course project that combines dual-channel ADC acquisition, sensor-body compensation, interrupt-driven display scanning, and timestamped serial reporting on an MSP430. The firmware shows how analog measurements are converted into a human-readable temperature and integrated with a real-time clock.

**中文：** 本嵌入式测量课程项目在 MSP430 上结合双通道 ADC、本体温度补偿、中断驱动显示和带时间戳串口报告，展示模拟测量如何换算为温度并与实时时钟集成。

## Technical discussion / 技术讨论

- **Signal chain / 信号链：** Explain how resistor sensing and conditioned target signals feed temperature conversion. / 解释电阻检测与调理信号如何参与温度换算。
- **Embedded scheduling / 嵌入式调度：** Discuss ADC interrupts, display refresh and foreground reporting. / 讨论 ADC 中断、显示刷新和主循环报告的关系。
- **Numerical design / 数值设计：** Explain scaled temperature storage, truncation and display resolution. / 解释温度放大存储、截断和显示分辨率。
- **Measurement quality / 测量质量：** Relate reference-voltage consistency and calibration evidence to meaningful accuracy claims. / 说明参考电压一致性、标定证据与精度结论的关系。

## Demonstration / 演示流程

**EN:** Introduce the signal-flow diagram, identify each hardware module, show raw ADC counts and their conversion, then compare the display and timestamped UART line. If a calibrated reference is available, show a dated comparison table. Present source exploration separately from a live validated measurement.

**中文：** 先介绍信号流和各硬件模块，再展示原始码值与换算，对比数码管和带时间戳串口输出。有标定参考时展示带日期的对照表，并区分源码讲解与已验证的实物测量。

## Evidence to add / 建议补充

**EN:** Add the board/front-end schematic, BOM, sensor identification, recoverable IDE project, wiring photo, calibration table and short demonstration. Course dates, team roles and individual contributions require confirmation before inclusion in a personal résumé. This brief does not invent those details or measured performance.

**中文：** 建议补充板卡与前端原理图、物料清单、传感器型号、可恢复 IDE 工程、接线照片、标定表和演示视频。课程日期、团队分工与个人贡献需确认后再写入简历；本文不虚构这些信息或实测性能。
