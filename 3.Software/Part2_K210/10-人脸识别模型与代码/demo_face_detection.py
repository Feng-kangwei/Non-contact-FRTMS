

# 可以实现人脸识别+串口输出识别结果的功能

import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
from machine import UART   # 引入串口库
import utime


#加载三组模型
task_fd = kpu.load(0x300000) #人脸检测模型
task_ld = kpu.load(0x400000) #人脸关键点检测模型
task_fe = kpu.load(0x500000) #人脸特征值检测模型

clock = time.clock()

fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)# 对于系统管脚重映射 适配板子
#串口重映射
fm.register(board_info.PIN10,fm.fpioa.UART2_TX,force=True)# 管脚映射  Pin10——UART2-TX
fm.register(board_info.PIN11,fm.fpioa.UART2_RX,force=True)#          Pin11——UART2-RX
uart2_ReceivePart= UART(UART.UART2, 115200, 8, 0, 0, timeout=1000, read_buf_len=4096)



key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)  #使能按键
start_processing = False
BOUNCE_PROTECTION = 50


def set_key_state(*_):
    global start_processing
    start_processing = True
    utime.sleep_ms(BOUNCE_PROTECTION)


key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)

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
             (81, 105)]  # standard face key point position  #标准人脸关键点坐标
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
img_face = image.Image(size=(128, 128))
a = img_face.pix_to_ai()
record_ftr = []
record_ftrs = []
names = ['FKW', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5',
         'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9', 'Mr.10']

ACCURACY = 85
read_TempDataBUFF=0
TempData='35.6'


while (1):
    img = sensor.snapshot()
    clock.tick()
    code = kpu.run_yolo2(task_fd, img)

    #接收串口温度  (当按键按下后 K210发送1 STM32读取到1后 向K210发送温度数据)
    read_TempDataBUFF=uart2_ReceivePart.read(4)  #读取STM32端的温度数据 一次只读四个字节
    if read_TempDataBUFF:# 加入读到了东西
        TempData = read_TempDataBUFF.decode('utf-8')#将Bytes类型通过UTF-8的编码格式给TempData
        print(read_TempDataBUFF)  #打印BUFF中的数据
        print(TempData)           #答应经过UTF-8编码后的数据
        #BUG：不经过 print(read_TempDataBUFF)语句就会产生Unicode错误（解决：一次只读4个字节）


    if code:
        for i in code:
            # Cut face and resize to 128x128   # 重新对于人脸图片进行标准化
            a = img.draw_rectangle(i.rect()) # 对检测到的人脸进行框选
            face_cut = img.cut(i.x(), i.y(), i.w(), i.h()) #剪切人脸部分
            face_cut_128 = face_cut.resize(128, 128)
            a = face_cut_128.pix_to_ai()
            # a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points   #标记人脸关键点位置
            fmap = kpu.forward(task_ld, face_cut_128)  #调用0x400000中的模型进行前向传播，使用的是框选的人脸
            plist = fmap[:]
            le = (i.x() + int(plist[0] * i.w() - 10), i.y() + int(plist[1] * i.h())) #左眼坐标
            re = (i.x() + int(plist[2] * i.w()), i.y() + int(plist[3] * i.h()))      #右眼坐标
            nose = (i.x() + int(plist[4] * i.w()), i.y() + int(plist[5] * i.h()))    #鼻子坐标
            lm = (i.x() + int(plist[6] * i.w()), i.y() + int(plist[7] * i.h()))      #左嘴角坐标
            rm = (i.x() + int(plist[8] * i.w()), i.y() + int(plist[9] * i.h()))      #右嘴角坐标
            a = img.draw_circle(le[0], le[1], 4) #画圆圈
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4)
            # align face to standard position   #将人脸校正到标准位置
            src_point = [le, re, nose, lm, rm]
            T = image.get_affine_transform(src_point, dst_point)
            a = image.warp_affine_ai(img, img_face, T)
            a = img_face.ai_to_pix()
            # a = img.draw_image(img_face, (128,0))
            del (face_cut_128)
            # calculate face feature vector    # 计算人脸的特征向量
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

                a = img.draw_string(i.x(), i.y(), ("%s :%s"  % (
                    names[index], TempData)), color=(0, 255, 0), scale=2) #显示人脸名称：温度
                #print("%s"%(names[index])) #将人脸检测到的信息上传至串口位置

            else:
                a = img.draw_string(i.x(), i.y(), ("X :%s" % (
                    TempData)), color=(255, 0, 0), scale=2)
            if start_processing:   # 这就是按键按下后给STM32端发送请求 使得STM32端发送温度数据
                record_ftr = feature
                record_ftrs.append(record_ftr)
                start_processing = False
                uart2_ReceivePart.write('1')
            break
    fps = clock.fps()
    #print("%2.1f fps" % fps)
    a = lcd.display(img)
    gc.collect()
    # kpu.memtest()

# a = kpu.deinit(task_fe)
# a = kpu.deinit(task_ld)
# a = kpu.deinit(task_fd)
