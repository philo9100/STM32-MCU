#include "stdint.h"
#include "stm32f10x.h" // Device header

int main()
{
	// 1.enable 对应 GPIO Pin组（本例是GPIOA组的Pin）的时钟
	/*
	当外设时钟未激活时，软件可能无法读取外设寄存器的值，返回的值总是0x0。
	在许多微控制器中，外设（如GPIO、UART等）的时钟默认是关闭的，以节省功耗。在使用这些外设之前，需要先激活（或打开）它们的时钟。
	如果没有激活外设的时钟，那么尝试读取该外设的寄存器可能会返回无效的值，这里指的就是0x0。
	这意味着，在编写使用外设的代码时，需要确保首先激活了外设的时钟。否则，你可能会看到意料之外的行为，因为寄存器的值可能不是你期望的值。

	配置时钟需要通过 reset and clock control (RCC) 下的一系列 registers 进行操作，配置GPIO就要找到它所在的位置
	通过 Memory and bus architecture 下的 System architecture 可以看到所有GPIO 都在 APB2 Bus 上，找到GPIOx在APB2（Advanced Peripheral Bus 2）总线上的位置
	因此，通过APB2外设时钟使能寄存器（APB2 peripheral clock enable register)(RCC_APB2ENR用来设置APB2总线上的外设是否enable）enable对应GPIO组（A组）的时钟
	参考手册中找到APB2外设时钟使能寄存器（RCC_APB2ENR）
	Address offset 偏移地址（0x18），是相对RCC下第一个寄存器（Clock control register (RCC_CR)）时钟控制寄存器的start address（0x4002 1000）而言
	每个寄存器都是32bit（四个字节）因此都是按照 4 个字节的地址进行偏移
	RCC寄存器的Boundary address在memory and bus architecture中的memory map中查找
	Reset value（0x0000 0000）表示的是该寄存器的复位值
	Access: word, half-word and byte access 表示的是该寄存器的访问方式
	访问APB2域中的外设时可能出现等待的状态，"等待状态"是指CPU在等待数据从外设或内存获取时的状态。在这种状态下，CPU会暂停执行，直到数据可用。
	通常情况下，访问APB2总线中的外设时不会产生等待状态，也就是说，CPU可以立即获取数据，但是，如果CPU在访问ABP2总线上的一个外设时，另一个访问请求正在进行
	那么当前这个请求会产生等待状态，直到对APB2外设的访问完成。这种机制可以确保对APB2外设的访问是按照请求的顺序进行的不会相互干扰，从而保证数据的准确性。

	通过寄存器示意图，第三位对应的是 GPIO port A clock enable（IOPAEN），by refering to manual知道该位置为1表示IO port A clock enabled
	即在该寄存器中写入表示的二进制数100，即4
	*/

	*(uint32_t *)(0x40021000 + 0x18) = 4; // 0x4002 1018是(RCC_APB2ENR)的首字节起始地址，包括起始地址对应的字节要访问寄存器的四个字节
										  // 将这个32位的16进制数表示成地址要强转成一个32位（4字节）的指针，通过该指针取值找到对应的寄存器（4字节内存空间）并赋值为4

	// 2.给GPIOA Pin设置工作模式：输出模式
	/*
	通过General-purpose I/Os找到GPIO register 中的 端口配置寄存器低位（port configuration register low）（GPIOx_CRL用来设置前8个GPIO Pin，其中x表示组（x=A..G），每个组中有16个引脚（0-15）
	一个寄存器有32bit，需要4bit配置一个引脚，因此一个GPIOx_CRL可以配置前8个引脚（0-7），一个Port configuration register high (GPIOx_CRH)可以配置后8个引脚（8-15）总共16个引脚）
	4位中的前两位用来配置MODE，后两位用来配置CNF
	GPIOx(x=A...G) 下的寄存器都是一致的，唯一的区别就是选择不同的端口（既不同的组）如果要配置GPIOA，要找到GPIO Port A 的起始地址
	Port A configuration register low (GPIOA_CRL) 这个寄存器的地址偏移量
	address offset（0x00）,是相对GPIO Port A的start address（0x4001 0800）而言
	根据偏移量可知，Port A configuration register low 是 GPIO Port A 下所有寄存器中的第一个
	GPIO Port A寄存器的Boundary address在memory and bus architecture中的memory map中查找
	通过寄存器示意图，PA0（GPIO Port A 下的 0 号引脚）对应的MODE0配置成11（ Output mode, max speed 50 MHz）CNF0配置成00（General purpose output push-pull）
	即在该寄存器中写入表示的二进制数0011，即3
	*/

	*(uint32_t *)(0x40010800 + 0x00) = 3; // *(uint32_t*)(0x40010800+0x00) 这步操作是找到寄存器对应的内存空间 并写入值 0b0011

	// 3.给对应GPIOA Pin0设置输出值：1/0
	/*
	通过General-purpose I/Os找到GPIO registers 中的 端口输出数据寄存器（Port output data register）（GPIOx_ODR用来设置端口中引脚输出的数据值）
	address offset（0x0c）,是相对GPIO Port A的start address（0x4001 0800）而言
	GPIO Port A寄存器的Boundary address在memory and bus architecture中的memory map中查找
	Bits 31:16 Reserved, must be kept at reset value（Reset value：0x0000 0000），重置的值中高16位是全0，也作为高位的保留值
	Bits 15:0 ODRy: Port output data (y= 0 .. 15).
	These bits can be read and written by software and can be accessed in Word mode only.
	对于ODR寄存器的位的原子 set/reset，可以通过单独写入或者清除GPIOx_BSRR register来完成。

	通过寄存器示意图，第一位ODR0对应的是第一个引脚，设置为0表示输出低电平，其他设置为高电平，保留位设置为reset value（0x0000 0000）
	*/

	*(uint32_t *)(0x40010800 + 0x0c) = 0x0000fffe; // 写入 0b0000 0000 0000 0000 1111 1111 1111 1110

	/*
	17节编译下载需要配置debug
	*/

	// 进化1：地址换成ST定义好的宏，引入标准库的头文件
	RCC->APB2ENR = 4;
	/*
	通过源码分析可以知道 RCC 是(RCC_TypeDef *) RCC_BASE 的宏定义
	RCC_BASE 是 (uint32_t)0x40021000 即，RCC寄存器的基地址，此时还是一个16进制数，将其转换成 RCC_TypeDef 的结构体指针就变成了一个地址
	RCC_TypeDef结构体中定义的都是RCC中的众多寄存器成员，既是RCC registers下的所有寄存器，通过结构体指针访问成员，访问到对应结构体成员
	既是各个寄存器的起始地址
	比如APB2ENR寄存器在第7个位置，因此就是在RCC_TypeDef地址的基础上再偏移24（4*6）个字节的地址，即偏移0x18
	也就是*(uint32_t*)(0x40021000+0x18)的操作
	*/

	// 同上，可以配置GPIOx Pinx的端口配置寄存器低位和端口输出数据寄存器
	GPIOA->CRL = 3;
	GPIOA->ODR = 0xfffe;

	// 进化2：只操作需要的位，避免误操作别的位（STM 32的寄存器要操作就是32位同时操作，因此要通过位运算操作一个位）
	// RCC->APB2ENR = (RCC->APB2ENR | 4);
	RCC->APB2ENR |= 4;
	/*
	只想把该寄存器的第三位置为1别的位保持原来的值，使用或运算
	即，RCC->APB2EN2的值（四个字节的内存空间中的值，该四字节的内存空间起名叫外设时钟使能寄存器）
	或上0000 0000 0000 0000 0000 0000 0000 0100
	得到的结果是该内存空间（寄存器）中的值只有第三位是1，其他位还保持原来的值
	结论
		如果想让某一位置为1其他位保持不变，就或上该位为1其他位皆为0的数
		如果想让某一位置为0其他位保持不变，就与上该位为0其他位皆为1的数，或者该位为1其他位皆为0的数先取反，再去与操作
	*/

	// 同上 GPIOA->CRL = 3;
	GPIOA->CRL |= 1;
	GPIOA->CRL |= 2;
	GPIOA->CRL &= ~4;
	GPIOA->CRL &= ~8;

	// 同上 GPIOA->ODR = 0xfffe
	GPIOA->ODR &= ~1;

	// 进化3：对于高位操作，不用计算高位对应的值（根据进化2操作低位时，只要或上该位为1其他位为0的数，或者与上该位为0其他位为1的数即可
	// 这适合低位计算数值，不适合高位计算数值，而且不便看出操作的是哪一个位上的值。因此可以使用1左移或者右移的操作）
	RCC->APB2ENR |= (1 << 2);
	/*
	1左移2位 低位用0补 变为 100 刚好是4 而且可以很快看出操作的是第3位的值
	*/

	// 同上
	GPIOA->CRL |= 1 << 0;
	GPIOA->CRL |= (1 << 1);
	GPIOA->CRL &= ~(1 << 2);
	GPIOA->CRL &= ~(1 << 3);

	// 同上
	GPIOA->ODR &= ~(1 << 0);

	// 进化4：直接使用定义好的宏（可以操作寄存器对应的位，但不便于知道该位的作用是什么）
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // 复位和时钟控制寄存器下的APB2域内外设时钟使能寄存器使能

	// 同上
	GPIOA->CRL |= GPIO_CRL_MODE0_0; // 端口配置寄存器低位第一个引脚的MODE的第一位配置为1
	GPIOA->CRL |= GPIO_CRL_MODE0_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF0_0; // 端口配置寄存器低位第一个引脚的CNF的第一位配置为0
	GPIOA->CRL &= ~GPIO_CRL_CNF0_1;
	// GPIOA->CRL = ~GPIO_CRL_CNF0_0 | ~GPIO_CRL_CNF0_0 | GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0;

	// 同上
	GPIOA->ODR &= ~GPIO_ODR_ODR0;

	return 0;
}
