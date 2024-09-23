#include <WiFi.h>
#include <WebServer.h>

// Configuración de tu red Wi-Fi
const char* ssid = "xxx4";
const char* password = "xxx";

// Configuración del servidor web en el puerto 80
WebServer server(80);

// Pin del LED
const int ledPinR = 2;
const int ledPinG = 4;
const int ledPinB = 12;
// Configuración del PWM
const int pwmChannel = 0;
const int pwmFrequency = 5000;
const int pwmResolution = 8;

int ledes[3] = {0, 1, 2};
String rgb[3] = {"sliderR", "sliderG","sliderB"};

void setup() {
  ledcSetup(ledes[0], pwmFrequency, pwmResolution);
  ledcAttachPin(ledPinR, ledes[0]);
  
  ledcSetup(ledes[1], pwmFrequency, pwmResolution);
  ledcAttachPin(ledPinG, ledes[1]);

  ledcSetup(ledes[2], pwmFrequency, pwmResolution);
  ledcAttachPin(ledPinB, ledes[2]);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado a Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setBrightness", handleSetBrightness);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
 
  server.send(200, "text/html", "sape");
}

void handleSetBrightness() {
  for(int i= 0;i<3;i++){
  if (server.hasArg(rgb[i])) {
    int nivel = server.arg(rgb[i]).toInt();
    if (nivel >= 0 && nivel <= 17) {
      int brillo = map(nivel,17,0,0,255);
      ledcWrite(ledes[i], brillo);
      Serial.print(rgb[i] + " = ");
      Serial.println(brillo);

      server.sendHeader("Access-Control-Allow-Origin", "*");//evita problema de cors
      server.send(200, "text/plain", "Nivel ajustado a: " + String(nivel));
    }
  }
}
}

