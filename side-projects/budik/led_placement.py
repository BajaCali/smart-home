#!/usr/bin/env python

from math import cos, sin, radians, pi
import pcbnew

def fromMm(mm):
    return 1000000 * mm

center = (fromMm(50), fromMm(50))
radius_led = fromMm(25)
count = 12
step = 360 / (count) / 180.0 * pi

filename = "budik.kicad_pcb"

b = pcbnew.LoadBoard(filename)

m = b.GetModules()
while True:
    if not m:
        break
    ref = m.GetReference()
    if ref.startswith("LED"):
        num = int(ref[3:]) - 1

        angle = num * step
        m.SetOrientation(-(angle - pi / 2) * 1800 / pi)
        c = m.GetPosition()
        c.Set(int(center[0] + radius_led * cos(angle)), int(center[1] + radius_led * sin(angle)))
        m.SetPosition(c)

        print("Diode: " + str(num))
    m = m.Next()

b.Save(filename)

print("Done")
