#include <WiFi.h>
#include <LilyGoLib.h>
#include <LV_Helper.h>

#define MAX 80
#define PORT 5555
#define SERVER_IP "192.168.0.40"

const char* ssid = "Mejdi@Telkos";
const char* password = "00000000";

WiFiClient client;

bool isPmuIRQ = false;
uint8_t rotation = 2;

lv_obj_t *label;

void setup(void) {
    Serial.begin(115200);

    // Initialize LVGL and create label
    lv_init();
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "not connected");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // Initialize watch
    watch.begin();
    beginLvglHelper();

    // Start WiFi connection
    WiFi.begin(ssid, password);

    // Wait for WiFi connection
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");

    // Try to connect to the server
    if (!client.connect(SERVER_IP, PORT)) {
        lv_label_set_text(label, "couldn't connect");
        Serial.println("Couldn't connect to server.");
    } else {
        lv_label_set_text(label, "connected");
        Serial.println("Connected to server.");
    }

    // Attach PMU interrupt
    watch.attachPMU([]() {
        isPmuIRQ = true;
    });
}

void loop() {
    lv_task_handler();
    delay(5);
}
