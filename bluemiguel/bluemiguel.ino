#include <Wire.h>
#include "RTClib.h"
#include "Thread.h"
#include "ThreadController.h"
ThreadController cpu;

/*
  # Endereco RTC/EPROM 0x50 0x68
*/

// Armazena o buffer dos caractares que cheram pela serial
#define BUFFER_SIZE 20

// Tamanho maximo do array de bytes que chegam da serial
int buf[BUFFER_SIZE];


// Posicao de cada dado salvo em memoria.
int memoria[20]    = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

// cada item e um dia da semana
//int dia_semana[8] = {0,0,0,0,0,0,0,0};
//int intervalo[8]  = {0,0,0,0,0,0,0,0};

int recebe;
int valor;
//int resposta;
int PIN_ALARME = 9;
int regra;

boolean acionado = false;
boolean primeiraVez = true;

int configuracao = 0;
// mega
//DS1307  rtc(20, 21);

// acionamento
int ultimoAcionamento = 0;
int intervalorTempo = 0;

// uno
RTC_DS1307 rtc(A4, A5);

Time t;

Thread acionamento;
void verificaAcionamento() {
  
  t = rtc.getTime();
  /*
    t.down retorna o dia da semana
    onde: 
      1 - segunda
      2 - terca
      3 - quarta
      4 - quinta
      5 - sexta
      6 - sabado
      7 - domingo
      
      
      Salvando na variavel "memoria"
      
      eu pego o item do array, para pegar apartir do primeiro, se o primeiro dia da semana eh o numero 1, eu faco "dia_da_semana-1" e pego o primeiro item do array
      
      Pegando o dia da semana atual, eu verifico se neste dia da semana, eh para fazer o acionamento.
      
  */
  
  
  // pegando o dia dasemana para saber se aciona ou nao.
  int dia_da_semana = (int) rtc.peek(memoria[t.dow - 1]);
  
    /* 
      verifica se no dia atual resgatado do array acima eh para acionar ou nao
      nos primeiros 7 itens do array salvo na variavel "memoria", esta em ordem, os dias da semana retorando entre 0 e 1, e 0 ele nao aciona e 1 ele aciona
      , mas, essa porra ta pegando da tabela ascii, entao o 0 eh igual a 48 e o 1 eh igual a 49
    */
   
  // se neste dia eh para acionar, entra nesse if  
  if (dia_da_semana == 49) {
    
    
    Serial.println("Hoje aciona!");
    
    
    regra = rtc.peek(memoria[7]);
    /*  
    
      Esta REGRA, retorna do APP entre 0 e 9, que sao os intervalos de acionamento.
      Onde essa regra fica salvo no indice 7, memoria[7] do array "memoria"
      A regra de intervalo eh:
        0 - 15 min
        1 - 20 min
        2 - 25 min
        3 - 30 min
        4 - 35 min
        5 - 40 min
        6 - 45 min
        7 - 50 min
        8 - 55 min
        9 - 60 min
        
      REGRA
      48	0
      49	1
      50	2
      51	3
      52	4
      53	5
      54	6
      55	7
      56	8
      57	9
    */
    
    if(ultimoAcionamento ==0) {
      primeiraVez = true;
    }
    
    intervalorTempo = (int)(((t.hour*100) + t.min) - ultimoAcionamento);
    
    // 15mim
    if((intervalorTempo == 15 && regra == 48 && intervalorTempo > 2) || (primeiraVez && regra == 48)) {
      Serial.println("PASSOU 15 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 20min
    if((intervalorTempo == 20 && regra == 49 && intervalorTempo > 2) || (primeiraVez && regra == 49)) {
      Serial.println("PASSOU 20 MIM");
      acionado = true;
      primeiraVez = false;
    }
    

    // 25min
    if((intervalorTempo == 25 && regra == 50 && intervalorTempo > 2 ) || (primeiraVez && regra == 50)) {
      Serial.println("PASSOU 25 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 30min
    if((intervalorTempo == 30 && regra == 51 && intervalorTempo > 2) || (primeiraVez && regra == 51)) {
      Serial.println("PASSOU 30 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 35min
    if((intervalorTempo == 35 && regra == 52 && intervalorTempo > 2) || (primeiraVez && regra == 52)) {
      Serial.println("PASSOU 35 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 40min
    if((intervalorTempo == 40 && regra == 53 && intervalorTempo > 2 ) || (primeiraVez && regra == 53)) {
      Serial.println("PASSOU 40 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 45min
    if((intervalorTempo == 45 && regra == 54 && intervalorTempo > 2) || (primeiraVez && regra == 54)) {
      Serial.println("PASSOU 45 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 50min
    if((intervalorTempo == 50 && regra == 55 && intervalorTempo > 2) || (primeiraVez && regra == 55)) {
      Serial.println("PASSOU 50 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 55min
    if((intervalorTempo == 55 && regra == 56 && intervalorTempo > 2) || (primeiraVez && regra == 56)) {
      Serial.println("PASSOU 55 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    // 60min
    if((intervalorTempo == 60 && regra == 57 && intervalorTempo > 2) || (primeiraVez && regra == 57)) {
      Serial.println("PASSOU 60 MIM");
      acionado = true;
      primeiraVez = false;
    }
    
    
    
    
    if(acionado) {
      ultimoAcionamento = (t.hour*100) + t.min;
      acionaAlarme();
      primeiraVez = false;
      acionado = false;
    }
    
    
    Serial.println();
    Serial.print("Passou min: ");
    Serial.print(intervalorTempo, DEC);
    Serial.println();
        
    Serial.println();
    Serial.print("REGRA: ");
    Serial.print(regra, DEC);
    Serial.println();
    
    
      
    
  } 
  else {
    Serial.println("Hoje NAO aciona!");
  }


}


/*
  Esta thread apena mostra a serial, a Data atual salva na memoria do filho da puta do RTC maldito.
*/
Thread thread1;
void mostraDataHora(){
  //thread1.enabled = false;
  t = rtc.getTime();

  // Send date over serial connection
  Serial.print("Dia ");
  Serial.print(t.date);
  Serial.print(". Mes ");
  Serial.print(rtc.getMonthStr());
  Serial.print(" ano ");
  Serial.print(t.year);
  //Serial.println(".");

  // Send Day-of-Week and time
  Serial.print(" dia ");
  Serial.print(t.dow);
  Serial.print(". ");
  Serial.print(t.hour);
  Serial.print(" : ");
  Serial.print(t.min);
  Serial.write(" : ");
  Serial.print(t.sec);
  Serial.print("");
}




Thread Memoria;
void StatusDaMemoria(){

 t = rtc.getTime();
  Serial.print("{ \"dias\":[");
  
  // um for pelos indices do array memoria, onde esta os dias da semana de 0 a 6, para saber se eh para acionar ou nao
  // e pego o indice 7 que esta  a regra de intervalos de tempo
  for (int a = 0; a < 7; a++) {
    int aciona = rtc.peek(memoria[a]);

    Serial.print("{ \"dia\":\"");
    Serial.print(a,DEC);
    Serial.print("\",\"valor\":\"");  
    Serial.print(aciona,DEC);
    if(a != 6) {
      Serial.print("\"},");
    } 
    else {
      Serial.print("\"}");
    }
    delay(1);
  }
  //Serial.print("]}");

  Serial.print("], \"intervalo\":\"");
  Serial.print(rtc.peek(memoria[7]),DEC);
  Serial.print("\",\"dia\":\"");
  Serial.print(t.date);
  Serial.print("\",\"mes\":\""); 
  Serial.print(rtc.getMonthStr());
  Serial.print("\",\"ano\":\"");
  Serial.print(t.year);
  Serial.print("\",\"hora\":\""); 
  Serial.print(t.hour);
  Serial.print("\",\"minuto\":\""); 
  Serial.print(t.min);
  Serial.print("\",\"segundo\":\""); 
  Serial.print(t.sec);
  Serial.print("\"}");

  Serial.println();
  
}

void setup(){
  Serial.begin(9600);
  
  Wire.begin();
  rtc.begin();
  
  rtc.halt(false);
  
  
  t = rtc.getTime();
  
  //rtc.setDOW(6);        // Set Day-of-Week to SUNDAY
  //rtc.setTime(11, 56, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(9, 26, 2015);   // Set the date to October 3th, 201

    
  pinMode(PIN_ALARME, OUTPUT);
  digitalWrite(PIN_ALARME, LOW);
 
  thread1.setInterval(5000);
  thread1.onRun(mostraDataHora);

  

  acionamento.setInterval(2000);
  acionamento.onRun(verificaAcionamento);

  Memoria.setInterval(2000);
  Memoria.onRun(StatusDaMemoria);


  cpu.add(&thread1);
  //cpu.add(&threadMemoria);
  cpu.add(&acionamento);
  cpu.add(&Memoria);

  
  


  
 
}

unsigned int len = 0;


void loop()
{
  cpu.run();

  //Serial.write( msg, 7 );

  //Serial.write('rogerio');


  while (Serial.available()) {
    unsigned int c = Serial.read();
    if (c == 0xA || c == 0xD) { // \n or \r
      sendData();
    } 
    else {
      bufferData(c);
    }
  }
}




void bufferData(int c) {
  if (len < 21) {
    buf[len++] = (int) c;
    //rtc.poke(memoria[c], buf[c]);
  } // TODO warn, or send data
}

void sendData() {
  //Serial.write("Us: ");
  
   for (int i = 0; i < len; i++) {
    Serial.write(buf[i]);
    /*
    ############################
    //rtc.poke(memoria[i], buf[i]);
    */
   }
  
  /*for (int i = 0; i < len; i++) {
      rtc.poke(memoria[i], buf[i]);
  }*/

  Serial.write(0xA); // TODO test on windows
  len = 0;  
}


void acionaAlarme() {
    Serial.println("ligou,desliga");
    digitalWrite(PIN_ALARME, HIGH);
    delay(300);
    digitalWrite(PIN_ALARME, LOW);
}

