#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int ledAzul = 2;
const int botaoAzul = 7;

const int ledVermelho = 3;
const int botaoVermelho = 8;

const int ledAmarelo = 4;
const int botaoAmarelo = 9;

const int ledVerde = 5;
const int botaoVerde = 10;

const int ledBranco = 6;
const int botaoBranco = 11;

const int botaoStart = 12;

const int buzzer = 13;


const int leds[5] = {
  ledAzul,
  ledVermelho,
  ledAmarelo,
  ledVerde,
  ledBranco
};

const int botoes[5] = {
  botaoAzul,
  botaoVermelho,
  botaoAmarelo,
  botaoVerde,
  botaoBranco
};

const int notas[5] = {
  262,   // Azul
  330,   // Vermelho
  392,   // Amarelo
  523,   // Verde
  659    // Branco
};

// Guarda até 7 cores
int sequencia[7];

// Nível atual
int nivel = 1;

// Pontuação
int pontos = 0;

// Acertos no nível atual
int acertosNoNivel = 0;

// Começa com 3 cores
int tamanhoSequencia = 3;

// Tempo que cada LED fica aceso
int tempoLed = 500;

// Pausa entre as cores
int pausa = 250;

void setup() {

  // Configura os 5 LEDs
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Configura os 5 botões
  for (int i = 0; i < 5; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }

  // Botão Start
  pinMode(botaoStart, INPUT_PULLUP);

  // Buzzer
  pinMode(buzzer, OUTPUT);

  // LCD Grove 16x2
  lcd.begin(16, 2);

  // Cor do LCD RGB
  lcd.setRGB(0, 150, 255);

  // Tela inicial
  telaInicial();

  // Inicializa números pseudoaleatórios
  randomSeed(analogRead(A0));
}

void loop() {

  // Espera o jogador apertar START
  while (digitalRead(botaoStart) == HIGH) {
  }

  // Pequeno debounce
  delay(50);

  // Espera o jogador soltar o START
  while (digitalRead(botaoStart) == LOW) {
  }

  // Inicia o jogo
  iniciarJogo();

  while (true) {

    // Mostra nível, pontos e acertos
    mostrarStatus();

    delay(1000);

    // Gera uma nova sequência
    gerarSequencia();

    // Mostra a sequência para o jogador
    mostrarSequencia();


    // Mensagem "Sua vez"
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SUA VEZ!");

    lcd.setCursor(0, 1);
    lcd.print("Pts:");
    lcd.print(pontos);

    delay(500);


    // Verifica a resposta
    if (!jogadorAcertou()) {

      gameOver();

      break;
    }

    pontos++;

    acertosNoNivel++;

    musicaAcerto();


    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("ACERTO!");

    lcd.setCursor(0, 1);
    lcd.print("Pts:");
    lcd.print(pontos);

    delay(900);

    if (acertosNoNivel >= 3) {

      subirNivel();

    } else {

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Acertos:");

      lcd.print(acertosNoNivel);
      lcd.print("/3");

      lcd.setCursor(0, 1);
      lcd.print("Nivel:");
      lcd.print(nivel);

      delay(1000);
    }
  }
}


void telaInicial() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   NEUROPLAY");

  lcd.setCursor(0, 1);
  lcd.print("APERTE START");
}

void iniciarJogo() {

  nivel = 1;

  pontos = 0;

  acertosNoNivel = 0;

  tamanhoSequencia = 3;

  tempoLed = 500;

  pausa = 250;


  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("PREPARE-SE!");

  lcd.setCursor(0, 1);
  lcd.print("JOGO INICIANDO");

  musicaInicio();

  delay(1000);


  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Nivel: 1");

  lcd.setCursor(0, 1);
  lcd.print("3 cores");

  delay(1200);
}

void mostrarStatus() {

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Nivel:");
  lcd.print(nivel);

  lcd.setCursor(9, 0);

  lcd.print("Pts:");
  lcd.print(pontos);


  lcd.setCursor(0, 1);

  lcd.print("Acertos:");

  lcd.print(acertosNoNivel);

  lcd.print("/3");
}

void gerarSequencia() {

  for (int i = 0; i < tamanhoSequencia; i++) {

    sequencia[i] = random(0, 5);
  }
}

void mostrarSequencia() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("OBSERVE!");

  lcd.setCursor(0, 1);
  lcd.print("Nivel:");
  lcd.print(nivel);

  delay(800);


  for (int i = 0; i < tamanhoSequencia; i++) {

    int cor = sequencia[i];


    // Acende o LED correspondente
    digitalWrite(leds[cor], HIGH);

    // Toca a nota correspondente
    tone(buzzer, notas[cor], tempoLed);

    // Mantém o LED ligado
    delay(tempoLed);

    // Apaga o LED
    digitalWrite(leds[cor], LOW);

    // Para o som
    noTone(buzzer);

    // Pausa antes da próxima cor
    delay(pausa);
  }
}

bool jogadorAcertou() {

  for (int i = 0; i < tamanhoSequencia; i++) {

    // Espera o jogador apertar um botão
    int resposta = esperarBotao();

    // Mostra a cor pressionada no LCD
    mostrarCor(resposta);


    // Compara a resposta com a sequência
    if (resposta != sequencia[i]) {

      return false;
    }

    delay(100);
  }

  return true;
}

int esperarBotao() {

  while (true) {

    for (int i = 0; i < 5; i++) {

      // INPUT_PULLUP:
      // LOW = botão pressionado
      if (digitalRead(botoes[i]) == LOW) {

        // Debounce
        delay(30);

        if (digitalRead(botoes[i]) == LOW) {

          // Acende o LED da cor pressionada
          digitalWrite(leds[i], HIGH);

          // Toca a nota da cor
          tone(buzzer, notas[i]);

          // Espera o jogador soltar o botão
          while (digitalRead(botoes[i]) == LOW) {
          }

          // Apaga o LED
          digitalWrite(leds[i], LOW);

          // Para o buzzer
          noTone(buzzer);

          delay(50);

          return i;
        }
      }
    }
  }
}

void mostrarCor(int cor) {

  lcd.clear();

  lcd.setCursor(0, 0);

  switch (cor) {

    case 0:
      lcd.print("AZUL");
      break;

    case 1:
      lcd.print("VERMELHO");
      break;

    case 2:
      lcd.print("AMARELO");
      break;

    case 3:
      lcd.print("VERDE");
      break;

    case 4:
      lcd.print("BRANCO");
      break;
  }


  lcd.setCursor(0, 1);

  lcd.print("Pts:");
  lcd.print(pontos);
}

void subirNivel() {

  // Aumenta o nível
  nivel++;

  // Reinicia os acertos do nível
  acertosNoNivel = 0;


  // Aumenta uma cor na sequência
  if (tamanhoSequencia < 7) {

    tamanhoSequencia++;
  }


  // Quando chegar a 7 cores,
  // começa a aumentar a velocidade

  if (tamanhoSequencia >= 7) {

    if (tempoLed > 180) {
      tempoLed -= 40;
    }

    if (pausa > 80) {
      pausa -= 20;
    }
  }


  // Mensagem de novo nível
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("NOVO NIVEL!");

  lcd.setCursor(0, 1);
  lcd.print("Nivel:");
  lcd.print(nivel);

  delay(1500);


  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Sequencia:");

  lcd.setCursor(0, 1);
  lcd.print(tamanhoSequencia);
  lcd.print(" cores");

  delay(1200);
}

void musicaInicio() {

  tone(buzzer, 294, 125);
  delay(125);

  tone(buzzer, 294, 125);
  delay(125);

  tone(buzzer, 587, 250);
  delay(250);

  tone(buzzer, 440, 260);
  delay(375);

  tone(buzzer, 415, 125);
  delay(250);

  tone(buzzer, 392, 250);
  delay(250);

  tone(buzzer, 349, 250);
  delay(250);

  tone(buzzer, 294, 125);
  delay(125);

  tone(buzzer, 349, 125);
  delay(125);

  tone(buzzer, 392, 125);
  delay(125);

  tone(buzzer, 261, 125);
  delay(62);

  tone(buzzer, 261, 125);
  delay(62);

  tone(buzzer, 261, 125);
  delay(62);

  tone(buzzer, 261, 125);
  delay(62);

  noTone(buzzer);
}


void musicaAcerto() {

  tone(buzzer, 523, 100);
  delay(120);

  tone(buzzer, 659, 100);
  delay(120);

  tone(buzzer, 784, 100);
  delay(120);

  tone(buzzer, 1047, 300);
  delay(350);

  noTone(buzzer);
}

void musicaGameOver() {

  tone(buzzer, 392, 400);
  delay(450);

  tone(buzzer, 330, 400);
  delay(450);

  tone(buzzer, 262, 400);
  delay(450);

  tone(buzzer, 220, 600);
  delay(700);

  tone(buzzer, 392, 300);
  delay(350);

  tone(buzzer, 330, 300);
  delay(350);

  tone(buzzer, 294, 500);
  delay(600);

  noTone(buzzer);
}

void gameOver() {

  // Música de Game Over
  musicaGameOver();


  // Mostra resultado
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("  GAME OVER");

  lcd.setCursor(0, 1);
  lcd.print("Pontos:");
  lcd.print(pontos);


  // Pisca todos os LEDs
  for (int j = 0; j < 3; j++) {

    for (int i = 0; i < 5; i++) {
      digitalWrite(leds[i], HIGH);
    }

    delay(200);


    for (int i = 0; i < 5; i++) {
      digitalWrite(leds[i], LOW);
    }

    delay(200);
  }


  delay(1500);


  // Volta para a tela inicial
  telaInicial();
}