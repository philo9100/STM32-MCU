# Universal synchronous asynchronous receiver transmitter (USART)

## USART introduction

The universal synchronous asynchronous receiver transmitter (USART) offers a flexible means of full-duplex data exchange with external equipment requiring an industry standard NRZ asynchronous serial data format. The USART offers a very wide range of baud rates using a fractional baud rate generator.
It supports synchronous one-way communication and half-duplex single wire communication. It also supports the LIN (local interconnection network), Smartcard Protocol and IrDA (infrared data association) SIR ENDEC specifications, and modem operations (CTS/RTS). It allows multiprocessor communication.
High speed data communication is possible by using the DMA for multibuffer configuration

## USART main features

- Full duplex, asynchronous communications
- NRZ standard format (Mark/Space)
- Fractional baud rate generator systems
  – A common programmable transmit and receive baud rates up to 4.5 MBits/s
- Programmable data word length (8 or 9 bits)
- Configurable stop bits - support for 1 or 2 stop bits
- LIN Master Synchronous Break send capability and LIN slave break detection capability
  – 13-bit break generation and 10/11 bit break detection when USART is hardware configured for LIN
- Transmitter clock output for synchronous transmission
- IrDA SIR Encoder Decoder

> IrDA SIR（Infrared Data Association Serial Infrared）是一种红外数据通信标准。它是由Infrared Data Association（IrDA）组织制定的一系列红外数据通信标准中的一种。
>
> SIR代表Serial Infrared，是IrDA标准中的基本模式，最高数据传输速率为115.2 kbps。它模拟了串行通信协议（如RS-232），但使用红外光进行通信，因此无需物理连接。这种通信方式常用于短距离（通常在1米以内）的设备间通信，如手机、PDA、打印机等设备之间的数据传输。
  – Support for 3/16 bit duration for normal mode
  
- Smartcard Emulation Capability
– The Smartcard interface supports the asynchronous protocol Smartcards as 
defined in ISO 7816-3 standards
– 0.5, 1.5 Stop Bits for Smartcard operation
 Single wire half duplex communication
 Configurable multibuffer communication using DMA (direct memory access)
– Buffering of received/transmitted bytes in reserved SRAM using centralized DMA
 Separate enable bits for Transmitter and Receiver
 Transfer detection flags:
– Receive buffer full
– Transmit buffer empty
– End of Transmission flags
 Parity control:
– Transmits parity bit
– Checks parity of received data byte
 Four error detection flags:
– Overrun error
– Noise error
– Frame error
– Parity error
 Ten interrupt sources with flags:
– CTS changes
– LIN break detection
– Transmit data register empty
– Transmission complete
– Receive data register full
– Idle line received
– Overrun error
– Framing error 
– Noise error
– Parity error
 Multiprocessor communication - enter into mute mode if address match does not occur
 Wake up from mute mode (by idle line detection or address mark detection)
 Two receiver wakeup modes: Address bit (MSB, 9th bit), Idle line

## USART functional description
The interface is externally connected to another device by three pins (see Figure 279). Any 
USART bidirectional communication requires a minimum of two pins: Receive Data In (RX) 
and Transmit Data Out (TX):
RX: Receive Data Input is the serial data input. Oversampling techniques are used for data 
recovery by discriminating between valid incoming data and noise.
TX: Transmit Data Output. When the transmitter is disabled, the output pin returns to its IO 
port configuration. When the transmitter is enabled and nothing is to be transmitted, the TX 
pin is at high level. In single-wire and smartcard modes, this IO is used to transmit and 
receive the data (at USART level, data are then received on SW_RX).

Through these pins, serial data is transmitted and received in normal USART mode as 
frames comprising: 
 An Idle Line prior to transmission or reception
 A start bit
 A data word (8 or 9 bits) least significant bit first
 0.5,1, 1.5, 2 Stop bits indicating that the frame is complete
 This interface uses a fractional baud rate generator - with a 12-bit mantissa and 4-bit 
fraction
 A status register (USART_SR)
 Data register (USART_DR)
 A baud rate register (USART_BRR) - 12-bit mantissa and 4-bit fraction.
 A guard-time register (USART_GTPR) in case of Smartcard mode.

The following pin is required to interface in synchronous mode:
 CK: Transmitter clock output. This pin outputs the transmitter data clock for 
synchronous transmission corresponding to SPI master mode (no clock pulses on start 
bit and stop bit, and a software option to send a clock pulse on the last data bit). In 
parallel data can be received synchronously on RX. This can be used to control 
peripherals that have shift registers (e.g. LCD drivers). The clock phase and polarity 
are software programmable. In Smartcard mode, CK can provide the clock to the 
smartcard.
The following pins are required in Hardware flow control mode:
 CTS: Clear To Send blocks the data transmission at the end of the current transfer 
when high
 RTS: Request to send indicates that the USART is ready to receive a data (when low)