# Hello World Example
#
# Welcome to the MaixPy IDE!
# 1. Conenct board to computer
# 2. Select board at the top of MaixPy IDE: `tools->Select Board`
# 3. Click the connect buttion below to connect board
# 4. Click on the green run arrow button below to run the script!

import sensor, image, time, lcd
from fpioa_manager import *
from Maix import GPIO
import utime
import KPU as kpu

#Grove connector setting
fm.register(34, fm.fpioa.GPIO1)
fm.register(35, fm.fpioa.GPIO0)
play = GPIO(GPIO.GPIO1, GPIO.OUT)
rec = GPIO(GPIO.GPIO0, GPIO.OUT)

#LED
fm.register(board_info.LED_W, fm.fpioa.GPIO3)
fm.register(board_info.LED_R, fm.fpioa.GPIO4)
fm.register(board_info.LED_G, fm.fpioa.GPIO5)
fm.register(board_info.LED_B, fm.fpioa.GPIO6)

led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_r = GPIO(GPIO.GPIO4, GPIO.OUT)
led_g = GPIO(GPIO.GPIO5, GPIO.OUT)
led_b = GPIO(GPIO.GPIO6, GPIO.OUT)

#initial all off
led_w.value(1)
led_r.value(1)
led_g.value(1)
led_b.value(1)

lcd.init(freq=15000000)
sensor.reset()                      # Reset and initialize the sensor. It will
                                    # run automatically, call sensor.run(0) to stop
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.
frame_num = 0

#set value to 0 to play sound
def play_sound():
    play.value(1)
    utime.sleep(1.0)
    play.value(0)
    utime.sleep(10.0)

#set value to 1 to recoding and keep 10s.
#set valuet to 0 to stop recording
def rec_sound():
    led_g.value(0)
    rec.value(1)
    utime.sleep(10.0)
    led_b.value(0)
    led_g.value(1)
    rec.value(0)
    utime.sleep(10.0)
    led_b.value(1)

#yolo
model_addr=0x300000
anchors = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025)

task = None
task = kpu.load(model_addr)
kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]

sensor_vflip = 0
sensor_hmirror=0

while(True):
    clock.tick()                    # Update the FPS clock.
    sensor.set_vflip(sensor_vflip)
    sensor.set_hmirror(sensor_hmirror)
    img = sensor.snapshot()         # Take a picture and return the image.
#    lcd.display(img)                # Display on LCD
    print(clock.fps())              # Note: MaixPy's Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
#    lcd.draw_string(20,50,str(frame_num))

    t = time.ticks_ms()
    objects = kpu.run_yolo2(task, img)
    count = 0
    t = time.ticks_ms() - t
    if objects:
        for obj in objects:
            img.draw_rectangle(obj.rect())
            count=count+1
#    img.draw_string(40, 120, "c:%d" %(count), scale=2)
#    img.draw_string(40, 100, "t:%dms" %(t), scale=2)
    lcd.display(img)

    if count > 0:
        led_r.value(0)
        img.draw_string(40, 100, "check message",scale=2)
        lcd.display(img)
        play_sound()
        led_r.value(1)
        img.draw_string(40, 120, "leave message",scale=2)
        lcd.display(img)
        rec_sound()

    frame_num = frame_num + 1
#    if frame_num >= 100:
#        break;
