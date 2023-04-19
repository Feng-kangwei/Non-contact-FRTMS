
import sensor, image, time,pyb

RED_LED_PIN=1
BLUE_LED_PIN=3

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.B128X128)
sensor.set_windowing((92,112))
sensor.skip_frames(10)
sensor.skip_frames(time = 2000)

num=

n=20

while(n):
    pyb.LED(RED_LED_PIN).on()
    sensor.skip_frames(time=3000)   #给三秒等待

    # 红灯亮，蓝灯灭
    pyb.LED(RED_LED_PIN).off()
    pyb.LED(BLUE_LED_PIN).on()

    #保存截取到的图片到SD卡
    print(n)
    sensor.snapshot().save("TEST1/s%s/%s.pgm"%(num,n))

    n-=1

    pyb.LED(BLUE_LED_PIN).off()
    print("Done! Reset it")



