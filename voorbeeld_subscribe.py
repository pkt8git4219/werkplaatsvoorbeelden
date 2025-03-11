import network
import time
from machine import Pin
from umqtt.simple import MQTTClient

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect("xxxxxxx1", "xxxxxxxxxx")
time.sleep(5)
print(wlan.isconnected())

LED = Pin(15, Pin.OUT)

mqtt_server = b'xxxxxxxxxxx.s2.eu.hivemq.cloud'
client_id = b'xxxxxxxxxxx'
topic_sub = 'test'


def sub_cb(topic, msg):
    print("New message on topic {}".format(topic.decode('utf-8')))
    msg = msg.decode('utf-8')
    print(msg)
    if msg == "on":
        LED.on()
    elif msg == "off":
        LED.off()


def connectMQTT():
    client = MQTTClient(client_id=b"kuijp2023",

                        server=b"xxxxxxxxxxxxxx.s2.eu.hivemq.cloud",
                        port=8883,
                        user=b"xxxxxxxxx",
                        password=b"xxxxxxxxxxx",
                        keepalive=7200,
                        ssl=True,
                        ssl_params={'server_hostname': 'xxxxxxxxxxxx.s2.eu.hivemq.cloud'}
                        )
    client.set_callback(sub_cb)
    client.connect()
    print('Connected to %s MQTT Broker' % (mqtt_server))
    return client


def reconnect():
    print('Failed to connect to MQTT Broker. Reconnecting...')
    time.sleep(5)
    machine.reset()


try:
    client = connectMQTT()
except OSError as e:
    reconnect()
while True:
    client.subscribe(topic_sub)
    time.sleep(1)