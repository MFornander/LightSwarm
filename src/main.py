import utime
import machine
import esp


if __name__ == '__main__':
    i = 0
    offset = 0
    on = False
    led = machine.Pin(2, machine.Pin.OUT)
    gpio4 = machine.Pin(4, machine.Pin.OUT)

    machine.freq(160000000)

    rgb = bytearray(240*3)
    oldTime = utime.ticks_ms()
    mode = 3

    while True:
        # mode 0: 142 fps (do nothing)

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
            while j < 240:
                rgb[j + 0] = 0
                rgb[j + 1] = offset % 24
                rgb[j + 2] = offset % 24
                j += 3

            offset = offset + 1
            offset = offset % 24

        esp.neopixel_write(gpio4, rgb, True)

        if on:
            led.low()
            on = False
        else:
            led.high()
            on = True

        newTime = utime.ticks_ms()
        print("FPS:", int(1000.0 / utime.ticks_diff(newTime, oldTime)))
        oldTime = newTime
