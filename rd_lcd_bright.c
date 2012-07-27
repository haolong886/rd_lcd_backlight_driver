#include<linux/module.h>
#include<linux/i2c.h>
#include<linux/err.h>
#include<linux/sysfs.h>
#include<linux/string.h>

#define DAC081_ADDR		0
#define I2C_REG_SIZE	2

#define BRIGHT_DBG(...)                  (printk(KERN_INFO __VA_ARGS__))
#define BRIGHT_ERR(...)                  (printk(KERN_ERR __VA_ARGS__))


typedef struct
{
	struct kobject *kobj;
	struct i2c_client *client;
}dac081_data;
dac081_data *dac081;

#define FORMAT_I2C	0
#define FORMAT_ARM  1
static void cvt_data_format(char const *data, int format)
{
	char *d = data;
	char tmp = 0;
	if(format == FORMAT_I2C)
	{
		tmp = *(d+1);
		*(d+1) = *d;
		*d = tmp;
	}
	if(format == FORMAT_ARM)
	{
		tmp = *(d);
		*d = *(d+1);
		*(d+1) = tmp;
	}
}

static ssize_t lcd_bright_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	unsigned int bright_level;
	struct i2c_client *client;
	int err = 0;
	client = dac081->client;
	err = i2c_master_recv(client, (char *)&bright_level, I2C_REG_SIZE);
	if(err != I2C_REG_SIZE)
	{
		BRIGHT_ERR("recv from dac081 err, err value = %d\n", err);
		return err;

	}
	
	bright_level &= 0xffff;
	cvt_data_format((char *)&bright_level, FORMAT_ARM);
	
	BRIGHT_DBG("read from i2c value = %x\n", bright_level);
	sprintf(s, "%x\n", bright_level);
	return strlen(s);//byte
}

static ssize_t lcd_bright_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
	unsigned int bright_level;
	int err = 0;
	struct i2c_client *client;
	client = dac081->client;
	err = sscanf(buf, "%x", &bright_level);
	bright_level = bright_level << 4;
	BRIGHT_DBG("send to i2c value = 0x%x, size = %d\n", bright_level, n);
	cvt_data_format((char *)&bright_level, FORMAT_I2C);
	err = i2c_master_send(client, (char *)&bright_level, I2C_REG_SIZE);
	if(err != I2C_REG_SIZE)
	{
		BRIGHT_ERR("send to dac081 error, err value = %d\n", err);
		return err;
	}
	return n;
}

static struct kobj_attribute kobj_attr_lcd_bright =
{
	.attr =
	{
		.name = __stringify(bright_level),
		.mode = 0666,
	},
	.show = lcd_bright_show,
	.store = lcd_bright_store,
};

static struct attribute *attr_g[] =
{
	&kobj_attr_lcd_bright.attr,
	NULL,
};

static struct attribute_group grp_attr =
{
	.attrs = attr_g,
};

static int dac081c081_probe(struct i2c_client *client,
			const struct i2c_device_id *dev_id)
{
	struct kobject *kobj;
	int err = 0;
	if (!(err = i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_WORD_DATA)))
		goto exit;
	
	dev_info(&client->dev, "chip found\n");
	dac081 = kzalloc(sizeof(dac081_data), GFP_KERNEL);
	kobj = kobject_create_and_add("lcd_bright", NULL);
	if(!kobj)
	{
		BRIGHT_ERR("lcd bright kobj didn't creat\n");
		err = (int)kobj;
		goto exit_kobj;
	}
	dac081->kobj = kobj;
	err = sysfs_create_link(kobj, &client->dev.kobj, "lcd_bright_level_link");
	if(err != 0)
		goto exit_link;
	err = sysfs_create_group(&client->dev.kobj, &grp_attr);
	if(err)
		goto exit_sysfs;
	dac081->client = client;
	//init brigt level
	return 0;
	
	exit_sysfs:
		sysfs_remove_link(kobj, "lcd_bright_level_link");
	exit_link:
		//delete kobject
		kobject_del(kobj);
		kobject_put(kobj);
	exit_kobj:
		kfree(dac081);
	exit:
		return err;
}

static int __devexit dac081c081_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &grp_attr);
	sysfs_remove_link(dac081->kobj, "lcd_bright_level_link");
	kobject_del(dac081->kobj);
	kobject_put(dac081->kobj);
	kfree(dac081);
	return 0;
}


static const struct i2c_device_id dac081c081_id[] = {
	{ "dac081c081", 0 },
	{}
};

static struct i2c_driver dac081c081_driver =
{
	.driver = 
	{
		.name = "dac081c081",
	},
	.probe = dac081c081_probe,
	.remove = __devexit_p(dac081c081_remove),
	.id_table = dac081c081_id,
};

static int __init dac081c081_init(void)
{
	return i2c_add_driver(&dac081c081_driver);
}

module_init(dac081c081_init);

static void __exit dac081c081_exit(void)
{
	return i2c_del_driver(&dac081c081_driver);
}

module_exit(dac081c081_exit);
MODULE_LICENSE("GPL");

