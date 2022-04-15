import hid
import sys
import subprocess
from pystray import Icon as icon, Menu as menu, MenuItem as item
from pynput import keyboard
from PIL import Image, ImageDraw
from pathlib import Path

state = False

def on_clicked(icon, item):
    global state
    state = not item.checked

def create_image():
    width = 10
    height = 10
    color1 = (255,255,255)
    color2 = (255,255,255)
    # Generate an image and draw a pattern
    image = Image.open(Path("~/Downloads/isla_fullxfull.41365147_1vxfnuph-removebg-preview.png").expanduser())

    return image
# Update the state in `on_clicked` and return the new state in
# a `checked` callable
icon('test', create_image(), menu=menu()).run_detached()
if __name__ == "__main__":

    vendor_id = 0xFEED  # Found in config.h for the keyboard's main directory
    product_id = 0x1701
    usage_page = 0xFF60  # The defaults, can be redefined in the same config.h
    usage = 0x61

    path = None    # Found with the code below

    for d in hid.enumerate(vendor_id, product_id):
        if d["usage_page"] == usage_page and d["usage"] == usage:
            path = d["path"]

    if path is None:
        print("Couldn't connect")
        sys.exit(0)

    # Have to specify the path, because there are several "usage pages" and
    # if we choose wrong one we get access denied error
    def hid_send(bytes_str):
        with hid.Device(vendor_id, product_id, path = path) as device:
            device.write(b"\x00" + bytes_str)

    vol_up_keys = 0
    vol_down_keys = 0

    def on_press(key):
        global vol_up_keys
        global vol_down_keys
        try:
            modifiers = key == keyboard.Key.ctrl_l or key == keyboard.Key.alt_l or key == keyboard.Key.cmd
            vol_up_keys = min(1, vol_up_keys + (key == keyboard.Key.f13))
            vol_down_keys = min(1, vol_down_keys + (key == keyboard.Key.f14))
            print(vol_up_keys, vol_down_keys)
            if vol_up_keys == 1:
                print("VOL UP")
                subprocess.run("./soundvolumeview-x64/SoundVolumeView.exe /ChangeVolume DefaultRenderDevice 2")
            if vol_down_keys == 1:
                print("VOL DOWN")
                subprocess.run("./soundvolumeview-x64/SoundVolumeView.exe /ChangeVolume DefaultRenderDevice -2")
            if vol_up_keys == 1 or vol_down_keys == 1:
                process = subprocess.run("./soundvolumeview-x64/SoundVolumeView.exe /GetPercent DefaultRenderDevice & echo %errorlevel%")
                hid_send(bytes([int(process.returncode / 10)]))
        except AttributeError:
            print('special key {0} pressed'.format(
                key))

    def on_release(key):
        global vol_up_keys
        global vol_down_keys
        modifiers = key == keyboard.Key.ctrl_l or key == keyboard.Key.alt_l or key == keyboard.Key.cmd
        vol_up_keys -= modifiers or key == keyboard.Key.f13
        vol_down_keys -= modifiers or key == keyboard.Key.f14
        print(vol_up_keys, vol_down_keys)
        if key == keyboard.Key.esc:
            # Stop listener
            return False

    # Collect events until released
    with keyboard.Listener(
            on_press=on_press,
            on_release=on_release) as listener:
        listener.join()
