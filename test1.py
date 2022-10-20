from pyparticleio.ParticleCloud import ParticleCloud

access_token = "712f83bdbd1c85b212f4adc2d4f4a7bebab74d51"

if __name__ == '__main__':
    c = ParticleCloud(username_or_access_token=access_token)
    devices = c.devices
    print(devices)

    print(devices['bacon_captain'].temp)
    print(c.bacon_captain.temp)
    print(c.bacon_captain.humidity)
    print(c.bacon_captain.light)
    print(c.bacon_captain.functions)
    print(c.bacon_captain.variables)


