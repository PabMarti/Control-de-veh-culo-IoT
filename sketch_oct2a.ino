const int motorPins[] = {16, 17, 18, 19};  // Pines de los motores M1, M2, M3, M4
const int speedPins[] = {27, 32};          // Pines de habilitación de velocidad ENA, ENB
const int echoPin = 5;
const int trigPin = 4;

// Definición de movimientos: {Motor1, Motor2, Motor3, Motor4, VelocidadENA, VelocidadENB}
const int actions[8][6] = {
  {HIGH, LOW, LOW, HIGH, 255, 255},  // Avanzar
  {LOW, LOW, HIGH, HIGH, 0, 0},      // Detener
  {LOW, HIGH, HIGH, LOW, 255, 255},  // Retroceder
  {LOW, LOW, LOW, HIGH, 0, 250},     // Girar Izquierda
  {HIGH, LOW, HIGH, HIGH, 255, 0},   // Girar Derecha
  {HIGH, LOW, LOW, HIGH, 100, 255},  // Curva Izquierda
  {HIGH, LOW, LOW, HIGH, 255, 100},  // Curva Derecha
  {LOW, LOW, LOW, LOW, 0, 0}         // Apagar motores
};

void setup() {
  // Configurar pines de salida para los motores
  configurarPines(motorPins, 4, OUTPUT);
  configurarPines(speedPins, 2, OUTPUT);

  // Inicializar todos los pines a estado bajo
  inicializarPines(motorPins, 4);
  inicializarPines(speedPins, 2);

  // Configurar el sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Iniciar comunicación serial para monitoreo
  Serial.begin(9600);
}

void configurarPines(const int pines[], int cantidad, int modo) {
  for (int i = 0; i < cantidad; i++) {
    pinMode(pines[i], modo);
  }
}

void inicializarPines(const int pines[], int cantidad) {
  for (int i = 0; i < cantidad; i++) {
    digitalWrite(pines[i], LOW);
  }
}

void mover(int actionIndex, int duracion) {
  // Ejecutar el movimiento correspondiente
  for (int i = 0; i < 4; i++) {
    digitalWrite(motorPins[i], actions[actionIndex][i]);
  }

  // Ajustar la velocidad
  for (int i = 0; i < 2; i++) {
    analogWrite(speedPins[i], actions[actionIndex][4 + i]);
  }

  // Esperar la duración del movimiento
  delay(duracion);

  // Detener los motores al finalizar
  detener();
}

void detener() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(motorPins[i], LOW);
  }
  for (int i = 0; i < 2; i++) {
    analogWrite(speedPins[i], 0);
  }
}

int medirDistancia() {
  // Enviar un pulso de trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo del eco
  unsigned long duracion = pulseIn(echoPin, HIGH);

  // Convertir a distancia en cm
  int distancia = duracion * 0.034 / 2;
  return distancia;
}

void loop() {
  // Obtener la distancia medida por el sensor ultrasónico
  int distancia = medirDistancia();
  Serial.print("Distancia medida: ");
  Serial.println(distancia);

  // Decidir el movimiento basado en la distancia
  if (distancia > 30) {
    mover(0, 100);  // Avanzar si la distancia es mayor a 30 cm
  } else {
    int movimientoAleatorio = random(0, 3);  // Generar un número aleatorio para decidir el movimiento
    if (movimientoAleatorio == 0) {
      mover(4, 500);  // Girar a la derecha
    } else if (movimientoAleatorio == 1) {
      mover(3, 500);  // Girar a la izquierda
    } else {
      mover(2, 500);  // Retroceder
    }
  }
}
