
import sensor, image, lcd, time
import KPU as kpu
from fpioa_manager import fm
from machine import UART
from board import board_info
from fpioa_manager import fm


lcd.init()
lcd.clear()

#摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)

#串口
fm.register(board_info.PIN10,fm.fpioa.UART2_TX)# 管脚映射
fm.register(board_info.PIN11,fm.fpioa.UART2_RX)

uart2 = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len=4096)


f=open('/sd/labels.txt','r')
labels=f.readlines()
f.close()

task = kpu.load('/sd/6bf1b010d89e01eda73800c4d00f2c1d_95a4cfaa505779a2118e70350fdc5ee8.smodel')
read_str=0
lcd.clear(0xFFFF)
clock = time.clock()

while(True):

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
    read_data = uart2.read(2)   #读缓冲区

    if(read_data):
        read_str = read_data.decode('utf-8')
        read_data=0
        if(read_str=='1'):
             Mask_state=1
             lcd.draw_string(10, 30, "Mask:%d"%(Mask_state))
        else:
             Mask_state=0
             lcd.draw_string(10, 30,"Mask:%d"%(Mask_state))


a = kpu.deinit(task)
