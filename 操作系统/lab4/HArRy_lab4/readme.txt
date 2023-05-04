Main.c中增加了task4的测试代码:
RunThreadManager2Tests
TestThreadManager2
其中调用的是threadManager2
（定义在Kernel.h）

threadManager2与threadManager的不同在于：
① mutex -> HoareMutex
② condition -> HoareCondition
③ while -> if
④ 上锁、解锁增加了条件
详细内容在pdf中说明。

如果有认为觉得不对的地方，请联系我：
qq:1032155695
