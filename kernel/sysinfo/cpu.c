void cpu_info(void)
{
        struct proc_info_list *list;

        /*
         * locate processor in the list of supported processor
         * types.  The linker builds this table for us from the
         * entries in arch/arm/mm/proc-*.S
         */
        list = lookup_processor_type(read_cpuid_id());
        if (!list) {
                printk("CPU configuration botched (ID %08x), unable "
                       "to continue.\n", read_cpuid_id());
                while (1);
        }

        cpu_name = list->cpu_name;
        __cpu_architecture = __get_cpu_architecture();


        printk("CPU: %s [%08x] revision %d (ARMv%s), cr=%08lx\n",
               cpu_name, read_cpuid_id(), read_cpuid_id() & 15,
               proc_arch[cpu_architecture()], cr_alignment);


        unsigned int cpu = smp_processor_id();
        struct stack *stk = &stacks[cpu];
}
