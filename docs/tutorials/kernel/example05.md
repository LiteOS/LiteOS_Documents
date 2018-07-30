# 范例05（Task综合）

Task1(高)创建Task2(低)，Task1会抢占低优先级Task2，2s后，Task1会自动挂起，Task2开始执行，完成后会调用vTaskDelete删除自己。

使用vTaskPrioritySet/vTaskPriorityGet来修改获取其他任务的优先级