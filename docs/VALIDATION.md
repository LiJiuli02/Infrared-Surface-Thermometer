# Validation and limitations / 验证与限制

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## Evidence boundary / 证据范围

**EN:** Reviewed on 2026-11-04 against commit `bb3262372e72548a4c92de1c89b907bee1775a45`. This was a source and file-inventory review, including historical compiler metadata. No clean IAR build, flashing session or calibration experiment was performed. There was no existing project README to translate; these guides reconstruct behavior from the code and its Chinese comments.

**中文：** 2026-11-04 对照上述提交核查源码、文件目录和历史编译元数据。本次未执行全新 IAR 构建、烧录或标定实验。原仓库没有项目 README，本文档根据源码及中文注释整理。

## Issues to resolve before reproduction / 复现前需要解决的问题

| Finding / 发现 | Implication / 影响 |
| --- | --- |
| Complete IDE/linker setup absent / 缺少完整工程与链接配置 | Reconstruct and record a clean build / 需恢复并记录构建 |
| A1 reference setting and 3300 mV conversion disagree / A1 参考配置与 3300 mV 换算不一致 | Check circuit and ADC scaling / 核对电路和 ADC 比例 |
| `c0 = 0` divides by zero; resistance feeds `log` / 零码值除零，电阻进入对数 | Define valid range and fault handling / 定义输入范围与故障处理 |
| `Rt` and `Tk` use `unsigned int`; conversion truncates / 无符号整数及截断 | Check type widths, overflow and precision with the chosen compiler / 检查类型宽度、溢出与精度 |
| Floating-point/log calculations run inside ADC ISR / ADC 中断执行浮点与对数 | Measure latency and display interference / 测量延迟及对显示的影响 |
| Shared ADC/display/report state is not synchronized / 中断与主循环共享状态未同步 | Review `volatile` usage and consistent multi-field snapshots / 检查易变变量声明与一致快照 |
| RTC is overwritten at boot / 启动覆盖 RTC | Define intended time-setting behavior / 明确校时行为 |
| `memset(s, 40, 0)` clears zero bytes / 清零调用参数顺序错误 | It does not clear the buffer; inspect formatting assumptions / 实际未清空，需核对格式化 |
| `Tt > 0` controls serial sign / 串口以大于零判断正号 | Exactly zero uses the negative-sign branch / 零值进入负号分支 |
| Two integer display digits / 仅两位整数显示 | Define handling beyond display range / 定义超范围行为 |

**EN:** These are inspection findings, not claims that every failure was observed on a board. The documentation update leaves firmware unchanged.

**中文：** 以上是源码核查发现，并不表示所有问题都已在实物上复现；本次文档更新未修改固件。

## Proposed validation / 建议验证流程

1. **Build:** record device, IAR/compiler version, optimization, linker/runtime setup, warnings and commit.
   **构建：** 记录器件、工具版本、优化、链接与运行库配置、告警和提交。
2. **Electrical acquisition:** apply known voltages to both inputs within verified limits; record ADC counts, references and measured voltages.
   **电采集：** 在核实的允许范围内施加已知电压，记录两路码值、参考及实测电压。
3. **Conversion boundaries:** check zero/full-scale counts, invalid resistance, negative/zero temperature and display limits using independent calculations.
   **换算边界：** 独立检查零值、满量程、无效电阻、负温/零温和显示范围。
4. **Temperature comparison:** use an appropriate reference target/thermometer over a documented range; record sensor distance, target surface/emissivity, ambient conditions and settling time.
   **温度对比：** 使用合适的参考目标与温度计，记录范围、距离、表面/发射率、环境及稳定时间。
5. **Timing and interface:** measure scan/report periods, inspect serial sign/format and test RTC restart behavior.
   **时序与接口：** 测量扫描及报告周期，检查串口符号/格式和重启后的 RTC 行为。

Suggested result columns / 建议结果字段：

| Date / 日期 | Commit / 提交 | Reference °C / 参考温度 | Reported °C / 输出温度 | Error °C / 误差 | Conditions / 条件 |
| --- | --- | --- | --- | --- | --- |

**EN:** No measured range, accuracy, response time or uncertainty budget is available from this review. Do not infer ±0.01 °C accuracy from two decimal places.

**中文：** 本次核查未获得实测量程、精度、响应时间或不确定度预算，不能从两位小数推断 ±0.01 °C 精度。
