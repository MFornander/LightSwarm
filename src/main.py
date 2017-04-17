import time, machine, neopixel, esp


def old():
    led = machine.Pin(2, machine.Pin.OUT)
    np = neopixel.NeoPixel(machine.Pin(4), 240)
    on = False
    offset = 0

    while True:
        for i in range(0, np.n):
            np[i] = ((i + offset) % 256, 0, 0)

        np.write()
        if on:
            led.low()
            on = False
        else:
            led.high()
            on = True

        offset = offset +1

        time.sleep_ms(20)


if __name__ == '__main__':
    i = 0
    offset = 0
    on = False
    led = machine.Pin(2, machine.Pin.OUT)
    led4 = machine.Pin(4, machine.Pin.OUT)

    machine.freq(160000000)

    rgb = bytearray(240*3)

    while True:
        print(i)
        i = i + 1

        rgb[offset*3 + 0] = 0
        rgb[offset*3 + 1] = 0
        rgb[offset*3 + 2] = 0

        offset = offset +1
        offset = offset % 239

        rgb[offset*3 + 0] = 0
        rgb[offset*3 + 1] = 255
        rgb[offset*3 + 2] = 255

        esp.neopixel_write(machine.Pin(4), rgb, True)

        if on:
            led.low()
            on = False
        else:
            led.high()
            on = True

