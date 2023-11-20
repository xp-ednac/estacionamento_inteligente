import time
from paho.mqtt import client as mqtt_client

# Configurações de conexão ao Broker
broker = 'mqtt.eclipseprojects.io'
port = 1883
topic_subscribe = "esp8266/inputs/#"  # Subscreva a todos os tópicos sob 'esp8266/inputs/'
client_id = 'BROKER_PC_SUB'

# Realiza conexão ao Broker
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Conectado ao Broker MQTT")
        else:
            print("Falha ao conectar, código de retorno %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

# Realiza subscribe ao Tópico
def subscribe(client):
    def on_message(client, userdata, msg):
        print(f"Recebido o dado `{msg.payload.decode()}` do tópico `{msg.topic}`")

    client.subscribe(topic_subscribe)
    client.on_message = on_message

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    run()
