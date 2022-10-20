from pyparticleio.ParticleCloud import ParticleCloud
from random import randint
import time

access_token = "712f83bdbd1c85b212f4adc2d4f4a7bebab74d51"

if __name__ == '__main__':
    c = ParticleCloud(username_or_access_token=access_token)
    # devices = c.devices

    while True:
        led_num = randint(1,11)
        if randint(1,100) < 50:
            print(led_num, "On")
            c.robot_raptor.ledOn(f"led{led_num}")
        else:
            print(led_num, "Off")
            c.robot_raptor.ledOff(f"led{led_num}")
        time.sleep(1)


