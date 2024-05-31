#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Configuración de la LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C puede variar, verificar la correcta

// Configuración del Servo
Servo servo;
int servoPin = 9;

// Configuración de los sensores ultrasónicos
const int trigEntrada = 2;
const int echoEntrada = 3;
const int trigSalida = 4;
const int echoSalida = 5;

// Variables de estado
int espaciosTotales = 7;  // Ajustar según la cantidad de espacios de parqueo
int espaciosDisponibles;

long distanciaEntrada, distanciaSalida;
long duracionEntrada, duracionSalida;

void setup() {
  // Inicializar la LCD
  lcd.init();
  lcd.backlight();

  // Inicializar el servo
  servo.attach(servoPin);
  servo.write(0);  // Posición inicial del servo

  // Inicializar los pines de los sensores ultrasónicos
  pinMode(trigEntrada, OUTPUT);
  pinMode(echoEntrada, INPUT);
  pinMode(trigSalida, OUTPUT);
  pinMode(echoSalida, INPUT);

  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar los espacios disponibles inicialmente
  espaciosDisponibles = espaciosTotales;

  // Mostrar los espacios disponibles en la LCD
  actualizarLCD();
}

void loop() {
  // Leer la distancia del sensor de entrada
  distanciaEntrada = medirDistancia(trigEntrada, echoEntrada);
  distanciaSalida = medirDistancia(trigSalida, echoSalida);

  // Verificar el sensor de entrada
  if (distanciaEntrada < 10) {  // Ajustar el umbral según sea necesario
    Serial.println("Auto detectado en la entrada");
    if (espaciosDisponibles > 0) {
      // Disminuir los espacios disponibles
      espaciosDisponibles--;
      // Levantar la plumilla
      servo.write(90);  // Ajustar ángulo según sea necesario
      delay(2000);  // Esperar 2 segundos para que pase el auto
      servo.write(0);  // Bajar la plumilla
      // Actualizar la LCD
      actualizarLCD();
    }
  }

  // Verificar el sensor de salida
  if (distanciaSalida < 10) {  // Ajustar el umbral según sea necesario
    Serial.println("Auto detectado en la salida");
    if (espaciosDisponibles < espaciosTotales) {
      // Aumentar los espacios disponibles
      espaciosDisponibles++;
      // Levantar la plumilla
      servo.write(90);  // Ajustar ángulo según sea necesario
      delay(2000);  // Esperar 2 segundos para que pase el auto
      servo.write(0);  // Bajar la plumilla
      // Actualizar la LCD
      actualizarLCD();
    }
  }

  delay(500);  // Pequeño retraso para evitar lecturas erróneas
}

long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;  // Convertir tiempo a distancia
  
  return distance;
}

void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Espacios Disp:");
  lcd.setCursor(0, 1);
  lcd.print(espaciosDisponibles);
}
