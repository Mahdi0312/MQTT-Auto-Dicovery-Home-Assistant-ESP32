#include <WiFi.h>
#include <ArduinoHA.h> 

// --- CONFIGURATION ---
const char* ssid = "enter-your-wifi-ssid";
const char* password = "enter-your-wifi-password";

// Nom mDNS de Home Assistant (au lieu d'une IP fixe)
const char* mqtt_server = "homeassistant.local";
const uint16_t mqtt_port = 1883;

const char* mqtt_user = "esp_test_01_user";
const char* mqtt_pass = "esp_test_01_pass";

// --- OBJETS ---
WiFiClient client;
HADevice device("esp_test_01");
HAMqtt mqtt(client, device);

// --- ENTITÉS HOME ASSISTANT ---
HASensor uptimeSensor("esp32_uptime");       
HASwitch ledSwitch("esp32_led_interne");     
HASensor statusMsg("esp32_status");

// --- CALLBACK : Commande depuis Home Assistant ---
void onSwitchCommand(bool state, HASwitch* sender) {
    Serial.print("COMMANDE REÇUE -> LED : ");
    Serial.println(state ? "ALLUMÉE" : "ÉTEINTE");

    digitalWrite(2, state ? HIGH : LOW);
    sender->setState(state);

    statusMsg.setValue(state ? "Allumée" : "Éteinte");
}

void setup() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);

    // --- INFOS APPAREIL ---
    device.setName("esp_test");
    device.setSoftwareVersion("1.0.0");
    device.setManufacturer("esp_test");

    // --- CONFIG DES ENTITÉS ---
    uptimeSensor.setName("Temps de fonctionnement");
    uptimeSensor.setUnitOfMeasurement("sec");

    ledSwitch.setName("Lampe Bureau");
    ledSwitch.onCommand(onSwitchCommand);

    statusMsg.setName("Statut de la LED");

    // --- CONNEXION WIFI ---
    Serial.print("Connexion WiFi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connecté !");
    Serial.print("IP ESP32 : ");
    Serial.println(WiFi.localIP());

    // --- CONNEXION MQTT (Home Assistant via mDNS) ---
    Serial.println("Connexion au broker MQTT (homeassistant.local)...");
    mqtt.begin(mqtt_server, mqtt_port, mqtt_user, mqtt_pass);
}

void loop() {
    mqtt.loop();

    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 5000) {
        lastUpdate = millis();

        if (mqtt.isConnected()) {
            Serial.println("MQTT CONNECTÉ !");
            unsigned long sec = millis() / 1000;
            uptimeSensor.setValue(String(sec).c_str());
        } else {
            Serial.println("MQTT DÉCONNECTÉ... reconnexion en cours");
        }
    }
}
