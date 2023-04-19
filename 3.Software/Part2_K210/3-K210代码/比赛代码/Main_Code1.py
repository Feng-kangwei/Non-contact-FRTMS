####################################################################
'''
文件：主文件草一

'''
import sensor, image, lcd, time
import KPU as kpu
#串口头文件
from fpioa_manager import fm
from machine import UART
from board import board_info
from fpioa_manager import fm
#led头文件
from fpioa_manager import *
from Maix import GPIO
######################################################################


#LCD初始化
lcd.init()
lcd.clear()

#摄像头初始化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)

#串口初始化
#串口二
fm.register(board_info.PIN10,fm.fpioa.UART2_TX)# 管脚映射  Pin10——UART2-TX
fm.register(board_info.PIN11,fm.fpioa.UART2_RX)#          Pin11——UART2-RX
uart2 = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len=4096)

#串口一
fm.register(board_info.PIN15,fm.fpioa.UART1_TX)
fm.register(board_info.PIN17,fm.fpioa.UART1_RX)
uart1 = UART(UART.UART1, 115200,8,0,0, timeout=1000, read_buf_len=4096)

#标签初始化
f=open('/sd/labels.txt','r')
labels=f.readlines()
f.close()

#人脸模型加载初始化
task = kpu.load('/sd/6bf1b010d89e01eda73800c4d00f2c1d_95a4cfaa505779a2118e70350fdc5ee8.smodel')
lcd.clear(0xFFFF)
clock = time.clock()

#led初始化
fm.register(board_info.LED_R, fm.fpioa.GPIO0)

led_r=GPIO(GPIO.GPIO0,GPIO.OUT)
led_r.value(1)

#全局变量
Face_RecogResult=0  #存放人脸识别结果
read_str=0
index_State=0
read_str_num=0
Mask_data=0
Mask_data_num=0
num=['1','2','3','4','5','6','7','8','9','0','53','48']

Self_Learn_Result=0
################################################################



##############################模式函数############################
'''
@函数名：
@函数功能：
@参数：
@返回值：
@笔记：
'''

def Face_Recognition():
    #人脸识别函数
    global Face_RecogResult  # 引入全局变量 存储人脸识别结果
    led_r.value(0)           #  红灯亮说明进入人脸检测模式
    img = sensor.snapshot()
    clock.tick()
    fmap = kpu.forward(task, img)
    fps=clock.fps()
    plist=fmap[:]
    pmax=max(plist)
    max_index=plist.index(pmax)
    a=img.draw_rectangle((26,26,162,162),color=(0,255,0),thickness=3)
    img.draw_string(178,190, "%s"%(labels[max_index].strip()),color=(255,0,0),scale=2)
    a = lcd.display(img, oft=(48,0))
    lcd.draw_string(48, 224, "%.2f:%s"%(pmax, labels[max_index].strip()))
    lcd.draw_string(10, 10, "fps:%f"%(fps))

    Face_RecogResult=labels[max_index].strip() #将人脸识别结果給全局变量

    return 'Func finish'
#################################################################

# 主循环
while(True):
    img = sensor.snapshot()
    clock.tick()
    ###########串口2接收上位机人脸检测指令####
    read_data = uart2.read(10)   #读缓冲区
    if(read_data):
        index_State=read_data
        print(index_State)
    #####################################


    ###########串口1接收口罩信息############
    Mask_data = uart1.read(10)   #读缓冲区
    if(Mask_data):
        Mask_data=Mask_data.decode("utf-8")
        Mask_data=Mask_data[0]
        #print("原数据为：%s"%(Mask_data[0]))
        if(Mask_data not in num):
            Mask_data='0'
        Mask_data_num=int(eval(Mask_data[0]))
        #print("接收口罩数据为：%d"%(Mask_data_num))
    #####################################


    ###############模式判别###############


    if(index_State==b'1'):     #1:表示开始人脸检测
        Run_State=Face_Recognition() #Run_State:显示当前模式状态的字符串  并发送给stm32的串口
        led_r.value(0)

        if(Face_RecogResult[0]=="F"):    # 结果如果是FKW
            if(Mask_data_num==5):
                uart2.write("1")
            elif(Mask_data_num==0):
                uart2.write("2")
        if(Face_RecogResult[0]=="W"):    # 结果是WXY
            if(Mask_data_num==5):
                uart2.write("3")
            elif(Mask_data_num==0):
                uart2.write("4")
        if(Face_RecogResult[0]=="Z"):    # 结果是ZZL
            if(Mask_data_num==5):
                uart2.write("5")
            elif(Mask_data_num==0):
                uart2.write("6")

    if(index_State==b'2'):     #2.表示收到结束信息
        led_r.value(1)      #红灯灭
        Face_RecogResult=0
        Mask_data_num=0
'''
    if(index_State==3):     #3.表示收到串口屏自学习指令 进入自学习模式
        uart1.write("1")    # 向openmv 写 1 表示进入自学习模式
        Self_Learn_Result = uart1.read(10)   #读缓冲区
        if(Self_Learn_Result):
            Self_Learn_Result=Self_Learn_Result.decode("utf-8")
            Self_Learn_Result=Self_Learn_Result[0]
            #print("自学习结果为：%s"%(Mask_data[0]))

    if(index_State==4):
        uart1.write("2")    # 向openmv 写 2 表示退出自学习模式
'''



    #####################################
a = kpu.deinit(task)
