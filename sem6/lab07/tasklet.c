#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/atomic.h>

#define HANDLED_IRQ 1

MODULE_AUTHOR("ivan");
MODULE_DESCRIPTION("lab7");
MODULE_LICENSE("GPL");


char my_tasklet_data[]="data";
static unsigned long counter = 0;

void tasklet_fn( unsigned long data );

// регистрируем тасклет статитчески
// создаем тасклет который разрешен для выполнения
// my_tasklet_data - передается в обработчик тасклета при вызове
DECLARE_TASKLET( my_tasklet, tasklet_fn, (unsigned long) &my_tasklet_data );

void tasklet_fn( unsigned long data ) 
{
	counter++;
	printk(KERN_INFO "+ interrupt %ld) - state: %ld, count %d, data:%s\n", counter, my_tasklet.state, atomic_read(&my_tasklet.count), (char *)data );
	return; 
}

// irq - номер прерывания
/* dev_id - Т.к. это указатель на void, может
указывать на что угодно. Обычно используется
указатель на структуру описывающую устройство. В
нашем случае используем указатель на обработчик. 

используется для разделения линии прерывания, когда обработчик прерывания освобождается, dev_id обеспечивает
уникальные куки-файлы
чтобы выполнить удаление только нужного обработчика прерывания с соответствующей линии прерывания
*/
static irqreturn_t irq_handler( int irq, void *dev_id ) {
	if (irq == HANDLED_IRQ)
	{
		// планируем тасклет на выполнение 
		// используем отложенное прерывание TASKLET_SOFTIRQ
		tasklet_schedule( &my_tasklet );
		return IRQ_HANDLED; // прерывание обработно 
	}
   	else
   		return IRQ_NONE; // прерывание не обработано
}


static int __init tasklet_int_init(void)
{
	printk(KERN_INFO "!!! loaded.");
	// регистрируют обработчик аппаратного прерывания и 
	// разрешают определенную линию irq:
    return request_irq(HANDLED_IRQ, // номер irq
					   (irq_handler_t)irq_handler, // наш обработчик 
					   IRQF_SHARED, // флаг, разрешает разделение irq несколькими устройствами
					   "klava_irq_handler", // имя устройства
					   (void*)(irq_handler)); // идентификатор устройства
}

static void __exit tasklet_int_exit(void)
{
	printk(KERN_INFO "!!! unloaded.");
	tasklet_kill(&my_tasklet);
	free_irq( HANDLED_IRQ, (void*)(irq_handler));
}	

module_init(tasklet_int_init);
module_exit(tasklet_int_exit);

