/*
Date: 04/07/2023
Documentation: 23/01/2025
Author of the project: Ericsón Vilcahuamán, 
-Contact: ericksm.electronica@gmail.com

TFT driver ST7796 4.0inch  480x320 pixeles
pinout and connections mapping:
 PIN TFT  | DESCRIPTION  |  OTHER NAME    |  PIN ESP32
 1. VCC   |     5V       |  Power supply  |   
 2. GND   |    tierra    |                |   GND
 3. CS    | chip select  |       SS       |   GPIO5
 4. RESET |    reset     |                |   GPIO21
 5. DC/RS | data command |                |   GPIO22
 6. SDI   |   spi in     |    MOSI,SDO    |   GPIO23
 7. SCK   |  spi clock   |      CLK       |   GPIO18
 8. LED   |  backlight   |       BL       |   3.3V/GPIO4
 9. SDO   |   spi out    |    MISO,SDI    |   GPIO19
*/

#include <Arduino.h>
#include <TFT.h>
#include <GUI.h>
#include "string.h"
#include <SPI.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"


#define DEBOUNCE_TIME 130

AsyncWebServer server(80);

const char* ssid = "*******";
const char* password = "************";

const char* PARAM_MESSAGE = "mensaje";

char paginaweb[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Esp32 Server</title>
    <style>
      #boton1{
        background: #34a6c9;
        height: 50px;
        width: 84px;
        position: relative;
        border: 10px;
        border-radius: 10px;
        padding: 10px;
        margin: 5px;
        font-size: 16px;
        color: rgb(253, 247, 247);
        outline: none;
        overflow: hidden;
        cursor: pointer;
      }
      #boton1:hover
      {
        background-color: #130468; /* Green */
        color: white;
      }
      #boton1:disabled
      {
        background: #a7a6a7;
        cursor: default;
        box-shadow: inset 3px 3px 10px 0px rgba(0,0,0,0.2);
      }
    </style>
  </head>
  <body>
    <h1>Servidor Web con ESP32</h1>
    <form onsubmit="return myfun(this);" method="post">
      <input type = "texto" id ="mensaje" name = "mensaje" minlength="3" maxlength="25" placeholder="ingrese un mensaje" required>
      <br><br>
      <input type="submit" id = "boton1" value = "enviar"  disabled >
      <p id="demo"></p>
      <script>
        var nIntervId;
        var aux_data = localStorage.getItem("someVarKey");
        var number_data = Number(aux_data);
        function myfun() {
          var someVarName = "60";
          number_data = Number(someVarName);
          localStorage.setItem("someVarKey", someVarName);
          document.getElementById("boton1").disabled = true;
          clearInterval(nIntervId);
          nIntervId = setInterval(flashText,1000);
          return true;
        }
        // Set the date we're counting down to

        // Update the count down every 1 second
        nIntervId = setInterval(flashText,1000);
      
        function flashText() {
          if (number_data < 0) 
          {
            document.getElementById("boton1").disabled = false;
          }
          if(number_data>=0)
          {
            // Find the distance between now and the count down date
            number_data = number_data - 1;
            localStorage.setItem("someVarKey", number_data+ " ");
            // Time calculations for days, hours, minutes and seconds

            // Display the result in the element with id="demo"
            document.getElementById("demo").innerHTML = "espere "+number_data + " segundos <br> para volver a enviar";

            // If the count down is finished, write some text
            if (number_data < 0) {
            document.getElementById("demo").innerHTML = " ";
            document.getElementById("boton1").disabled = false;
            clearInterval(nIntervId);
          }
        }
      }
      </script>
    </form>
  </body>
</html>

)=====";

SPIClass * hspi = NULL;
uint8_t mensaje_buffer[30][30];//={"1","2","3","4 test","Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
unsigned long time_current=0;
uint8_t flag_new_data = 0;
String message;
char * message_aux;
int str_lengt= 0;
uint8_t count_message = 0;

void copy_array(String data1, uint8_t* data2)
{
  uint8_t i = 0;
  for(i = 0; i < 28; i++)
  {
    data2[i] = data1[i];
  }

}
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void setup() {
  hspi = new SPIClass(HSPI);
  hspi->begin(SCK, MISO, MOSI, SS);
  pinMode(SS,OUTPUT);
  hspi->beginTransaction(SPISettings(8000000,MSBFIRST,SPI_MODE0));
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(16, INPUT_PULLUP);
  Serial.printf("init mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html",paginaweb);////////// Send the page design
  });

  // Send a POST request with user message
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    if (request->hasParam(PARAM_MESSAGE, true)) 
    {
      message = request->getParam(PARAM_MESSAGE, true)->value();
      copy_array(message,mensaje_buffer[count_message]);
      count_message++;
      if(count_message==30)count_message=0;
      flag_new_data = 1;
    } 
    else 
    {
        message = "No message sent";
    }
    request->send_P(200, "text/html",paginaweb);
    Serial.print(message);
    Serial.println();
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() 
{
  uint8_t i = 0;
  int currentState;
  uint16_t sizeoff_y =15;
  uint8_t page_count =0;
  uint8_t contador_aux = 0;
  uint8_t aux_count_message = 0;
  uint8_t flag_push_button = 0;
  delay(200);
  LCD_Init();
  delay(100);
  
  //test TFT
  LCD_Clear(RED);
  delay(400);
  LCD_Clear(BLUE);
  delay(400);
  LCD_Clear(BLACK);
  digitalWrite(2,HIGH);
  delay(400);
  digitalWrite(2,LOW);
  LCD_ShowChar(2,3,YELLOW,BLACK,'1',1);
  delay(400);
  
  //We can receive a maximum of 27 characters, but I limit it to 25 characters.
  while(1)
  {
    if(flag_new_data)
    {
      flag_new_data = 0;
      if(count_message==0)aux_count_message=29;
      else aux_count_message = count_message-1;

      sizeoff_y = 15;
      if(aux_count_message>=10*page_count)contador_aux = aux_count_message-10*page_count;
      else contador_aux = aux_count_message+20;
      for(i = 0; i<10;i++)
      {
        print_str4_v2(10,sizeoff_y,WHITE,BLACK,mensaje_buffer[contador_aux],27);
        if(contador_aux>0)contador_aux--;
        else
        {
          if(mensaje_buffer[29][0]!=0)
          {
            contador_aux = 29;
          }
          else break;
        }
        sizeoff_y+=30;
      }
    }
    currentState = digitalRead(16);
    if(currentState == LOW && flag_push_button==0)
    {
      time_current = millis();
      flag_push_button = 1;
      
    }
    if(flag_push_button)
    {
      if(millis()-time_current>DEBOUNCE_TIME)
      {
        flag_push_button = 0;
        if(digitalRead(16)==LOW)
        {
          page_count++;//3 pages
          if(page_count==3)page_count=0;
          
          contador_aux = page_count*10;
          if(mensaje_buffer[contador_aux][0]==0)page_count = 0;
       
          Serial.println((const char*)mensaje_buffer[contador_aux]);
          LCD_Clear(BLACK);   
          LCD_ShowChar(2,3,YELLOW,BLACK,page_count+0x31,1); //show number page
          sizeoff_y = 15;
          if(aux_count_message>=10*page_count)contador_aux = aux_count_message-10*page_count;
          else contador_aux = aux_count_message+20;
          for (i=0;i<10;i++)
          {         
            print_str4_v2(10,sizeoff_y,WHITE,BLACK,mensaje_buffer[contador_aux],27);
            if(contador_aux>0)contador_aux--;
            else
            {
              if(mensaje_buffer[29][0]!=0)
              {
                contador_aux = 29;
              }
              else break;
            }
            sizeoff_y+=30;
          }
        }
      }
    }
  }
}
