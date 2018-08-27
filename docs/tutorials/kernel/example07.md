# 范例07（任务综合）

Task1(低)创建Task2(高)，Task2会抢占低优先级Task1，Task2开始执行，完成后会调用vTaskDelete删除自己。Task1重新运行