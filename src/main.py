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

    while True:
        i = i + 1

        rgb[offset*3 + 0] = 0
        rgb[offset*3 + 1] = 0
        rgb[offset*3 + 2] = 0

        offset = offset + 1
        offset = offset % 239

        rgb[offset*3 + 0] = 0
        rgb[offset*3 + 1] = 255
        rgb[offset*3 + 2] = 255

        esp.neopixel_write(gpio4, rgb, True)

        if on:
            led.low()
            on = False
        else:
            led.high()
            on = True

        newTime = utime.ticks_ms()
        print(1000.0 / utime.ticks_diff(newTime, oldTime))
        oldTime = newTime
