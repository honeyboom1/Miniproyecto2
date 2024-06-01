#include <IRremote.h>

int pin_receptor = 2; 
int pin_buzzer = 4; 
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 50; // 50 milisegundos de retraso
const int tiempo = 150; // Tiempo base en milisegundos para un punto

String MENSAJE___1 = "";

void setup() {
    Serial.begin(9600);
    pinMode(pin_buzzer, OUTPUT);
    IrReceiver.begin(pin_receptor, DISABLE_LED_FEEDBACK); // Iniciar receptor IR
    Serial.println("Receptor IR listo para recibir señales...");
}

void loop() {
    if (IrReceiver.decode()) {
        if (millis() - lastButtonPress > debounceDelay) {
            handleReceivedSignal(IrReceiver.decodedIRData.command);
            lastButtonPress = millis(); 
            delay(150);
        }
        IrReceiver.resume(); // Prepararse para la siguiente señal
    }
}

void handleReceivedSignal(uint8_t command) {
    int r = 0;
    if (command != 0x01 && command != 0x05) { 
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
        for (r = 0; r < 5; r++) { 
            MENSAJE___1 += '!';
        }
    }
    else if (command == 0x04) { 
        MENSAJE___1 += '+';
    } 
    else if (command == 0x07) {
        decodeMorsemensaje();
    }
}

// ... (el resto de las funciones: decodeMorsemensaje, traducirMorse) ...
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
  IrReceiver.resume();
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