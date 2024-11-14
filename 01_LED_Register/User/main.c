#include "stdint.h"
#include "stm32f10x.h" // Device header

int main()
{
	// 1.enable ��Ӧ GPIO Pin�飨������GPIOA���Pin����ʱ��
	/*
	������ʱ��δ����ʱ����������޷���ȡ����Ĵ�����ֵ�����ص�ֵ����0x0��
	�����΢�������У����裨��GPIO��UART�ȣ���ʱ��Ĭ���ǹرյģ��Խ�ʡ���ġ���ʹ����Щ����֮ǰ����Ҫ�ȼ����򿪣����ǵ�ʱ�ӡ�
	���û�м��������ʱ�ӣ���ô���Զ�ȡ������ļĴ������ܻ᷵����Ч��ֵ������ָ�ľ���0x0��
	����ζ�ţ��ڱ�дʹ������Ĵ���ʱ����Ҫȷ�����ȼ����������ʱ�ӡ���������ܻῴ������֮�����Ϊ����Ϊ�Ĵ�����ֵ���ܲ�����������ֵ��

	����ʱ����Ҫͨ�� reset and clock control (RCC) �µ�һϵ�� registers ���в���������GPIO��Ҫ�ҵ������ڵ�λ��
	ͨ�� Memory and bus architecture �µ� System architecture ���Կ�������GPIO ���� APB2 Bus �ϣ��ҵ�GPIOx��APB2��Advanced Peripheral Bus 2�������ϵ�λ��
	��ˣ�ͨ��APB2����ʱ��ʹ�ܼĴ�����APB2 peripheral clock enable register)(RCC_APB2ENR��������APB2�����ϵ������Ƿ�enable��enable��ӦGPIO�飨A�飩��ʱ��
	�ο��ֲ����ҵ�APB2����ʱ��ʹ�ܼĴ�����RCC_APB2ENR��
	Address offset ƫ�Ƶ�ַ��0x18���������RCC�µ�һ���Ĵ�����Clock control register (RCC_CR)��ʱ�ӿ��ƼĴ�����start address��0x4002 1000������
	ÿ���Ĵ�������32bit���ĸ��ֽڣ���˶��ǰ��� 4 ���ֽڵĵ�ַ����ƫ��
	RCC�Ĵ�����Boundary address��memory and bus architecture�е�memory map�в���
	Reset value��0x0000 0000����ʾ���ǸüĴ����ĸ�λֵ
	Access: word, half-word and byte access ��ʾ���ǸüĴ����ķ��ʷ�ʽ
	����APB2���е�����ʱ���ܳ��ֵȴ���״̬��"�ȴ�״̬"��ָCPU�ڵȴ����ݴ�������ڴ��ȡʱ��״̬��������״̬�£�CPU����ִͣ�У�ֱ�����ݿ��á�
	ͨ������£�����APB2�����е�����ʱ��������ȴ�״̬��Ҳ����˵��CPU����������ȡ���ݣ����ǣ����CPU�ڷ���ABP2�����ϵ�һ������ʱ����һ�������������ڽ���
	��ô��ǰ������������ȴ�״̬��ֱ����APB2����ķ�����ɡ����ֻ��ƿ���ȷ����APB2����ķ����ǰ��������˳����еĲ����໥���ţ��Ӷ���֤���ݵ�׼ȷ�ԡ�

	ͨ���Ĵ���ʾ��ͼ������λ��Ӧ���� GPIO port A clock enable��IOPAEN����by refering to manual֪����λ��Ϊ1��ʾIO port A clock enabled
	���ڸüĴ�����д���ʾ�Ķ�������100����4
	*/

	*(uint32_t *)(0x40021000 + 0x18) = 4; // 0x4002 1018��(RCC_APB2ENR)�����ֽ���ʼ��ַ��������ʼ��ַ��Ӧ���ֽ�Ҫ���ʼĴ������ĸ��ֽ�
										  // �����32λ��16��������ʾ�ɵ�ַҪǿת��һ��32λ��4�ֽڣ���ָ�룬ͨ����ָ��ȡֵ�ҵ���Ӧ�ļĴ�����4�ֽ��ڴ�ռ䣩����ֵΪ4

	// 2.��GPIOA Pin���ù���ģʽ�����ģʽ
	/*
	ͨ��General-purpose I/Os�ҵ�GPIO register �е� �˿����üĴ�����λ��port configuration register low����GPIOx_CRL��������ǰ8��GPIO Pin������x��ʾ�飨x=A..G����ÿ��������16�����ţ�0-15��
	һ���Ĵ�����32bit����Ҫ4bit����һ�����ţ����һ��GPIOx_CRL��������ǰ8�����ţ�0-7����һ��Port configuration register high (GPIOx_CRH)�������ú�8�����ţ�8-15���ܹ�16�����ţ�
	4λ�е�ǰ��λ��������MODE������λ��������CNF
	GPIOx(x=A...G) �µļĴ�������һ�µģ�Ψһ���������ѡ��ͬ�Ķ˿ڣ��Ȳ�ͬ���飩���Ҫ����GPIOA��Ҫ�ҵ�GPIO Port A ����ʼ��ַ
	Port A configuration register low (GPIOA_CRL) ����Ĵ����ĵ�ַƫ����
	address offset��0x00��,�����GPIO Port A��start address��0x4001 0800������
	����ƫ������֪��Port A configuration register low �� GPIO Port A �����мĴ����еĵ�һ��
	GPIO Port A�Ĵ�����Boundary address��memory and bus architecture�е�memory map�в���
	ͨ���Ĵ���ʾ��ͼ��PA0��GPIO Port A �µ� 0 �����ţ���Ӧ��MODE0���ó�11�� Output mode, max speed 50 MHz��CNF0���ó�00��General purpose output push-pull��
	���ڸüĴ�����д���ʾ�Ķ�������0011����3
	*/

	*(uint32_t *)(0x40010800 + 0x00) = 3; // *(uint32_t*)(0x40010800+0x00) �ⲽ�������ҵ��Ĵ�����Ӧ���ڴ�ռ� ��д��ֵ 0b0011

	// 3.����ӦGPIOA Pin0�������ֵ��1/0
	/*
	ͨ��General-purpose I/Os�ҵ�GPIO registers �е� �˿�������ݼĴ�����Port output data register����GPIOx_ODR�������ö˿����������������ֵ��
	address offset��0x0c��,�����GPIO Port A��start address��0x4001 0800������
	GPIO Port A�Ĵ�����Boundary address��memory and bus architecture�е�memory map�в���
	Bits 31:16 Reserved, must be kept at reset value��Reset value��0x0000 0000�������õ�ֵ�и�16λ��ȫ0��Ҳ��Ϊ��λ�ı���ֵ
	Bits 15:0 ODRy: Port output data (y= 0 .. 15).
	These bits can be read and written by software and can be accessed in Word mode only.
	����ODR�Ĵ�����λ��ԭ�� set/reset������ͨ������д��������GPIOx_BSRR register����ɡ�

	ͨ���Ĵ���ʾ��ͼ����һλODR0��Ӧ���ǵ�һ�����ţ�����Ϊ0��ʾ����͵�ƽ����������Ϊ�ߵ�ƽ������λ����Ϊreset value��0x0000 0000��
	*/

	*(uint32_t *)(0x40010800 + 0x0c) = 0x0000fffe; // д�� 0b0000 0000 0000 0000 1111 1111 1111 1110

	/*
	17�ڱ���������Ҫ����debug
	*/

	// ����1����ַ����ST����õĺ꣬�����׼���ͷ�ļ�
	RCC->APB2ENR = 4;
	/*
	ͨ��Դ���������֪�� RCC ��(RCC_TypeDef *) RCC_BASE �ĺ궨��
	RCC_BASE �� (uint32_t)0x40021000 ����RCC�Ĵ����Ļ���ַ����ʱ����һ��16������������ת���� RCC_TypeDef �Ľṹ��ָ��ͱ����һ����ַ
	RCC_TypeDef�ṹ���ж���Ķ���RCC�е��ڶ�Ĵ�����Ա������RCC registers�µ����мĴ�����ͨ���ṹ��ָ����ʳ�Ա�����ʵ���Ӧ�ṹ���Ա
	���Ǹ����Ĵ�������ʼ��ַ
	����APB2ENR�Ĵ����ڵ�7��λ�ã���˾�����RCC_TypeDef��ַ�Ļ�������ƫ��24��4*6�����ֽڵĵ�ַ����ƫ��0x18
	Ҳ����*(uint32_t*)(0x40021000+0x18)�Ĳ���
	*/

	// ͬ�ϣ���������GPIOx Pinx�Ķ˿����üĴ�����λ�Ͷ˿�������ݼĴ���
	GPIOA->CRL = 3;
	GPIOA->ODR = 0xfffe;

	// ����2��ֻ������Ҫ��λ��������������λ��STM 32�ļĴ���Ҫ��������32λͬʱ���������Ҫͨ��λ�������һ��λ��
	// RCC->APB2ENR = (RCC->APB2ENR | 4);
	RCC->APB2ENR |= 4;
	/*
	ֻ��ѸüĴ����ĵ���λ��Ϊ1���λ����ԭ����ֵ��ʹ�û�����
	����RCC->APB2EN2��ֵ���ĸ��ֽڵ��ڴ�ռ��е�ֵ�������ֽڵ��ڴ�ռ�����������ʱ��ʹ�ܼĴ�����
	����0000 0000 0000 0000 0000 0000 0000 0100
	�õ��Ľ���Ǹ��ڴ�ռ䣨�Ĵ������е�ֵֻ�е���λ��1������λ������ԭ����ֵ
	����
		�������ĳһλ��Ϊ1����λ���ֲ��䣬�ͻ��ϸ�λΪ1����λ��Ϊ0����
		�������ĳһλ��Ϊ0����λ���ֲ��䣬�����ϸ�λΪ0����λ��Ϊ1���������߸�λΪ1����λ��Ϊ0������ȡ������ȥ�����
	*/

	// ͬ�� GPIOA->CRL = 3;
	GPIOA->CRL |= 1;
	GPIOA->CRL |= 2;
	GPIOA->CRL &= ~4;
	GPIOA->CRL &= ~8;

	// ͬ�� GPIOA->ODR = 0xfffe
	GPIOA->ODR &= ~1;

	// ����3�����ڸ�λ���������ü����λ��Ӧ��ֵ�����ݽ���2������λʱ��ֻҪ���ϸ�λΪ1����λΪ0�������������ϸ�λΪ0����λΪ1��������
	// ���ʺϵ�λ������ֵ�����ʺϸ�λ������ֵ�����Ҳ��㿴������������һ��λ�ϵ�ֵ����˿���ʹ��1���ƻ������ƵĲ�����
	RCC->APB2ENR |= (1 << 2);
	/*
	1����2λ ��λ��0�� ��Ϊ 100 �պ���4 ���ҿ��Ժܿ쿴���������ǵ�3λ��ֵ
	*/

	// ͬ��
	GPIOA->CRL |= 1 << 0;
	GPIOA->CRL |= (1 << 1);
	GPIOA->CRL &= ~(1 << 2);
	GPIOA->CRL &= ~(1 << 3);

	// ͬ��
	GPIOA->ODR &= ~(1 << 0);

	// ����4��ֱ��ʹ�ö���õĺ꣨���Բ����Ĵ�����Ӧ��λ����������֪����λ��������ʲô��
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // ��λ��ʱ�ӿ��ƼĴ����µ�APB2��������ʱ��ʹ�ܼĴ���ʹ��

	// ͬ��
	GPIOA->CRL |= GPIO_CRL_MODE0_0; // �˿����üĴ�����λ��һ�����ŵ�MODE�ĵ�һλ����Ϊ1
	GPIOA->CRL |= GPIO_CRL_MODE0_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF0_0; // �˿����üĴ�����λ��һ�����ŵ�CNF�ĵ�һλ����Ϊ0
	GPIOA->CRL &= ~GPIO_CRL_CNF0_1;
	// GPIOA->CRL = ~GPIO_CRL_CNF0_0 | ~GPIO_CRL_CNF0_0 | GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0;

	// ͬ��
	GPIOA->ODR &= ~GPIO_ODR_ODR0;

	return 0;
}
