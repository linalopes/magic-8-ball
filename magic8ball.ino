#include "SD.h"               // Biblioteca para acesso ao cart�o SD
#include "TMRpcm.h"           // Biblioteca para reprodu��o de �udio
#include "SPI.h"              // Biblioteca de comunica��o SPI


#define SD_ChipSelectPin 10   // Pino CS para o m�dulo SD
#define BUTTON_PIN 2          // Pino do bot�o (utiliza INPUT_PULLUP)


TMRpcm tmrpcm;                // Inst�ncia do objeto TMRpcm
const char *audioFiles[] = {"yes.wav", "no.wav", "maybe.wav"}; // Lista de arquivos de �udio
const int numFiles = sizeof(audioFiles) / sizeof(audioFiles[0]); // Quantidade de arquivos


bool lastButtonState = HIGH;  // Estado anterior do bot�o (para detectar mudan�as)


void setup() {
  tmrpcm.speakerPin = 9;      // Define o pino de sa�da do speaker
  Serial.begin(9600);         // Inicializa comunica��o serial para debug


  // Inicializa o m�dulo SD
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail"); // Mensagem de erro caso o SD n�o seja encontrado
    return;                    // Sai do setup se o cart�o SD falhar
  }


  tmrpcm.setVolume(5);         // Ajusta o volume do �udio (0-7)
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o bot�o com resistor pull-up interno
}


void loop() {
  bool buttonState = digitalRead(BUTTON_PIN); // L� o estado atual do bot�o


  // Verifica se o bot�o foi pressionado (mudan�a de HIGH para LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce para evitar leituras m�ltiplas causadas por ru�do
    if (digitalRead(BUTTON_PIN) == LOW) { // Verifica novamente se o bot�o est� pressionado
      playRandomAudio(); // Chama a fun��o para tocar um �udio aleat�rio
    }
  }


  lastButtonState = buttonState; // Atualiza o estado anterior do bot�o
}


// Fun��o para tocar um �udio aleat�rio
void playRandomAudio() {
  int randomIndex = random(numFiles); // Seleciona um �ndice aleat�rio
  const char *selectedFile = audioFiles[randomIndex]; // Obt�m o arquivo correspondente


  Serial.print("Playing: ");  // Exibe no monitor serial qual arquivo ser� reproduzido
  Serial.println(selectedFile);


  tmrpcm.play(selectedFile);  // Reproduz o arquivo selecionado
}

