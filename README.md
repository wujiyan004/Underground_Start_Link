## 稳健的星链定位与通信系统设计 
**Start-Link** 这是一个使用定制化的UWB系统设计的一个实时的同步定位与实时通信的系统。我愿称之为星链。是UWB与超宽带惯性系统的一种鲁棒的实时初始化方法。所提出的方法校准了LiDAR和IMU之间的时间偏移和外部参数，以及重力矢量和IMU偏差。我们的方法不需要任何目标或额外的传感器、特定的结构化环境、先验环境点图或外部和时间偏移的初始值。我们的方案解决了以下关键问题:

1. A robust UWB/Imu/LiDAR odometry (**Start-Link**) modified from Start-Link.
2. Fast and robust **temporal offset and extrinsic parameter calibration** between UWB and IMU without any hardware setup.
3. Support **multiple UWB types**: Within the coverage area of Starlink, multiple robots equipped with Starlink devices can achieve multi to multi real-time 3D high-precision, high-frequency (50hz) positioning and communication
4. Seamlessly merged into Start-Link, as a robust initialization module.

**Contributors**: [Jiyu Wang 王继禹](https://github.com/wujiyan004/Start_Link)




<div align="center"><img src="image/11.jpg" width=50% /></div>


> Demo video link: <https://b23.tv/J1yiU1J>



### Pipeline

<div align="center"><img src="image/jizhan.png" width=100% /></div>

<div align="center"><img src="image/startlink.png" width=100% /></div>

<div align="center"><img src="image/startlink8.0.png" width=100% /></div>

### Excite the Sensors

<div align="center"><img src="image/Excitation.png" width=100% /></div>

### Related Paper

<div align="center"><img src="image/xiaolv.jpg" width=100% /></div>

our related papers are now available:  [Robust Real-time LiDAR-inertial Initialization](https://ieeexplore.ieee.org/document/9982225)

If our code is used in your project, please cite our paper following the bibtex below:
```
@inproceedings{zhu2022robust,
  title={Robust real-time lidar-inertial initialization},
  author={Zhu, Fangcheng and Ren, Yunfan and Zhang, Fu},
  booktitle={2022 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS)},
  pages={3948--3955},
  year={2022},
  organization={IEEE}
}
```
<div align="center"><img src="image/tx.jpg" width=100% /></div>
### Related Video:

our accompanying videos are now available on **YouTube** (click below images to open) and [Bilibili](https://www.bilibili.com/video/BV1Zz411i7CX/?spm_id_from=333.1387.upload.video_card.click&vd_source=5bf2994b3504ab19bc2368b1accdc012).

<div align="center">
    <a href="https://www.youtube.com/watch?v=WiHgcPpKwvU" target="_blank">
    <img src="image/video_cover.png" width=70% />
</div>

<div align="center"><img src="image/fx.jpg" width=100% /></div>




<div align="center"><img src="image/图片1.png" width=100% /></div>

<div align="center"><img src="image/图片1.png" width=100% /></div>
<div align="center"><img src="image/图片2.png" width=100% /></div>
<div align="center"><img src="image/图片3.png" width=100% /></div>
<div align="center"><img src="image/图片4.png" width=100% /></div>
<div align="center"><img src="image/图片5.png" width=100% /></div>
<div align="center"><img src="image/图片6.png" width=100% /></div>
<div align="center"><img src="image/图片7.png" width=100% /></div>
<div align="center"><img src="image/图片8.png" width=100% /></div>



<div style="display: flex; justify-content: space-between;">
  <img src="./image/图片10.png" width="43%" />
  <img src="./image/图片9.png" width="55.2%" />
</div>


<div style="display: flex; justify-content: space-between;">
  <img src="./image/图片11.png" width="38%" />
  <img src="./image/图片12.png" width="60.5%" />
</div>
<div style="display: flex; justify-content: space-between;">
  <img src="./image/图片13.png" width="41%" />
  <img src="./image/图片14.png" width="56.5%" />
</div>
<div style="display: flex; justify-content: space-between;">
  <img src="./image/图片15.png" width="41%" />
  <img src="./image/图片16.png" width="57%" />
</div>






<div align="center"><img src="image/anysis.png" width=100% /></div>
<div align="center"><img src="image/anysis2.png" width=100% /></div>






<div align="center"><img src="image/公式1.png" width=100% /></div>






## 1. Prerequisites

### 1.1 **Ubuntu王继禹点灯** and **ROS**

Ubuntu >= 18.04.

ROS    >= Melodic. [ROS Installation](http://wiki.ros.org/ROS/Installation)
<div align="center"><img src="image/sl1.png" width=100% /></div>
### 1.2. **PCL && Eigen**

PCL    >= 1.8,   Follow [PCL Installation](http://www.pointclouds.org/downloads/linux.html).

Eigen  >= 3.3.4, Follow [Eigen Installation](http://eigen.tuxfamily.org/index.php?title=Main_Page).

### 1.3. **livox_ros_driver**

Follow [livox_ros_driver Installation](https://github.com/Livox-SDK/livox_ros_driver).

*Remarks:*

- Since the **LI_Init** must support Livox serials LiDAR firstly, so the **livox_ros_driver** must be installed and **sourced** before run any LI_Init luanch file.
- How to source? The easiest way is add the line `source $Livox_ros_driver_dir$/devel/setup.bash` to the end of file `~/.bashrc`, where `$Livox_ros_driver_dir$` is the directory of the livox_ros_driver workspace (should be the `ws_livox` directory if you completely followed the livox official document).

###  **1.4. ceres-solver**

Our code has been tested on [ceres-solver-2.0.0](http://ceres-solver.org/installation.html#linux). Please download ceres-solver  following the instructions.

### **1.5. 精度提升方案**  

修正方案：1.完成了晶振的修改替换，通过示波器和频率测量仪对38.4Mhz温度补偿晶振进行二次校准。2.通过RSSI_N,通过RSSI_C对功率进行补偿，通过STM32_MCU的高精度12位ADC采样，剔除MCU功率器件与DW1000芯片的功耗，计算出天线端发射功率，通过已知天线的硬件增益，计算补偿误差。3.自定义96字节测距源码，获得较低的前导码，并且加快TR发射时间，对测距精度影响进行降低。4.校准了不同距离下RSSI扩散图对精度的影响。5.对于多路径效应带来的误差，我们通过对PA放大电路进行灵敏性设计，尽可能最优化第一路径识别。
For more information, you can check [docker_start.md](./docker/docker_start.md).  

## 2. 误差校准步骤

Clone the repository and catkin_make:
初步确定测距误差来源：1.温补晶振对测距精度和稳定度的影响。2.车辆角度影响测距精度3.信号增益对测量精度的影响。4.天线信噪比和辐射图对测量精度的影响。5.天线延迟对测量精度的影响6.信号第一路径与增强反射路径电压比对测量精度的影响。
```
cd ~/catkin_ws/src
git clone https://github.com/hku-mars/LiDAR_IMU_Init.git
cd ..
catkin_make -j
source devel/setup.bash
```

## 3. Run Your Own Data
<div align="center"><img src="image/pl.jpg" width=100% /></div>
**Please make sure the unit of your input angular velocity is rad/s.** If it is degree/s, please refer to https://github.com/hku-mars/LiDAR_IMU_Init/issues/43.

**Please make sure the parameters in config/xxx.yaml are correct before running the project.**

**It is highly recommended to stay still for more than 5 seconds after launch the algorithm, for accumulating dense initial map.**

It is highly recommended to run LI-Init and record your own data simultaneously, because our algorithm is able to automatically detect the degree of excitation and instruct users how to give sufficient excitation (e.g. rotate or move along which direction).

Theoretically livox_avia.launch supports mid-70, mid-40 LiDARs.

**Note:** The code of LI-Init contains the initialization module and sequential FAST-LIO. If you run the code of LI-Init, it will first do initialization (if suffienct excitation is given, it will tell you the extrinsic transformation and temporal offset) and then it will switch into FAST-LIO. **Thus, if you want to run FAST-LIO on your own data but unfortunately the LiDAR and IMU are not synchronized or calibrated before, you can directly run LI-Init**. As for R3LIVE, you can write the extrinsic and temporal offset between LiDAR and IMU obtained by LI-Init into the config file of R3LIVE.

### Important parameters

Edit `config/xxx.yaml` to set the below parameters:

* `lid_topic`:  Topic name of LiDAR pointcloud.
* `imu_topic`:  Topic name of IMU measurements.

* `cut_frame_num`: Split one frame into sub-frames, to improve the odom frequency. Must be positive integers.
* `orig_odom_freq` (Hz): Original LiDAR input frequency. For most LiDARs, the input frequency is 10 Hz. It is recommended that cut_frame_num * orig_odom_freq = 30 for mechinical spinning LiDAR,  cut_frame_num * orig_odom_freq = 50 for livox LiDARs.
* `mean_acc_norm` (m/s^2):  The acceleration norm when IMU is stationary. Usually, 9.805 for normal IMU, 1 for livox built-in IMU.
* `data_accum_length`: A threshold to assess if the data is enough for initialization. Too small may lead to bad-quality results.
* `online_refine_time` (second):  The time of extrinsic refinement with FAST-LIO2. About 15~30 seconds of refinement is recommended.
* `filter_size_surf` (meter):  It is recommended that filter_size_surf = 0.05~0.15 for indoor scenes, filter_size_surf = 0.5 for outdoor scenes.
* `filter_size_map` (meter): It is recommended that filter_size_map = 0.15~0.25 for indoor scenes, filter_size_map = 0.5 for outdoor scenes.



After setting the correct topic name and parameters, you can directly run **LI-Init** with your own data..

```
cd catkin_ws
source devel/setup.bash
roslaunch lidar_imu_init xxx.launch
```

After initialization and refinement finished, the result would be written into `catkin_ws/src/LiDAR_IMU_Init/result/Initialization_result.txt`

## 4. 星链工作 原理示例

Download our test bags here: [UWB IMU Initialization Datasets](https://connecthkuhk-my.sharepoint.com/:f:/g/personal/zhufc_connect_hku_hk/EgdJ_F763sVOnkUNBRv-op8BmNL7eZrxETu2zSEAoiRX4A?e=cbNiJI).

Use `rosbag info xxx.bag` to get the correct topic name.

**对于星链基站端**: 多基站分配不同ID，基站ID分配方式：示例如下图所示：

<div align="center"><img src="image/ID.png" width=100% /></div>

* `1.`:  其中0xCA和0xED代表整个星链网络的组网ID

* `2.`:  其中0x00和0x02代表整个星链网络每个基站的固定ID（0-65535）（对于移动端而言）

* `3.`:  其中'S'和'C '代表整个星链网络每个移动端的固定ID（0-65535）（对于移动端而言）

* `4.`:  其中最后面的8个字节作为CAN总线网络的用户自定义数据

* `5.`:  其中最后17个字节作为内部传输通信传输测距数据、基站三维坐标、UWB时间戳差分数据
以上是空中协议内容。

<div align="center"><img src="image/juzhen.png" width=10% /></div>

上图是多基站与多移动端通过分时原理进行交错通信与定位示意图。


**对于星链移动端**: 多移动端分配不同ID，以实现不同人员和车辆的区分和通信
* `1.`:  ID从0-500分配给车辆ID，500-35535分配给人员ID使用，其余预留给井下各种设备使其能够接入星链网络，暂时不予定义

* `2.`:  移动端带有本地CAN网络接口，外部设备可搭载一个移动设备或者直连基站，使其数据能够上传到井上管控平台进行管理


<div align="center"><img src="image/dw1000.jpeg" width=100% /></div>


## 5. 高精度定位算法详解

* `1.`:  移动站开机时与所有基站进行交互一次，得到N个测距结果，按照最近原则锁定最近的三个基站进行定位


* `2.`:  移动端分别发送三次测距请求给三个最近ID的基站得到基站的实际XYZ数据和与这三个基站的测距距离D1、D2、D3

* `3.`:  通过三点加权得到虚拟基站坐标X`Y`Z`，然后按照虚拟坐标为坐标原点分别在XYZ三个方向每隔5cm部撒一个粒子（坐标），一共在长宽高为3米的空间内部署216000个粒子，最后通过贪心算法对（得到的三基站距离）标准差误差进行排序算法处理，最后得到几个最优解，按照矢量方差方法判断最可能得方位，最终得到最优解的坐标XYZ

* `4.`:  多基站切换时，可能由于误差、测量方法等原因导致XYZ在切换时产生突变，由于车辆在XYZ三个维度上是线性化的，因此对结果XYZ三个数据分别进行线性卡尔曼滤波进行线性化处理，最终使得车辆在行驶过程中丝滑稳定运行。

Thanks for [HKU MaRS Lab](https://github.com/hku-mars),  [Fast-LIO2](https://github.com/hku-mars/FAST_LIO) (Fast Direct LiDAR-inertial Odometry) and [ikd-tree](https://github.com/hku-mars/ikd-Tree).

Thanks for [Livox Technology](https://www.livoxtech.com/) for equipment support.



## 6. 星链校准测试

FAST-LIO最重要的参数是外部旋转和平移矩阵以及时间偏移。

对于相同的设备设置（IMU和激光雷达之间的相对姿态是固定的），您只需将外部的写入FAST-LIO的配置文件即可。

至于时间偏移，则取决于激光雷达和IMU的同步机制。对于pixhawk IMU，据我所知，时间戳是PC时间。如果激光雷达的时间戳也是PC时间，那么时间偏移可能是相同的。下次可以绕过临时初始化。但对于Livox avia/horizon等一些激光雷达来说，时间戳的起源是激光雷达开机的那一刻。因此，如果你关闭电源并再次开机，时间戳从0开始计数。在这种情况下，每次激光雷达通电时都需要进行时间初始化。因此，您可以运行一次LI Init，记录时间偏移；然后关闭激光雷达和IMU几分钟，然后打开它们并再次校准时间偏移。如果时间偏移量接近，则意味着下次可以绕过时间偏移初始化。只需将时间偏移量写入FAST-LIO的配置文件的time_diff_lidar_to_imu即可。

至于IMU偏差和重力，FAST-LIO可以在线对其进行细化。

## 7. License

The source code is released under [GPLv2](http://www.gnu.org/licenses/) license.

We are still working on improving the performance and reliability of our codes. For any technical issues, please contact us via email [zhufc@connect.hku.hk](mailto:zhufc@connect.hku.hk). For commercial use, please contact Dr. Fu Zhang [fuzhang@hku.hk](mailto:fuzhang@hku.hk).




# OpenRSSI
开源空间三点定位（含高度，非伪3d）系统。严格来说更适合UWB？  
使用了拟动量法计算误差。

## 典型应用

### 动作捕捉
不同于SlimeVR，本方案的追踪结果是0累计误差的。你大可以趴在房间里面睡觉，几个小时后位置并不会漂移导致你的形象扭成麻花。  
我们推荐使用BU01或者BU03的UWB模块；每个模块的参考报价是￥75...考虑到电池和盒子，可能100？注意一下，你需要在地上丢3个beacon。  
BU03的误差是10cm（或者说±5cm）。参考内建的仿真代码，动捕的最终精度是半径为6cm的球；可以轻松松突破100fps。  

### RSSI定位
RSSI转半径后最大的问题也是误差处理，感觉目前的文献都没什么人提这个...啧...  
具体RSSI转距离可以自己推算；值得一提的是**不推荐**通过只有beacon有RSSI功能，节点是蓝牙/Zigbee标签的方法来做。  
如你所见，节点之间的位置关系也很重要。

### 科研用途
不需要我说了吧？（x

## 可改进点

### 提升精度
仅需悬空地安装一个信标。我祝你校准时不会自闭（不是）  

### 简化安装
仅需先计算beacon位置即可。我在Num3d中为你留下了不少实用的函数，例如Rodrigues' rotation formula的实现 ：）  

### 减少计算
唉，嵌入式狂魔。好吧，我也说一下：  
仅需在减少lr的if中将1.9改成2.1即可。

## 一些注释

### 代码结构
PCloudAnswer: 点云重建问题，包含了mock数据生成器。  
PCloudQuestion： 点云重建结果，包含了点云压缩表示数据结构。  
Num3d： 3d库。包含了诸多我没用到，但是我觉得你用的到的函数。  
PhiSolver： 求解器。  
Main： 测试程序。  

### 有疑问？
请开启issue，看到了会回...但是不保证及时。  

## 公式与引用
为了方便论文人在自己的论文中引用，我还是写一下  
这些公式目前不属于任何论文的一部分，包括下面提到的那篇（唉我真是个大善人）  
LaTeX格式的，抄吧（  

Citation：  
AlvinChou @ WHU, Licensed under MIT.  
```bib
@article{202504.0345,
	doi = {10.20944/preprints202504.0345.v1},
	url = {https://doi.org/10.20944/preprints202504.0345.v1},
	year = 2025,
	month = {April},
	publisher = {Preprints},
	author = {Theresa Chen},
	title = {OpenRSSI: Zero-Drift Motion Capture Using Consumer-Grade UWB Hardware},
	journal = {Preprints}
}
```

