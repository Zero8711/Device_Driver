#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define DEBUG 1
#define IMX_GPIO_NR(bank, nr)       (((bank) - 1) * 32 + (nr))

static int led[] = {
	IMX_GPIO_NR(1, 16),   //16
	IMX_GPIO_NR(1, 17),	  //17
	IMX_GPIO_NR(1, 18),   //18
	IMX_GPIO_NR(1, 19),   //19
};

void led_state(unsigned long data)
{
	for(int i = 0; i < 4; i++)
	{
		if(data == 1)
			printk("O"); //O
		else
			printk("X"); //X
		if(i != 3)
			printk(":");
		else
			printk("\n");
	}

static int led_init(void)
{
	int ret = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(led); i++) {
		ret = gpio_request(led[i], "gpio led");
		if(ret<0){
			printk("#### FAILED Request gpio %d. error : %d \n", led[i], ret);
		} 
		gpio_direction_output(led[i], 1);
	}
	return ret;
}
static void led_exit(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(led); i++){
		gpio_free(led[i]);
	}
}

void led_write(unsigned long data)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(led); i++){
//		gpio_direction_output(led[i], (data >> i ) & 0x01);
		gpio_set_value(led[i], (data >> i ) & 0x01);
	}
#if DEBUG
	printk("#### %s, data = %ld\n", __FUNCTION__, data);
#endif
}
void led_read(unsigned long * led_data)
{
	int i;
	unsigned long data=0;
	unsigned long temp;
	for(i=0;i<4;i++)
	{
  		gpio_direction_input(led[i]); //error led all turn off
		temp = gpio_get_value(led[i]) << i;
		data |= temp;
	}
/*	
	for(i=3;i>=0;i--)
	{
  		gpio_direction_input(led[i]); //error led all turn off
		temp = gpio_get_value(led[i]);
		data |= temp;
		if(i==0)
			break;
		data <<= 1;  //data <<= 1;
	}
*/
#if DEBUG
	printk("#### %s, data = %ld\n", __FUNCTION__, data);
#endif
	*led_data = data;
//	led_write(data);
	return;
}

static int led_on(void)
{
	led_init();
	led_write(0x0f);
	return 0;
}

static void led_off(void)
{
	led_write(0x00);
	led_exit();
}
module_init(led_on);
module_exit(led_off);
MODULE_AUTHOR("KHG");
MODULE_DESCRIPTION("Test Module");
MODULE_LICENSE("Dual BSD/GPL");
