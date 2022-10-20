from pyparticleio.ParticleCloud import ParticleCloud

access_token = "712f83bdbd1c85b212f4adc2d4f4a7bebab74d51"

if __name__ == '__main__':
    c = ParticleCloud(username_or_access_token=access_token)
    devices = c.devices_list
    print(devices)

    for device in devices:
        print(device.name)
        print(device.attribute_names())

    device_name = "bacon_captain"
    print("-"*10)
    print(c.devices[device_name])
