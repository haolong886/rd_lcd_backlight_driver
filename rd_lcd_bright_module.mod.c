#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x42c712ef, "module_layout" },
	{ 0x1d2e0a15, "kobject_put" },
	{ 0x51d50f74, "i2c_master_send" },
	{ 0x97255bdf, "strlen" },
	{ 0xafe4e9c3, "i2c_del_driver" },
	{ 0x1a9df6cc, "malloc_sizes" },
	{ 0x64134fdf, "kobject_del" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xdd898587, "sysfs_remove_group" },
	{ 0x1a01225a, "kobject_create_and_add" },
	{ 0xea147363, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0xc1e5077d, "sysfs_create_group" },
	{ 0x24c36704, "sysfs_remove_link" },
	{ 0xd2cf0711, "i2c_register_driver" },
	{ 0x91c1c34c, "sysfs_create_link" },
	{ 0xbb4f6c53, "kmem_cache_alloc" },
	{ 0x82dd1188, "dev_driver_string" },
	{ 0x7b8131a0, "i2c_master_recv" },
	{ 0x37a0cba, "kfree" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5295B28DA2959B3866099DE");
