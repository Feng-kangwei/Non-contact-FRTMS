
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
import utime
import gc
######################################################################


#LCD初始化
lcd.init()
lcd.clear()

#摄像头初始化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_hmirror(1) #自学习
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
byte16=bytearray([0xff,0xff,0xff])



#自学习变量及初始化
fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)
key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)
start_processing = False

BOUNCE_PROTECTION = 50

def set_key_state(*_):
    global start_processing
    start_processing = True
    utime.sleep_ms(BOUNCE_PROTECTION)

key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)

anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437,
          6.92275, 6.718375, 9.01025)  # anchor for face detect
dst_point = [(44, 59), (84, 59), (64, 82), (47, 105),
             (81, 105)]  # standard face key point position


img_lcd = image.Image()
img_face = image.Image(size=(128, 128))
a = img_face.pix_to_ai()
record_ftr = []
record_ftrs = []
names = ['FKW', 'WXY', 'ZZL', 'USER1', 'USER2',
         'USER3', 'USER4', 'USER5', 'USER6', 'USER7']

ACCURACY = 85



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
    task = kpu.load('/sd/6bf1b010d89e01eda73800c4d00f2c1d_95a4cfaa505779a2118e70350fdc5ee8.smodel')
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


def Self_Learn():

    task_fd = kpu.load('/sd/FD.smodel')
    task_ld = kpu.load('/sd/KP.smodel')
    task_fe = kpu.load('/sd/FE.smodel')
    a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor)

    code = kpu.run_yolo2(task_fd, img)
    if code:
        for i in code:
        # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect())
            face_cut = img.cut(i.x(), i.y(), i.w(), i.h())
            face_cut_128 = face_cut.resize(128, 128)
            a = face_cut_128.pix_to_ai()
            # a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128)
            plist = fmap[:]
            le = (i.x() + int(plist[0] * i.w() - 10), i.y() + int(plist[1] * i.h()))
            re = (i.x() + int(plist[2] * i.w()), i.y() + int(plist[3] * i.h()))
            nose = (i.x() + int(plist[4] * i.w()), i.y() + int(plist[5] * i.h()))
            lm = (i.x() + int(plist[6] * i.w()), i.y() + int(plist[7] * i.h()))
            rm = (i.x() + int(plist[8] * i.w()), i.y() + int(plist[9] * i.h()))
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4)
            # align face to standard position
            src_point = [le, re, nose, lm, rm]
            T = image.get_affine_transform(src_point, dst_point)
            a = image.warp_affine_ai(img, img_face, T)
            a = img_face.ai_to_pix()
            # a = img.draw_image(img_face, (128,0))
            del (face_cut_128)
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face)
            feature = kpu.face_encode(fmap[:])
            reg_flag = False
            scores = []
            for j in range(len(record_ftrs)):
                score = kpu.face_compare(record_ftrs[j], feature)
                scores.append(score)
            max_score = 0
            index = 0
            for k in range(len(scores)):
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k
            if max_score > ACCURACY:
                a = img.draw_string(i.x(), i.y(), ("%s :%2.1f" % (
                    names[index], max_score)), color=(0, 255, 0), scale=2)
            else:
                a = img.draw_string(i.x(), i.y(), ("X :%2.1f" % (
                    max_score)), color=(255, 0, 0), scale=2)
            if start_processing:
                record_ftr = feature
                record_ftrs.append(record_ftr)
                start_processing = False

            break
    fps = clock.fps()
    print("%2.1f fps" % fps)
    a = lcd.display(img)
    gc.collect()
    # kpu.memtest()

    # a = kpu.deinit(task_fe)
    # a = kpu.deinit(task_ld)
    # a = kpu.deinit(task_fd)


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
                uart2.write("t0.txt=\"FKW\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"Mask\"")
                uart2.write(byte16)
            elif(Mask_data_num==0):
                uart2.write("t0.txt=\"FKW\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"NO Mask\"")
                uart2.write(byte16)
        if(Face_RecogResult[0]=="W"):    # 结果是WXY
            if(Mask_data_num==5):
                uart2.write("t0.txt=\"WXY\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"Mask\"")
                uart2.write(byte16)
            elif(Mask_data_num==0):
                uart2.write("t0.txt=\"WXY\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"NO Mask\"")
                uart2.write(byte16)
        if(Face_RecogResult[0]=="Z"):    # 结果是ZZL
            if(Mask_data_num==5):
                uart2.write("t0.txt=\"ZZL\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"Mask\"")
                uart2.write(byte16)
            elif(Mask_data_num==0):
                uart2.write("t0.txt=\"ZZL\"")
                uart2.write(byte16)
                uart2.write("t2.txt=\"Mask\"")
                uart2.write(byte16)
    if(index_State==b'2'):     #2.表示收到结束信息
        led_r.value(1)      #红灯灭
        Face_RecogResult=0
        Mask_data_num=0
    if(index_State==b'3'):     #3.表示收到串口屏自学习指令 进入自学习模式
        Self_Learn()





    #####################################
a = kpu.deinit(task)


byte16=bytearray([0xff,0xff,0xff])
