#include <Servo.h>

#define pinBotaoT 2 
#define pinBotao1 3
#define pinBotao2 4
#define pinBotao3 5

#define pinSensorT 6
#define pinSensor1 7
#define pinSensor2 8
#define pinSensor3 9

#define nivelSensor LOW

#define pinServo 12
#define servoParado 90
#define servoSobe   115 
#define servoDesce  77
#define servoSobeLento  108
#define servoDesceLento  82

#define velocidadeLenta 5
#define velocidadeMedia 15
#define velocidadeRapida 30

class PushButton {
  public:
    PushButton(byte pinBotao, int tempoDebounce = 200);
    void button_loop(); 
    bool pressed();
  private:
    unsigned long debounceBotao;
    bool estadoBotaoAnt = HIGH;
    bool apertado = false;
    byte pino;
    int  tempo;
};

PushButton::PushButton(byte pinBotao, int tempoDebounce) {
  pinMode(pinBotao, INPUT_PULLUP);
  pino = pinBotao;
  tempo = tempoDebounce;
}

void PushButton::button_loop() {
  bool estadoBotao = digitalRead(pino);
  apertado = false;
  if (  (millis() - debounceBotao) > tempo ) {
     if (!estadoBotao && estadoBotaoAnt) {
         apertado = true;
         debounceBotao = millis();
     }
  }  
  estadoBotaoAnt = estadoBotao;
}

bool PushButton::pressed() {
  return apertado;
}

PushButton botaoT(pinBotaoT);
PushButton botao1(pinBotao1);
PushButton botao2(pinBotao2);
PushButton botao3(pinBotao3);

Servo motor;

byte andarChamado = 9;  //Número do Andar que Foi Chamado. O número 9 significa que nenhum andar foi chamado.
unsigned long delayTempo;

byte andar();

void setup() {
  Serial.begin(9600);

  pinMode(pinSensorT, INPUT);
  pinMode(pinSensor1, INPUT);
  pinMode(pinSensor2, INPUT);
  pinMode(pinSensor3, INPUT);

  motor.attach(pinServo);
}

void loop() {
  botaoT.button_loop();
  botao1.button_loop();
  botao2.button_loop();
  botao3.button_loop();
  
  if (andarChamado == 9) {
      //Se nenhum andar foi chamado

      if (botaoT.pressed()) {
         andarChamado = 0;
         delayTempo = millis();
      }
    
      if (botao1.pressed()) {
         andarChamado = 1;
         delayTempo = millis();
      }
    
      if (botao2.pressed()) {
         andarChamado = 2;
         delayTempo = millis();
      }
    
      if (botao3.pressed()) {
         andarChamado = 3;
         delayTempo = millis();
      }
      
      if (andar() != 9) {
         //Se o elevador se encontra num andar identificado, para o elevador
         motor.write(servoParado); 
      } 
    
      if (andar() == 9) {
         //Se o elevador se encontra fora de um andar, posiciona o elevador no andar abaixo
         motor.write(servoDesceLento); 
      }
  } else {
      //Se um andar foi chamado 

      if (andar() != 9) {
          if (andarChamado > andar()) {
             //Sobe o elevador se ele esta abaixo do andar chamado
             motor.write(servoSobe);
          }
    
          if (andarChamado < andar()) {
             //Desce o elevador se ele esta acima do andar chamado
             motor.write(servoDesce);
          }
    
          if (andarChamado == andar()) {
             //Se chegou no andar, para o elevador
             if ((millis() - delayTempo) > 200 ) {
                motor.write(servoParado);
                andarChamado = 9;
             }
          }
      }
  }

  Serial.println(andarChamado);
  
}

byte andar() {
  if (digitalRead(pinSensorT) == nivelSensor) {
     return 0;
  } else if (digitalRead(pinSensor1) == nivelSensor) {
     return 1;
  } else if (digitalRead(pinSensor2) == nivelSensor) {
     return 2;
  } else if (digitalRead(pinSensor3) == nivelSensor) {
     return 3;
  } else {
     return 9;
  }
}
