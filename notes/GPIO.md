# General-purpose Input/Output (GPIO)

## Introduction

The General-purpose I/O (GPIO) is a generic pin on a chip whose behavior (including whether it is an input or output pin) can be controlled (programmed) by the user at run time. GPIO pins have no predefined purpose and are unused by default. They can be used to interface with external devices, such as LEDs, switches, sensors, and other devices.

STM32有多组GPIO，比如现在所使用的芯片：STM32F103ZET6共有7组GPIO端口，他们分别是GPIOx（x=A-G），每组控制16个引脚，共有112个GPIO引脚。具体一个其他STM32芯片有多少组GPIO，可以去查看对应的数据手册。
每个引脚的电平是0-3.3V，部分引脚最高可以兼容到5V。

### GPIO 的主要特点

1. 多功能：每个GPIO引脚都可以被配置为输入或输出模式，或者用于特殊功能，如外设的替代功能。

2. 可编程：GPIO引脚的模式和配置可以通过软件进行编程，包括输入/输出模式、上拉/下拉电阻、开漏/推挽输出等。

3. 中断能力：每个GPIO引脚都可以被配置为外部触发中断，例如在引脚电平改变或达到特定电平时触发。

4. 高驱动能力：GPIO引脚具有高电流驱动能力，可以直接驱动LED等设备。

5. 低功耗：在微控制器的低功耗模式下，GPIO引脚可以保持其状态。

6. 高速：GPIO引脚可以操作的频率高达几十MHz，适合于高速信号的输入/输出。

## GPIO functional description

Each of the general-purpose I/O ports has two 32-bit configuration registers (GPIOx_CRL,
GPIOx_CRH), two 32-bit data registers (GPIOx_IDR, GPIOx_ODR), a 32-bit set/reset
register (GPIOx_BSRR), a 16-bit reset register (GPIOx_BRR) and a 32-bit locking register (GPIOx_LCKR)

Subject to the specific hardware features of each I/O port (refer to the product reference)
each port bit of the General Purpose IO (GPIO) Ports, can be individually configured by software in several modes:

- 浮空输入 Input floating：
  这种模式下，引脚不连接任何电路，输入值取决于引脚的电平。通常用于高阻抗输入。
- 上拉输入 Input pull-up：
  这种模式下，引脚内部连接一个上拉电阻，如果没有外部电压输入，引脚将读取为高电平。
- 下拉输入 Input-pull-down：
  这种模式下，引脚内部连接一个下拉电阻，如果没有外部电压输入，引脚将读取为低电平。
- 模拟输入 Analog：
  这种模式下，引脚可以作为模拟信号的输入，通常连接到ADC（模拟/数字转换器）。
- 开漏输出 Output open-drain：
  这种模式下，引脚可以输出低电平，或者在输出高电平时进入高阻抗状态。通常用于I2C等开漏接口。
- 推挽输出 Output push-pull：
  这种模式下，引脚可以输出高电平或低电平，可以直接驱动LED等设备。
- 复用推挽输出 Alternate function push-pull：
  这种模式下，引脚可以作为某些外设（如UART、SPI等）的接口，同时也可以作为普通的推挽输出。
- 复用开漏输出 Alternate function open-drain：
  这种模式下，引脚可以作为某些外设（如I2C等）的接口，同时也可以作为普通的开漏输出。

### GPIO Reset

During and just after reset, the alternate functions are not active and the I/O ports are configured in `Input Floating` mode.

>JTAG是一种用于测试集成电路连续性的标准，也被广泛用于在系统级别对芯片进行调试和编程。

The JTAG pins are in input PU/PD after reset:
PA15: JTDI (JTAG Data Input) in PU (Pull-Up)
PA14: JTCK (JTAG Clock) in PD (Pull-Down)
PA13: JTMS (JTAG Mode Select) in PU (Pull-Up)
PB4: NJTRST (JTAG Reset) in PU (Pull-Up)

When configured as output, the value written to the Output Data register (GPIOx_ODR) is output on the I/O pin. It is possible to use the output driver in Push-Pull mode or Open-Drain mode (only the N-MOS is activated when outputting 0).
The Input Data register (GPIOx_IDR) captures the data present on the I/O pin at every APB2 clock cycle.
All GPIO pins have an internal weak pull-up and weak pull-down that can be activated or not
when configured as input.

### GPIO Atomic bit set/reset

There is no need for the software to disable interrupts when programming the GPIOx_ODR at bit level: it is possible to modify only one or several bits in a single atomic APB2 write access. This is achieved by programming to ‘1’ the Bit Set/Reset register (GPIOx_BSRR, or for reset only GPIOx_BRR) to select the bits to modify. The unselected bits will not be modified.

### External interrupt/event lines

All ports have external interrupt capability. To use external interrupt lines, the port must be configured in input mode.(refer to the product reference for External interrupt/event controller (EXTI) and Wakeup event management)

### Alternate function(AF)

It is necessary to program the Port Bit Configuration register before using a default alternate function.

- For alternate function inputs, the port must be configured in Input mode (floating, pull-
up or pull-down) and the input pin must be driven `externally`（输入引脚必须由外部驱动 意味着引脚的电平由外部电路决定）.

>Note: It is also possible to emulate（去模拟AFI 输入模式通过软件编程GPIO控制器） the AFI input pin by software by programming the GPIO controller. In this case, the port should be configured in Alternate Function Output mode.And obviously, the corresponding port should not be driven externally as it will be driven by the software using the GPIO controller.（相应的端口不应由外部驱动，因为它将由使用GPIO控制器的软件驱动）

- For alternate function outputs, the port must be configured in Alternate Function Output mode (Push-Pull or Open-Drain).
- For bidirectional Alternate Functions, the port bit must be configured in Alternate Function Output mode (Push-Pull or Open-Drain). In this case the input driver is configured in input floating mode
If a port bit is configured as Alternate Function Output, this disconnects the output register and connects the pin to the output signal of an on-chip peripheral.（如果一个端口位被配置为替代功能输出，这将断开输出寄存器，并将引脚连接到片上外设的输出信号）
If software configures a GPIO pin as Alternate Function Output, but peripheral is not activated, its output is not specified.（如果软件将GPIO引脚配置为替代功能输出，但外设没有被激活，那么其输出是未指定的。这意味着如果没有启用相应的外设，GPIO引脚的电平可能是不确定的）

### Software remapping of I/O alternate functions

To optimize the number of peripheral I/O functions for different device packages, it is possible to remap some alternate functions to some other pins. This is achieved by software, by programming the corresponding registers (refer to AFIO registers). In that case, the alternate functions are no longer mapped to their original assignations.

### Input configuration

当引脚配置为输入模式时，会执行如下步骤：

1. The Output Buffer is disabled
2. The Schmitt Trigger Input is activated
3. The weak pull-up and pull-down resistors are activated or not depending on input configuration (pull-up, pull-down or floating):
4. The data present on the I/O pin is sampled into the Input Data register every APB2 clock cycle
5. A read access to the Input Data register obtains the I/O State.

### Output configuration

当引脚配置为输出模式时，会执行如下步骤：

1. 输出缓冲器被使能
    - 开漏模式：在输出数据寄存器中的数据为0时，会激活N-MOS，而数据为1时，会使端口处于高阻态（P-MOS始终处于截止状态）。
    - 推挽模式：在输出数据寄存器中的数据为0时，会激活N-MOS，而数据为1时，会激活P-MOS。
2. The Schmitt Trigger Input is activated.
3. The weak pull-up and pull-down resistors are disabled.
4. The data present on the I/O pin is sampled into the Input Data register every APB2 clock cycle.
5. 对输入数据寄存器的读取会获取到开漏模式下的I/O状态。
6. 对输出数据寄存器的读取会获取到推挽模式下最后写入的值。

### Alternate function configuration

When the I/O Port is programmed as Alternate Function:

- The Output Buffer is turned on in Open Drain or Push-Pull configuration
- The Output Buffer is driven by the signal coming from the peripheral (alternate function
out)
- The Schmitt Trigger Input is activated
- The weak pull-up and pull-down resistors are disabled.
- The data present on the I/O pin is sampled into the Input Data register every APB2 clock cycle
- A read access to the Input Data register gets the I/O state in open drain mode
- A read access to the Output Data register gets the last written value in Push-Pull mode

### GPIO configurations for device peripherals

The GPIO configuration for device peripherals is described in the reference manual of the device.

## GPIO Registers

Refer to Section 2.2 on page 45 for a list of abbreviations used in register descriptions of the GPIO reference manual.
``The peripheral registers have to be accessed by words (32-bit).``

### Port configuration registers low (GPIOx_CRL) (x=A..G)

### Port configuration registers high (GPIOx_CRH) (x=A..G)

### Port input data register (GPIOx_IDR) (x=A..G)

`Bits 31:16 Reserved, must be kept at reset value (0x0000).`
Bits 15:0 IDRy: Port input data (y= 0 .. 15)
These bits are `read only` and can `be accessed in Word mode only`. They contain the input value of the corresponding I/O port.

### Port output data register (GPIOx_ODR) (x=A..G)

`Bits 31:16 Reserved, must be kept at reset value.`
Bits 15:0 ODRy: Port output data (y= 0 .. 15)
These bits can `be read and written` by software and can `be accessed in Word mode only`.

>Note: For atomic bit set/reset, the ODR bits can be individually set and cleared by writing to the GPIOx_BSRR register (x = A..G)

### Port bit set/reset register (GPIOx_BSRR) (x=A..G)

Bits 31:16 BRy: Port x Reset bit y (y= 0 .. 15)
These bits are `write-only` and can be accessed in `Word mode only`.
0: No action on the corresponding ODRx bit
1: Reset the corresponding ODRx bit

Bits 15:0 BSy: Port x Set bit y (y= 0 .. 15)
These bits are `write-only` and can be accessed in `Word mode only`.
0: No action on the corresponding ODRx bit
1: Set the corresponding ODRx bit

`Note: If both BSx and BRx are set, BSx has priority.`

For atomic bit set/reset, the ODR bits can be individually set and cleared by writing to the GPIOx_BSRR register (x = A..G).

在STM32的GPIO中，ODR（Output Data Register）位可以通过写入GPIOx_BSRR（Bit Set/Reset Register）寄存器来单独设置和清除完成原子操作。

"原子操作"是指在执行过程中不会被其他任务或事件中断的操作。在这种情况下，原子位设置/重置意味着设置或重置ODR位的操作是不可分割的，即在这个操作执行过程中，不会被其他GPIO操作中断。

这种原子操作对于需要确保GPIO状态改变的一致性和准确性的应用非常有用，例如在多任务环境中，或者在需要精确控制GPIO状态改变时间的情况下。

### Port bit reset register (GPIOx_BRR) (x=A..G)

Each I/O port bit is freely programmable, however the I/O port registers have to be
accessed as 32-bit words (half-word or byte accesses are not allowed). The purpose of the
GPIOx_BSRR and GPIOx_BRR registers is to allow atomic read/modify accesses to any of
the GPIO registers. This way, there is no risk that an IRQ occurs between the read and the
modify access.
Figure 13 shows the basic structure of an I/O Port bit.
