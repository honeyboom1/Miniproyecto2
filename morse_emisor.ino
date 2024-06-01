#include <IRremote.h>

int pin_transmisor = 3;
int pin_receptor = 2;
int pin_buzzer = 4;
int LED_PIN_TRANSMIT = 5;  // Ejemplo para un LED de color

const int tiempo = 150; // Tiempo base en milisegundos para un punto

String MENSAJE___1 = "";

void setup() {
  Serial.begin(9600);
  pinMode(pin_transmisor, OUTPUT);
  pinMode(pin_buzzer, OUTPUT);
  pinMode(LED_PIN_TRANSMIT, OUTPUT);
  IrReceiver.begin(pin_receptor, DISABLE_LED_FEEDBACK);
  IrSender.begin(pin_transmisor, DISABLE_LED_FEEDBACK, 0);
  int numRepeticiones = 1;
}

void loop() {
  if (Serial.available() > 0) {
    Serial.println("Por favor, ingrese el mensaje!!!");
    String mensaje = Serial.readString(); // mensaje = mensaje que ingresamos en el monitor serial
    
    Serial.println("¿Cuántas veces quieres enviar el mensaje?");
      while (Serial.available() == 0) {
            // Esperar a que el usuario ingrese un número
      }
      int numRepeticiones = Serial.parseInt();
    
    for (int i = 0; i < mensaje.length(); i++) {
      char c = mensaje.charAt(i);
      Serial.print(c);
    }
    Serial.println();
    Serial.println("Init Morse:");
    F_enviarmorse(mensaje, numRepeticiones);
  }
}

void F_enviarmorse(String mensaje, int numRepeticiones)  {
  digitalWrite(LED_PIN_TRANSMIT, HIGH); // led q se prende cuando se envia mensaje
  IrSender.sendNEC(0x1234, 0x01, 1);
  delay(tiempo);
  for (int j = 0; j < numRepeticiones ; j++) {  
    for (int i = 0; i < mensaje.length(); i++) {
      char c = mensaje.charAt(i);
      if (c == ' ') {
        IrSender.sendNEC(0x1234, 0x06, 1);
        delay(tiempo * 7); // Pausa palabras
      } else {
        F_morsificador(c);
        IrSender.sendNEC(0x1234, 0x05, 1);
        delay(tiempo * 3); // Pausa letra  
      }
    }
  }  
  IrSender.sendNEC(0x1234, 0x07, 1);
  delay(tiempo);
  tone(pin_buzzer, 500, tiempo * 2);
  digitalWrite(LED_PIN_TRANSMIT, LOW); // Fin de la transmisión
}

void F_morsificador(char c) {
  String codigo_morse = getcodigo_morse(c);
    Serial.println();
  for (int i = 0; i < codigo_morse.length(); i++) {
    if (codigo_morse.charAt(i) == '.') {
      sendDot();
    } 
    else if (codigo_morse.charAt(i) == '-') {
      sendDash();
    }
    else if (codigo_morse.charAt(i) == '+') {
      elemento_fantasma();
    }
  }
    for (int i = 0; i < codigo_morse.length(); i++) {
      char c = codigo_morse.charAt(i);
      Serial.print(c);
    }
}

void sendDot() {
  IrSender.sendNEC(0x1234, 0x02, 1);
  delay(50);
    for (int i = 0; i < tiempo; i++) { 
        analogWrite(pin_buzzer, 128);  // 50% duty cycle (volumen medio)
        delayMicroseconds(500);       // Aproximadamente 1 kHz
        analogWrite(pin_buzzer, 0);
        delayMicroseconds(500);
    }
  delay(tiempo);
}

void sendDash() {
  IrSender.sendNEC(0x1234, 0x03, 1);
  delay(50);
    for (int i = 0; i < tiempo; i++) { 
        analogWrite(pin_buzzer, 128);  // 50% duty cycle (volumen medio)
        delayMicroseconds(250);       // Aproximadamente 1 kHz
        analogWrite(pin_buzzer, 0);
        delayMicroseconds(250);
    }
  delay(tiempo * 3);
}

void elemento_fantasma() {
  IrSender.sendNEC(0x1234, 0x04, 1);
  delay(tiempo);
}

String getcodigo_morse(char c) {
  // Letras
  if (c == 'A' || c == 'a') return ".-+++";
  if (c == 'B' || c == 'b') return "-...+";
  if (c == 'C' || c == 'c') return "-.-.+";
  if (c == 'D' || c == 'd') return "-..++";
  if (c == 'E' || c == 'e') return ".++++";
  if (c == 'F' || c == 'f') return "..-.+";
  if (c == 'G' || c == 'g') return "--.++";
  if (c == 'H' || c == 'h') return "....+";
  if (c == 'I' || c == 'i') return "..+++";
  if (c == 'J' || c == 'j') return ".---+";
  if (c == 'K' || c == 'k') return "-.-++";
  if (c == 'L' || c == 'l') return ".-..+";
  if (c == 'M' || c == 'm') return "--+++";
  if (c == 'N' || c == 'n') return "-.+++";
  if (c == 'O' || c == 'o') return "---++";
  if (c == 'P' || c == 'p') return ".--.+";
  if (c == 'Q' || c == 'q') return "--.-+";
  if (c == 'R' || c == 'r') return ".-.++";
  if (c == 'S' || c == 's') return "...++";
  if (c == 'T' || c == 't') return "-++++";
  if (c == 'U' || c == 'u') return "..-++";
  if (c == 'V' || c == 'v') return "...-+";
  if (c == 'W' || c == 'w') return ".--++";
  if (c == 'X' || c == 'x') return "-..-+";
  if (c == 'Y' || c == 'y') return "-.--+";
  if (c == 'Z' || c == 'z') return "--..+";
  
  // Números
  if (c == '1') return ".----";
  if (c == '2') return "..---";
  if (c == '3') return "...--";
  if (c == '4') return "....-";
  if (c == '5') return ".....";
  if (c == '6') return "-....";
  if (c == '7') return "--...";
  if (c == '8') return "---..";
  if (c == '9') return "----.";
  if (c == '0') return "-----";
  
  return ""; // Devuelve una cadena vacía si no se encuentra el carácter
}
