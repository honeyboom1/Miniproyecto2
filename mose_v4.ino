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
  Serial.println("Por favor, ingrese el mensaje!!!");
  int numRepeticiones = 1;
}

void loop() {
  if (Serial.available() > 0) {
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
  if (IrReceiver.decode()) {
    handleReceivedSignal(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // Recibir el siguiente valor
    delay(tiempo);
  }
  for (int j = 0; j < numRepeticiones ; j++) {  
    for (int i = 0; i < mensaje.length(); i++) {
      char c = mensaje.charAt(i);
      if (c == ' ') {
        IrSender.sendNEC(0x1234, 0x06, 1);
        delay(tiempo * 7); // Pausa palabras
          if (IrReceiver.decode()) {
      handleReceivedSignal(IrReceiver.decodedIRData.command);
      IrReceiver.resume(); // Recibir el siguiente valor
      delay(tiempo);
    }
      } else {
        F_morsificador(c);
        IrSender.sendNEC(0x1234, 0x05, 1);
        delay(tiempo * 3); // Pausa letras
          if (IrReceiver.decode()) {
      handleReceivedSignal(IrReceiver.decodedIRData.command);
      IrReceiver.resume(); // Recibir el siguiente valor
    }
        
      }
    }
  }  
  IrSender.sendNEC(0x1234, 0x07, 1);
  delay(tiempo);
    if (IrReceiver.decode()) {
    handleReceivedSignal(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // Recibir el siguiente valor
    delay(tiempo);
  }
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
      if (IrReceiver.decode()) {
    handleReceivedSignal(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // Recibir el siguiente valor
    delay(tiempo);
  }
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
  if (IrReceiver.decode()) {
    handleReceivedSignal(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // Recibir el siguiente valor
    delay(tiempo);
  }
}

void elemento_fantasma() {
  IrSender.sendNEC(0x1234, 0x04, 1);
  delay(tiempo);
      if (IrReceiver.decode()) {
    handleReceivedSignal(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // Recibir el siguiente valor
    delay(tiempo);
  }
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


void handleReceivedSignal(uint8_t command) {
  int r = 0;
  if (command != 0x01 && command != 0x05) { // Corregido a &&
    Serial.print(command, HEX);
    Serial.print(" ");
  }
  if (command == 0x02) {
    MENSAJE___1 += '.';
  } 
  else if (command == 0x03) {
    MENSAJE___1 += '-';
  }
  else if (command == 0x06) {
    for (r = 0; r < 5; r++) { // Corregido el bucle for
      MENSAJE___1 += '!';
    }
  }
  else if (command == 0x04) { // fantasma
    MENSAJE___1 += '+';
  } 
  else if (command == 0x07) {
    decodeMorsemensaje();
  }
}

void decodeMorsemensaje() {
  String mensajeDecodificado = "";
  String letraMorse = "";
  Serial.println("\nFinal Morse:");
  for (int i = 0; i < MENSAJE___1.length(); i++) {
    Serial.print(MENSAJE___1.charAt(i));
  }
  Serial.println();

  for (int i = 0; i < MENSAJE___1.length(); i++) {
      if ((i% 5) != 0 && i != 0) {
        letraMorse += MENSAJE___1.charAt(i); // Agregar punto o guion a la letra Morse actual
      } 
      else {
        mensajeDecodificado += traducirMorse(letraMorse); // Traducir letra Morse a carácter
        letraMorse = ""; // Resetear la letra Morse para la siguiente
        letraMorse += MENSAJE___1.charAt(i);
      }
  }
  
  // Traduce la última letra Morse si no hay un espacio al final
  if (letraMorse != "") {
    mensajeDecodificado += traducirMorse(letraMorse);
  }
  mensajeDecodificado = mensajeDecodificado.substring(1);
  if (mensajeDecodificado == "SOS") {
    for (int i = 0; i < 7; i++) {
      tone(pin_buzzer, 1000, 200); // Tono agudo y corto
      delay(250);
    }
  }
  else if (mensajeDecodificado == "TIC") {
    // Sonido de victoria (ejemplo)
    tone(pin_buzzer, 440, 500); // Tono La (440 Hz) durante 500 ms
    delay(500);
    tone(pin_buzzer, 880, 500); // Tono La una octava más alta
    delay(500);
  }
  Serial.println("mensaje recibido: " + mensajeDecodificado);
  MENSAJE___1 = ""; // Limpiar el mensaje recibido
}

char traducirMorse(String letraMorse) {
  // Letras
    if (letraMorse == ".-+++") return 'A';
    if (letraMorse == "-...+") return 'B';
    if (letraMorse == "-.-.+") return 'C';
    if (letraMorse == "-..++") return 'D';
    if (letraMorse == ".++++") return 'E';
    if (letraMorse == "..-.+") return 'F';  // Corregido (era "..-." antes)
    if (letraMorse == "--.++") return 'G';
    if (letraMorse == "....+") return 'H';
    if (letraMorse == "..+++") return 'I';  // Corregido (era "..+++" antes)
    if (letraMorse == ".---+") return 'J';
    if (letraMorse == "-.-++") return 'K';
    if (letraMorse == ".-..+") return 'L';
    if (letraMorse == "--+++") return 'M';
    if (letraMorse == "-.+++") return 'N';
    if (letraMorse == "---++") return 'O';
    if (letraMorse == ".--.+") return 'P';
    if (letraMorse == "--.-+") return 'Q';
    if (letraMorse == ".-.++") return 'R'; // Sin cambios
    if (letraMorse == "...++") return 'S';
    if (letraMorse == "-++++") return 'T';
    if (letraMorse == "..-++") return 'U';
    if (letraMorse == "...-+") return 'V';
    if (letraMorse == ".--++") return 'W';
    if (letraMorse == "-..-+") return 'X';
    if (letraMorse == "-.--+") return 'Y';
    if (letraMorse == "--..+") return 'Z';
    if (letraMorse == "!!!!!") return ' ';
  // Números
  if (letraMorse == ".----") return '1';
  if (letraMorse == "..---") return '2';
  if (letraMorse == "...--") return '3';
  if (letraMorse == "....-") return '4';
  if (letraMorse == ".....") return '5';
  if (letraMorse == "-....") return '6';
  if (letraMorse == "--...") return '7';
  if (letraMorse == "---..") return '8';
  if (letraMorse == "----.") return '9';
  if (letraMorse == "-----") return '0';
  ; // Devuelve un carácter de interrogación si no se encuentra la secuencia Morse
}