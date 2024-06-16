#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pin_a D0 
#define pin_b D1
#define pin_c D2 
#define pin_d D3 
#define pin_e D4 
#define TOPIC_PUBLISH "RECEBE_DADOS_ESP" // Tópico 

char buf[30];
int val_a, val_b, val_c, val_d, val_e; // Declarando as variáveis val no escopo global.
int lastAState, lastBState, lastCState, lastDState, lastEState; // Variável para armazenar o estado anterior de cada pino.

const char *ssid = "Android";  // ssid
const char *password = "1234567890";  // Substitua pela sua senha
const char *mqtt_broker = "mqtt.eclipseprojects.io"; 
const int mqtt_port = 1883;  // Substitua pela porta correta do broker MQTT
const char *topic = "esp8266/inputs";

WiFiClient espClient;
PubSubClient client(espClient);

// Declaração da função getLeitura
void getLeitura();

void setup() {
    Serial.begin(9600);
    pinMode(pin_a, INPUT);
    pinMode(pin_b, INPUT);
    pinMode(pin_c, INPUT);
    pinMode(pin_d, INPUT);
    pinMode(pin_e, INPUT);

    // Conecta-se à rede Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado à rede Wi-Fi");

    // Configura o servidor MQTT
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    // Conecta-se ao servidor MQTT
    reconnect();
}

void loop() {
    // Mantém a conexão MQTT ativa
    if (!client.connected()) {
        reconnect();
    }

    // Realiza a leitura de cada pino
    val_a = digitalRead(pin_a);
    val_b = digitalRead(pin_b);
    val_c = digitalRead(pin_c);
    val_d = !digitalRead(pin_d);
    val_e = !digitalRead(pin_e);

    // Verifica mudanças em cada pino e publica no broker se necessário
    if (val_a != lastAState) {
        getLeitura('a', val_a);
        lastAState = val_a;
    }

    if (val_b != lastBState) {
        getLeitura('b', val_b);
        lastBState = val_b;
    }

    if (val_c != lastCState) {
        getLeitura('c', val_c);
        lastCState = val_c;
    }

    if (val_d != lastDState) {
        getLeitura('d', val_d);
        lastDState = val_d;
    }

    if (val_e != lastEState) {
        getLeitura('e', val_e);
        lastEState = val_e;
    }

    // Aguarda antes de enviar novas leituras
    delay(1000);
  
    // Mantém a conexão MQTT ativa
    client.loop();
}

// Função que realiza a leitura do sensor e enviar para o tópico Publisher
void getLeitura(char pino, int valor) {
    dtostrf(valor, 6, 2, buf);
    String topic_pino = "esp8266/inputs/" + String(pino);
    client.publish(topic_pino.c_str(), buf);
    Serial.print("Pino ");
    Serial.print(pino);
    Serial.print(": ");
    Serial.println(buf);
    Serial.println("Payload enviado!");
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Mensagem no tópico: ");
    Serial.println(topic);
    Serial.print("Conteúdo da mensagem: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void reconnect() {
    while (!client.connected()) {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("Conectando o cliente %s ao broker MQTT\n", client_id.c_str());
        if (client.connect(client_id.c_str())) {
            Serial.println("Conectado ao broker MQTT!");
            client.subscribe(topic);
        } else {
            Serial.print("Falha na conexão: ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}
