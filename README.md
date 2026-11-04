# Infrared Surface Thermometer

[简体中文](README.zh-CN.md) · [Setup](docs/GETTING_STARTED.md) · [Architecture](docs/ARCHITECTURE.md) · [Validation](docs/VALIDATION.md) · [Portfolio](docs/PORTFOLIO.md)

An MSP430 course-project firmware archive for non-contact surface-temperature measurement. It combines two analog acquisition channels, sensor-body temperature compensation (温度补偿), a five-position seven-segment display (数码管), and timestamped UART output using a DS1302 real-time clock (RTC，实时时钟).

The conversion code identifies the sensor as **SI-111**. Its formulas depend on the original analog front end and calibration constants; the repository does not contain the schematic or calibration report needed to establish measurement accuracy.

**Status:** source archive with historical IAR build artifacts. No complete IDE project or reproducible clean build is supplied, and hardware operation has not been revalidated for this documentation update.

## Implemented in source

- Repeated two-channel ADC12 conversion with ten-sample averaging.
- Resistance-to-body-temperature conversion and an amplified/offset signal-to-target-temperature calculation.
- Interrupt-driven multiplexed display refresh.
- DS1302 register access and date/time formatting.
- Nominal 9600-baud serial reporting using an 8 MHz clock assumption.

## At a glance

| Item | Evidence |
| --- | --- |
| MCU family | MSP430x24x headers; archived build metadata names MSP430F249 |
| Historical environment | IAR Embedded Workbench 6.4 path; archived compiler metadata reports V5.50.1.20465 |
| Analog inputs | P6.0/A0 and P6.1/A1 |
| Display | P4 data bus; P5.5 segment latch, P5.6 digit latch |
| RTC | DS1302: P2.7 reset/enable, P2.5 clock, P2.6 data |
| Serial | P3.4 TX, P3.5 RX; nominal 9600, 8N1 |
| Reporting loop | Two `delay_ms(1000)` calls; actual interval also depends on interrupt execution |

## Documentation

- [Getting started](docs/GETTING_STARTED.md): hardware checklist, build recovery and serial operation.
- [Architecture](docs/ARCHITECTURE.md): conversion formulas, interrupt flow, pins and output format.
- [Validation](docs/VALIDATION.md): missing evidence, source-level risks and calibration plan.
- [Portfolio brief](docs/PORTFOLIO.md): project description and demonstration outline.
- [Contributing](CONTRIBUTING.md): bilingual maintenance and evidence requirements.

## Repository layout

| Path | Purpose |
| --- | --- |
| `main.c` | Startup, ADC/display interrupt handlers and reporting loop |
| `ADC_Func.c/.h` | ADC setup |
| `DataTrans.c/.h` | Temperature conversion and display formatting |
| `UART0.c`, `uart0.h` | Serial output and clock-dependent delay macros |
| `ds1302.c/.h` | Software-driven RTC interface |
| `Debug/`, `Release/`, `settings/` | Historical generated files; not a replacement for project configuration |
| `docs/` | Bilingual engineering and portfolio documentation |

Start with the [build-recovery guide](docs/GETTING_STARTED.md). The upload is suitable for source exploration, but cannot be presented as a verified clone-and-build package yet.

No repository-level license file was found in the reviewed snapshot; no new license is assigned by this documentation update.

Reviewed against commit `bb32623` on **2026-11-04**.
