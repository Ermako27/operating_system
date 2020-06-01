#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

#include <linux/init.h>
#include <linux/proc_fs.h>

#include <asm/atomic.h>

#define HANDLED_IRQ 1

MODULE_AUTHOR("Ivan");
MODULE_DESCRIPTION("Operating Systems lab8");
MODULE_LICENSE("GPL");

char work_data[] = "data";
static unsigned long counter = 0;

struct workqueue_struct *my_wq;
// ems1828
void work_fn(struct work_struct *work);

typedef struct {
  struct work_struct work;
  char *x;
} work_type;

work_type *work;

void work_fn(struct work_struct *work) {
	work_type *tmp_work = (work_type *)work;
	counter++;
    printk(KERN_INFO "+ work: %ld data: %s\n",counter,tmp_work->x);
	return;
}

static irqreturn_t irq_handler( int irq, void *dev_id ) {
	if (irq == HANDLED_IRQ)
	{
		// ставим ворк в очередь
		// ставит ворк на выполнение планировщик задач
		queue_work( my_wq, (struct work_struct *)work );
		return IRQ_HANDLED;
	}
   	else
	{
		return IRQ_NONE;
	} 
}


static int __init work_int_init(void)
{
	// регистрируют обработчик аппаратного прерывания и 
	// разрешают определенную линию irq:
	if (request_irq(HANDLED_IRQ,
		(irq_handler_t)irq_handler,
		IRQF_SHARED,
		"klava_irq_handler",
		(void*)(irq_handler)))
	{
		return -1;
	} 
	else 
	{
		/*
		WQ_UNBOUND
		В привязанных очередях work’и при добавлении привязываются к текущему CPU, то есть в таких очередях work’и исполняются на том ядре, 
		которое его планирует ( на котором выполнялся обработчик прерывания ). В непривязанных очередях work’и могут исполняться на любом ядре
		*/
		my_wq = alloc_workqueue("queue", WQ_UNBOUND, 0);
		if(!my_wq)
		{
			return -1;
		}
		/*
		GFP_KERNEL, означает, что выделение роизводится от имени процесса, запущенного в пространстве ядра. 
		Иными словами, это означает, что вызывающая функция выполняет системный вызов от имени процесса. 
		Использование GFP_KERNEL означает, что kmalloc может поместить текущий процесс в сон для ожидания страницы при вызове в ситуациях недостатка памяти. 
		*/
		work = (work_type*)kmalloc(sizeof(work_type),GFP_KERNEL);
		if (!work)
		{
			return 0;
		}
		// задаем структуру work_struct динамически
		INIT_WORK((struct work_struct *)work, work_fn );
		work->x = work_data;
		printk(KERN_INFO "+ Module loaded.");
		return 0;
	}
}

static void __exit work_int_exit(void)
{
	printk(KERN_INFO "+ Module unloaded.");
	flush_workqueue( my_wq );
	destroy_workqueue( my_wq );
	free_irq( HANDLED_IRQ, (void*)(irq_handler));
}	

module_init(work_int_init);
module_exit(work_int_exit);

