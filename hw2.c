#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>
//#include <errno.h>

asmlinkage long sys_hello(void) {
 printk("Hello, World!\n");
 return 0;
}

asmlinkage long sys_set_weight(int weight){
    if(weight < 0){
        return -EINVAL;
    }
    current->weight = weight;
    return 0;
}

asmlinkage long sys_get_weight(void){
  printk("weight is %ld \n, my pid is %d",current->weight,current->pid);
  return current->weight;
}

long recursive_get_leaf(struct task_struct *root_task){
    struct task_struct *task;
    struct list_head *list;
    long sum=0;
    if(list_empty(&root_task->children)){
        return root_task->weight;
    }
    list_for_each(list, &root_task->children){
        task = list_entry(list, struct task_struct, sibling);
        sum += recursive_get_leaf(task);
    }
    return sum;
}


asmlinkage long sys_get_leaf_children_sum(void){
    if(list_empty(&current->children)){
        return -ECHILD;
    }
    return recursive_get_leaf(current);
}


asmlinkage long sys_get_heaviest_ancestor(void){
    long max_pid = 0;
    long max = 0;
    struct task_struct* current_task = current;
    while(current_task->pid != 1){
        if(current_task->weight > max){
            max = current_task->weight;
            max_pid = current_task->pid;
        }
        current_task = current_task->parent;
    }
    return max_pid;
}

