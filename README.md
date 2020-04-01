## 潮流计算程序

### 要求

1. 据给定的潮流计算任务书整理潮流计算的基础数据：节点的分类，线路模型，等值变压器模型，电压等级的归算，标幺值的计算；

2. 基础数据的计算机存储：节点数据，支路数据（包括变压器）；

3. 用牛顿-拉夫逊法计算；

4. 根据所选潮流计算方法画流程图，划分出功能模块，有数据输入模块，导纳阵形成模块，解线性方程组模块，计算不平衡功率模块，形成雅可比矩阵模块，解修正方程模块，计算线路潮流。
   1. 每次迭代的各节点电压幅值、相位或者实部、虚部
   2. 收敛的迭代次数
   3. 收敛后各节点电压幅值、相位，各支路的S<sub>ij</sub>，S<sub>ji</sub>，ΔS<sub>ij</sub>
   4. 收敛后PV节点的注入Q
   5. 收敛后平衡节点的注入功率S

5. 据上述模块编制程序并上机调试程序，得出潮流计算结果；



### 输入输出

- 输入：
  - ./9-Nodes-Data/Bus.txt 
  - ./9-Nodes-Data/Branch.txt
- 输出：
  - Origin Data.txt
  - Matrix Y.txt
  - Results.txt
  - Balance PQ.txt
  - BranchPQ.txt
  - BranchLoss.txt



### 项目文件

- 头文件：
  - NEquation.h：解方程的类和方法
  - FlowFunction.h：计算调用函数的声明
  - DataForm.h：声明数据结构与变量，初始化一些变量

- 源文件：
  - PowerFlowMain.cpp：潮流计算主程序
  - FlowFunction.cpp：计算调用函数的定义