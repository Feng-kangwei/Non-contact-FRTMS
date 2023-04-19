
import sensor,image,lcd,time
import KPU as kpu

lcd.init(freq=15000000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)

clock = time.clock()

classes = ['FKW','WXY','ZZL']

task = kpu.load('/sd/6bf1b010d89e01eda73800c4d00f2c1d_95a4cfaa505779a2118e70350fdc5ee8.smodel')
anchor = (1.08, 1.19, 3.42, 4.41, 6.63, 11.38, 9.42, 5.11, 16.62, 10.52)
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
while(True):
    clock.tick()
    img = sensor.snapshot()
    code = kpu.run_yolo2(task, img)
    print(clock.fps())
    if code:
        for i in code:
            a=img.draw_rectangle(i.rect())
            a = lcd.display(img)
            for i in code:
                lcd.draw_string(i.x(), i.y(), classes[i.classid()], lcd.RED, lcd.WHITE)
                lcd.draw_string(i.x(), i.y()+12, '%.3f'%i.value(), lcd.RED, lcd.WHITE)
    else:
        a = lcd.display(img)
a = kpu.deinit(task)

