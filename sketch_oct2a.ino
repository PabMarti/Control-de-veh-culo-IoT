const int ruadas[] = {16, 17, 18, 19}; // Pines para M1, M2, M3, M4
const int enable[] = {27, 32}; // Pines para ENA, ENB
const int EchoPin = 5;
const int TriggerPin = 4;

// Matriz de movimientos: {M1, M2, M3, M4, ENA, ENB}
const int movimientos[8][6] = {
  {HIGH, LOW, LOW, HIGH, 255, 255}, // Adelante
  {LOW, LOW, HIGH, HIGH, 0, 0},     // Detenerse
  {LOW, HIGH, HIGH, LOW, 255, 255}, // Reversa
  {LOW, LOW, LOW, HIGH, 0, 250},    // Giro Izquierda
  {HIGH, LOW, HIGH, HIGH, 255, 0},  // Giro Derecha
  {HIGH, LOW, LOW, HIGH, 100, 255}, // Curva Izquierda
  {HIGH, LOW, LOW, HIGH, 255, 100}, // Curva Derecha
  {LOW, LOW, LOW, LOW, 0, 0},       // Apagar motores
};

void setup() {
  // Configuración de pines de motores y habilitadores
  for (int i = 0; i < 4; i++) pinMode(ruedas[i], OUTPUT);
  for (int i = 0; i < 2; i++) pinMode(enable[i], OUTPUT);

  // Inicializar todos los pines a LOW
  for (int i = 0; i < 4; i++) digitalWrite(ruedas[i], LOW);
  for (int i = 0; i < 2; i++) digitalWrite(enable[i], LOW);

  // Inicializar Trigger y Echo
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void ejecutarMovimiento(int movimiento, int tiempo) {
  // Asignar las señales según el movimiento seleccionado
  for (int i = 0; i < 4; i++) digitalWrite(motores[i], movimientos[movimiento][i]);
  
  // Ajustar la velocidad mediante PWM
  for (int i = 0; i < 2; i++) analogWrite(enable[i], movimientos[movimiento][4 + i]);

  // Esperar el tiempo especificado
  delay(tiempo);
}
// Medir distancia sensor US
int ping(int TriggerPin, int EchoPin) {
unsigned short int duration;
float distanceCm;

digitalWrite(TriggerPin, LOW); //para generar un pulso limpio ponemos a LOW 4us
delayMicroseconds(4);
digitalWrite(TriggerPin, HIGH); //generamos Trigger (disparo) de 10us
delayMicroseconds(10);
digitalWrite(TriggerPin, LOW);

duration = pulseIn(EchoPin, HIGH); //medimos el tiempo entre pulsos, en microsegundos

distanceCm = float(duration)/58.2; //convertimos a distancia, en cm
return distanceCm;
}

void loop() {
  // Ejecución secuencial de movimientos
 /*  int secuencia[] = {0, 2, 3, 4, 5, 6, 1}; // Secuencia de movimientos
  for (int i = 0; i < sizeof(secuencia) / sizeof(secuencia[0]); i++) {
    ejecutarMovimiento(secuencia[i], 200);
  } */
  float cm = ping(TriggerPin, EchoPin);

  if (cm > 30){
    ejecutarMovimiento(0, 100);
  }else{
  if (random(0, 1) == 0){
    ejecutarMovimiento(4, 450);
  }else{
    ejecutarMovimiento(3, 450);
  }
  }
}