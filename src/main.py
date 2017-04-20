import utime
import machine
import esp
import math


@micropython.asm_xtensa
def a(a2, a3):
    mov(a4, a2)
    movi(a2, 0)
    movi(a5, 1)
    label(LOOP)
    add(a2, a2, a4)
    sub(a3, a3, a5)
    bnez(a3, LOOP)

if __name__ == '__main__':
    i = 0
    offset = 0
    on = False
    led = machine.Pin(2, machine.Pin.OUT)
    gpio4 = machine.Pin(4, machine.Pin.OUT)

    machine.freq(160000000)

    rgb = bytearray(240*3)
    oldTime = utime.ticks_ms()

    mode = 4
    while True:
        # mode 0: 142 fps (do nothing)
        # Theoretical max of 240 LED strip is 145 fps eg: 1 / (1.2us * 8 * 3 * 240)

        # mode 1: 125 fps (set six bytes, two pixels)
        if mode == 1:
            rgb[offset*3 + 0] = 0
            rgb[offset*3 + 1] = 0
            rgb[offset*3 + 2] = 0

            offset = offset + 1
            offset = offset % 239

            rgb[offset*3 + 0] = 0
            rgb[offset*3 + 1] = 255
            rgb[offset*3 + 2] = 255

        # mode 2: 27 fps (set all 720 bytes, 240 pixels - using range)
        elif mode == 2:
            for j in range(0, 240*3, 3):
                rgb[j + 0] = 0
                rgb[j + 1] = offset
                rgb[j + 2] = offset

            offset = offset + 1
            offset = offset % 24

        # mode 3: 55 fps (same as mode 2 - using operator +=)
        elif mode == 3:
            j = 0
            while j < 240*3:
                rgb[j + 0] = 0
                rgb[j + 1] = offset
                rgb[j + 2] = offset
                j += 3

            offset += 1
            offset %= 24

        # mode 4: 22 fps (rainbow'ish)
        elif mode == 4:
            j = 0
            while j < 240*3:

                rgb[j + 0] = int((1 + math.sin(offset/10)) * 120)
                rgb[j + 1] = int((1 + math.sin(offset/9)) * 120)
                rgb[j + 2] = int((1 + math.sin(offset/8)) * 120)
                j += 3

            offset += 1
            offset %= 0xFFFF

        esp.neopixel_write(gpio4, rgb, True)

        on = ~on
        if on:
            led.low()
        else:
            led.high()

        newTime = utime.ticks_ms()
        print("FPS:", int(1000.0 / utime.ticks_diff(newTime, oldTime)))
        oldTime = newTime
