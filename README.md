## 1 介绍
这是一个基于esp32s3的rbfsdk演示示例，用户可以通过cli指令进行对rbfsdk的简单测试。

## 2 目录介绍
### 2.1 main 
测试代码目录
* main.c <br>
测试代码入口，注册了所有cli测试指令。
* rbftest.c <br>
封装了所有测试接口对rbfsdk的调用。

### 2.2 rbfsdk
rbfsdk目录 
* include
sdk头文件
* librbfsdk.a
rbfsdk链接静态库

rbfsdk的版本信息可以在rbf_api.h头文件中查看。


## 3 测试
### 3.1 编译
本例程经过idf-v5.3.1环境中测试 <br>
1. 导入idf开发环境, 其中esp-idf为你的esp32 idf sdk目录 <br>
`source esp-idf/export.sh`
2. 进入例程目录 <br>
`cd rbfsdk`
3. 设置esp32芯片型号<br>
`idf.py set-target esp32s3`
4. 编译 <br>
`idf.py build` 

### 3.2 烧写
请根据自己的环境将代码烧写到esp32硬件板子上。

### 3.3 测试指令
1. 初始化sdk<br>
`init`
2. 进入注册模式<br>
`enroll`
3. 操作子设备进入配网模式
4. 入网成功后, 查询子设备
`device`
5. IO设备批量布撤防
例： 批量设置从IO设备号0开始的3个设备进入布防状态 <br>
`setarming -e 0 3` <br>
例： 批量设置从IO设备号0开始的3个设备进入撤防状态 <br>
`setarming -d 0 3` <br>


通过help可以查看所有指令
