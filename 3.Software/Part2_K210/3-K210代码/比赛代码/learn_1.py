import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
import utime


#加载人脸模型
task_fd = kpu.load('/sd/FD.smodel')
task_ld = kpu.load('/sd/KP.smodel')
task_fe = kpu.load('/sd/FE.smodel')
clock = time.clock()

#初始化boot引脚部分
fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)
key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)
start_processing = False


BOUNCE_PROTECTION = 50


def set_key_state(*_):
    global start_processing
    start_processing = True
    utime.sleep_ms(BOUNCE_PROTECTION)


key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)


# 初始化
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1)
sensor.set_vflip(1)
sensor.run(1)
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437,
          6.92275, 6.718375, 9.01025)  # anchor for face detect
dst_point = [(44, 59), (84, 59), (64, 82), (47, 105),
             (81, 105)]  # standard face key point position


a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor) #初始化人脸模型
img_lcd = image.Image()  #设置显示buff
img_face = image.Image(size=(128, 128)) #设置128*128 人脸的图片buff
a = img_face.pix_to_ai()  #将图片转化为KPU可以接受的格式
record_ftr = []          #用于存储当前196维特征
record_ftrs = []         #存储按键记录的人脸特征   可以将这个列表的值以txt形式读到SD卡中可以实现模型保存
#record_ftrs=open('/sd/featurn.txt','r')

names = ['FKW', 'WXY', 'ZZL', 'USER1', 'USER2',          #标签名
         'USER3', 'USER4', 'USER5', 'USER6', 'USER7']

ACCURACY = 85


################################串口初始化####################################
#串口二
fm.register(board_info.PIN10,fm.fpioa.UART2_TX)# 管脚映射  Pin10——UART2-TX
fm.register(board_info.PIN11,fm.fpioa.UART2_RX)#          Pin11——UART2-RX
uart2 = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len=4096)

#串口一
fm.register(board_info.PIN15,fm.fpioa.UART1_TX)
fm.register(board_info.PIN17,fm.fpioa.UART1_RX)
uart1 = UART(UART.UART1, 115200,8,0,0, timeout=1000, read_buf_len=4096)
#############################################################################


###############################下位机及上位机的通讯变量###########################################
read_data=0   #存放串口屏指令
index_State=0 #串口屏指令 用于判断

Mask_data=0     #openMV信息
Mask_data_num=0 #转换为数字的口罩信息


##################################################主循环##########################################################
while (1):

    img = sensor.snapshot()
    clock.tick()
    #######串口2接收上位机人脸检测指令#######
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
        Mask_data_num=int(eval(Mask_data[0]))
        #print("接收口罩数据为：%d"%(Mask_data_num))
    #####################################
    #二级
    code = kpu.run_yolo2(task_fd, img)
    if code:
        #三级
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
                    a = img.draw_string(i.x(), i.y(), ("%s :%2.1f" % (names[index], max_score)), color=(0, 255, 0), scale=2)
                    #串口屏按下开始
                    if(index_State==b'1'):
                #################结果发送上位机####################
                        Face_Recognition=names[index]  #读取结果
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
                        read_data = uart2.read(10)   #读缓冲区
                        if(read2_data):
                            old_index_State=read2_data
                            if old_index_State==b'6':     #进入判别领域   说明是FKW
                                if(Face_RecogResult[0]!="F"):      #如果检测到的是FKW
                                    uart2.write("click b3,1")
                                    uart2.write(byte16)

                            if old_index_State==b'7':         #WXY
                                if(Face_RecogResult[0]!="W"): #如果检测到的不是WXY
                                    led_r.value(1)
                                    uart2.write("click b3,1")
                                    uart2.write(byte16)
                            if old_index_State==b'8':
                                if(Face_RecogResult[0]!="Z"):      #如果检测到的不是WXY
                                    uart2.write("click b3,1")
                                    uart2.write(byte16)
                            if old_index_State==b'2':
                                index_State=read2_data


                else:
                    a = img.draw_string(i.x(), i.y(), ("X :%2.1f" % (max_score)), color=(255, 0, 0), scale=2)
                if start_processing:
                    record_ftr = feature
                    record_ftrs.append(record_ftr)
                    start_processing = False
                    #record_ftrs.save("sd/featurn.txt")
                break
    #一级
    fps = clock.fps()
    print("%2.1f fps" % fps)
    a = lcd.display(img)
    gc.collect()
    # kpu.memtest()

#循环外
a = kpu.deinit(task_fe)
a = kpu.deinit(task_ld)
a = kpu.deinit(task_fd)

