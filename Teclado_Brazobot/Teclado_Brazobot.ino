


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
int tiempoRepeticion = 250;

// Sería bueno definir una opción de no repetición

//
//


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
  
  Keyboard.begin();

}

void loop() {
  //Comprueba si hay alguna tecla pulsada
  botonPulsado = 0;
  
  if      (!digitalRead (joystick_delante)) botonPulsado = 65;  // A
  else if (!digitalRead (joystick_atras  )) botonPulsado = 66;  // B
  else if (!digitalRead (joystick_izqda  )) botonPulsado = 67;  // C
  else if (!digitalRead (joystick_drcha  )) botonPulsado = 68;  // D

  else if (!digitalRead (boton_subir     )) botonPulsado = 69;  // E
  else if (!digitalRead (boton_bajar     )) botonPulsado = 70;  // F

  else if (!digitalRead (ajuste_fino     )) botonPulsado = 71;  // G
  else if (!digitalRead (ajuste_normal   )) botonPulsado = 72;  // H

  else if (!digitalRead (boton_parada    )) botonPulsado = 73;  // I

  
  // Si detecta una pulsación la envía y espera antes de volver a leer el teclado
  if (botonPulsado >= 65){
      digitalWrite (LED_BUILTIN, HIGH);
      Keyboard.press (botonPulsado);
      
      Serial.println(botonPulsado);
      delay (5);
      Keyboard.releaseAll ();
      delay (100);
      digitalWrite (LED_BUILTIN, LOW);
      delay (tiempoRepeticion);
      botonPulsado = 0;
  }  
}
