#include <WiFi.h>
#include <ArduinoHA.h> 
#include <ESPmDNS.h> // Indispensable pour homeassistant.local

#define DEVICE_NAME           "DEVICE_NAME"
#define Manufacturer_TYPE     "Manufacturer_TYPE"
#define MODEL_TYPE            "MODEL_TYPE"
#define FIRMWARE_VERSION_HA   "V. 1.0.0"

// --- CONFIGURATION ---
const char* ssid        = "Enter_The_SSID";
const char* password    = "Enter_The_Password";

// Paramètres MQTT
IPAddress serverIp;
const char* mqtt_user = "esp_test_01_user";
const char* mqtt_pass = "esp_test_01_pass";
const uint16_t mqtt_port = 1883;

// --- OBJETS ---
WiFiClient client;
HADevice device(DEVICE_NAME);
HAMqtt mqtt(client, device); // L'objet 'mqtt' est déclaré ici globalement

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

// ================= TROUVER IP HA =================
void findHA_IP() {
    // --- RÉSOLUTION mDNS ---
    if (!MDNS.begin("esp32-test")) {
        Serial.println("Erreur mDNS");
    }

    Serial.println("Recherche de homeassistant.local...");
    int attempts = 0;
    
    // On essaye de trouver l'IP pendant 10 secondes
    while (serverIp.toString() == "0.0.0.0" && attempts < 10) {
        serverIp = MDNS.queryHost("homeassistant");
        if (serverIp.toString() == "0.0.0.0") {
            delay(1000);
            attempts++;
            Serial.print(".");
        }
    }

    if (serverIp.toString() != "0.0.0.0") {
        Serial.print("\nIP trouvée : ");
        Serial.println(serverIp);
        // On utilise l'IP trouvée pour démarrer le MQTT
        mqtt.begin(serverIp, mqtt_port, mqtt_user, mqtt_pass);
    } else {
        Serial.println("\nImpossible de trouver homeassistant.local. Vérifiez votre réseau.");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);

    // --- INFOS APPAREIL ---
    device.setName(DEVICE_NAME);
    device.setManufacturer(Manufacturer_TYPE);
    device.setSoftwareVersion(FIRMWARE_VERSION_HA);
    device.setModel(MODEL_TYPE);

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
    findHA_IP();
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

