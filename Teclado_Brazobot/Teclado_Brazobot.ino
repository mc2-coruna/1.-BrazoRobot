


/* Teclado para las aplicaciones de la exposición 
La Astronomía Como Nos la Cuentan.
----------------------------------------    
Pines para modulo CAMARA TERMOGRAFICA
    botón cambio imagen --> 4
----------------------------------------    
Pines para módulo BRAZOROBOT
    joystick delante 3
             atras   4
             izqda   5
             drcha   6

    boton subir      7
          bajar      8

    ajuste fino      9
           normal   10

    parada          11
----------------------------------------*/

// Tiempo minimo entre dos pulsaciones si se maniene el boton pulsado
int tiempoRepeticion = 500;

// Terminales de entrada de los botones
#define joystick_delante 3
#define joystick_atras   4
#define joystick_izqda   5
#define joystick_drcha   6

#define boton_subir      7
#define boton_bajar      8

#define ajuste_fino      9
#define ajuste_normal   10

#define boton_parada    11


// Variables auxiliares
byte botonPulsado = 0;

void setup() {
  // Configura las entradas y salidas
  pinMode (LED_BUILTIN, OUTPUT);
  
  pinMode (joystick_delante, INPUT_PULLUP);
  pinMode (joystick_atras,   INPUT_PULLUP);
  pinMode (joystick_izqda,   INPUT_PULLUP);
  pinMode (joystick_drcha,   INPUT_PULLUP);
  pinMode (boton_subir,      INPUT_PULLUP);
  pinMode (boton_bajar,      INPUT_PULLUP);
  pinMode (ajuste_fino,      INPUT_PULLUP);
  pinMode (ajuste_normal,    INPUT_PULLUP);
  pinMode (boton_parada,     INPUT_PULLUP);

}

void loop() {
  //Comprueba si hay alguna tecla pulsada
  if      (!digitalRead (joystick_delante)) botonPulsado = 65;  // a
  else if (!digitalRead (joystick_atras  )) botonPulsado = 66;  // b
  else if (!digitalRead (joystick_izqda  )) botonPulsado = 67;  // c
  else if (!digitalRead (joystick_drcha  )) botonPulsado = 68;  // d

  else if (!digitalRead (boton_subir   )) botonPulsado = 69;  // e
  else if (!digitalRead (boton_bajar   )) botonPulsado = 70;  // f

  else if (!digitalRead (ajuste_fino   )) botonPulsado = 71;  // g
  else if (!digitalRead (ajuste_normal )) botonPulsado = 72;  // h

  else if (!digitalRead (boton_parada  )) botonPulsado = 74;  // i

  else if (!digitalRead (botonPulsado  )) botonPulsado = 75;  // j
  
  // Si detecta una pulsación la envía y espera antes de volver a leer el teclado
  if (botonPulsado > 0){
      digitalWrite (LED_BUILTIN, HIGH);
      Keyboard.press (botonPulsado);
      
      Serial.println(botonPulsado);
      delay (10);
      Keyboard.releaseAll ();
      delay (140);
      digitalWrite (LED_BUILTIN, LOW);
      delay (tiempoRepeticion);
      botonPulsado = 0;
  }  
}
