#include <WiFi.h>
#include <HTTPClient.h>

// Definição dos pinos dos LEDs
#define LED_VERDE 41
#define LED_AMARELO 9
#define LED_VERMELHO 40

// Definição dos pinos do botão e do LDR
#define BOTAO_PIN 18
#define LDR_PIN 4

// Limiar para o sensor LDR (define claro e escuro)
const int LDR_THRESHOLD = 600;

// Variáveis para debounce do botão
unsigned long ultimaAlteracaoBotao = 0;
const unsigned long debounceDelay = 50;
int estadoBotaoAtual = LOW;
int estadoBotaoAnterior = LOW;
int contadorPressionamentos = 0;

// Variáveis de tempo para o semáforo
unsigned long ultimoEstadoSemaforo = 0;
const int TEMPO_VERDE = 3000;
const int TEMPO_AMARELO = 2000;
const int TEMPO_VERMELHO = 5000;


// Configurações de Wi-Fi falta conectar o outro lado do botao
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

// Estados do sistema
enum Modo { NOTURNO, DIURNO };
enum EstadoSemaforo { VERDE, AMARELO, VERMELHO };
Modo modoAtual = DIURNO;
EstadoSemaforo estadoAtualSemaforo = VERDE;

// Função para conectar ao Wi-Fi
void conectarWiFi() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);

  unsigned long tempoInicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicio < 15000) { // Tenta conectar por 15 segundos
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi.");
  }
}

void setup() {
  // Inicialização do monitor serial
  Serial.begin(115200);

  // Configuração dos LEDs como saídas
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Inicializa os LEDs (desligados)
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  // Configuração do botão e do LDR
  pinMode(BOTAO_PIN, INPUT_PULLDOWN);

  // Conecta ao Wi-Fi
  conectarWiFi();
}

void loop() {
  // Leitura do valor do LDR
  int valorLDR = analogRead(LDR_PIN);

  // Verifica se está escuro ou claro
  if (valorLDR < LDR_THRESHOLD) {
    ativarModoNoturno();
  } else {
    ativarModoDiurno();
  }
}

// Função para ativar o modo noturno
void ativarModoNoturno() {
  if (modoAtual != NOTURNO) {
    modoAtual = NOTURNO;
    Serial.println("Modo noturno ativado");
    // Desliga todos os LEDs, exceto o amarelo
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
  }

  // Pisca o LED amarelo a cada 1 segundo
  static unsigned long ultimoPiscar = 0;
  if (millis() - ultimoPiscar >= 1000) {
    ultimoPiscar = millis();
    digitalWrite(LED_AMARELO, !digitalRead(LED_AMARELO));
  }
}

// Função para ativar o modo diurno
void ativarModoDiurno() {
  if (modoAtual != DIURNO) {
    modoAtual = DIURNO;
    Serial.println("Modo diurno ativado");
    digitalWrite(LED_AMARELO, LOW); // Desliga o LED amarelo
  }

  // Lógica do semáforo (alternância entre as cores)
  unsigned long tempoAtual = millis();
  switch (estadoAtualSemaforo) {
    case VERDE:
      if (tempoAtual - ultimoEstadoSemaforo >= TEMPO_VERDE) {
        mudarEstadoSemaforo(AMARELO);
      }
      break;
    case AMARELO:
      if (tempoAtual - ultimoEstadoSemaforo >= TEMPO_AMARELO) {
        mudarEstadoSemaforo(VERMELHO);
      }
      break;
    case VERMELHO:
      if (tempoAtual - ultimoEstadoSemaforo >= TEMPO_VERMELHO) {
        mudarEstadoSemaforo(VERDE);
      }
      verificarBotao();
      break;
  }
}

// Função para mudar o estado do semáforo
void mudarEstadoSemaforo(EstadoSemaforo novoEstado) {
  ultimoEstadoSemaforo = millis();
  estadoAtualSemaforo = novoEstado;

  // Atualiza os LEDs
  digitalWrite(LED_VERDE, novoEstado == VERDE);
  digitalWrite(LED_AMARELO, novoEstado == AMARELO);
  digitalWrite(LED_VERMELHO, novoEstado == VERMELHO);
}

// Função para verificar o botão com debounce
void verificarBotao() {
  int leitura = digitalRead(BOTAO_PIN);

  // Implementa debounce
  if (leitura != estadoBotaoAnterior) {
    ultimaAlteracaoBotao = millis();
  }
  if ((millis() - ultimaAlteracaoBotao) > debounceDelay) {
    if (leitura != estadoBotaoAtual) {
      estadoBotaoAtual = leitura;
      if (estadoBotaoAtual == HIGH) {
        contadorPressionamentos++;
        Serial.println("Botão pressionado!");

        if (contadorPressionamentos == 3) {
          enviarRequisicaoHTTP();
          contadorPressionamentos = 0; // Reseta o contador
        }
      }
    }
  }
  estadoBotaoAnterior = leitura;
}

// Função para enviar uma requisição HTTP
void enviarRequisicaoHTTP() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://www.google.com.br/");
    int codigoRespostaHTTP = http.GET();
    if (codigoRespostaHTTP > 0) {
      Serial.print("Resposta HTTP: ");
      Serial.println(codigoRespostaHTTP);
    } else {
      Serial.print("Erro na requisição HTTP: ");
      Serial.println(codigoRespostaHTTP);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi não conectado. Não é possível enviar requisição HTTP.");
  }
}