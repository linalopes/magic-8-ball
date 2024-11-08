#include "SD.h"               // Biblioteca para acesso ao cartão SD
#include "TMRpcm.h"           // Biblioteca para reprodução de áudio
#include "SPI.h"              // Biblioteca de comunicação SPI


#define SD_ChipSelectPin 10   // Pino CS para o módulo SD
#define BUTTON_PIN 2          // Pino do botão (utiliza INPUT_PULLUP)


TMRpcm tmrpcm;                // Instância do objeto TMRpcm
const char *audioFiles[] = {"yes.wav", "no.wav", "maybe.wav"}; // Lista de arquivos de áudio
const int numFiles = sizeof(audioFiles) / sizeof(audioFiles[0]); // Quantidade de arquivos


bool lastButtonState = HIGH;  // Estado anterior do botão (para detectar mudanças)


void setup() {
  tmrpcm.speakerPin = 9;      // Define o pino de saída do speaker
  Serial.begin(9600);         // Inicializa comunicação serial para debug


  // Inicializa o módulo SD
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail"); // Mensagem de erro caso o SD não seja encontrado
    return;                    // Sai do setup se o cartão SD falhar
  }


  tmrpcm.setVolume(5);         // Ajusta o volume do áudio (0-7)
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o botão com resistor pull-up interno
}


void loop() {
  bool buttonState = digitalRead(BUTTON_PIN); // Lê o estado atual do botão


  // Verifica se o botão foi pressionado (mudança de HIGH para LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce para evitar leituras múltiplas causadas por ruído
    if (digitalRead(BUTTON_PIN) == LOW) { // Verifica novamente se o botão está pressionado
      playRandomAudio(); // Chama a função para tocar um áudio aleatório
    }
  }


  lastButtonState = buttonState; // Atualiza o estado anterior do botão
}


// Função para tocar um áudio aleatório
void playRandomAudio() {
  int randomIndex = random(numFiles); // Seleciona um índice aleatório
  const char *selectedFile = audioFiles[randomIndex]; // Obtém o arquivo correspondente


  Serial.print("Playing: ");  // Exibe no monitor serial qual arquivo será reproduzido
  Serial.println(selectedFile);


  tmrpcm.play(selectedFile);  // Reproduz o arquivo selecionado
}

