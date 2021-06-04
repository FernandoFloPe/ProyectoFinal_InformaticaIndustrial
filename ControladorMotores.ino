//Proyeto Final
//Control Digital de un Sistema
//Informatica Industrial
//Fernando Flores Pérez

#define PWM 3 //Pin digital del PWM

// Variables medidas analogicamente
float Ref; //Referencia
float s_n; //Lectura del sensor
float medicionPWM; //Lectura del PWM

// Variables calculadas digitalmente
float y_n; //Salida del controlador presente
float y_n_PWM;
float e_n; //Error presente
float e_n_1; //Error de la muestra pasada
float e_n_2;
float y_n_1; //Salida del controlador de la muestra pasada

// Constantes obtenidas del sistema
float a0;
float a1;
float a2;
float b0;


void setup() {
  pinMode(PWM,OUTPUT); //Configura el pin del PWM como salida
  Serial.begin(9600); //Inicializa la comunicacion serial

  // Inicializacion de datos para el control discreto
  e_n_1=0;
  e_n_2=0;
  y_n_1=0;

  // Constantes del sistema obtenidas con la discretizacion en matlab
  a0=-0.125576;
  a1=0.13687;
  a2=0;
  b0=1;
}

void loop() {
  medicionPWM=analogRead(A2);

  Ref=analogRead(A0); //Puerto A0 es la referencia
  s_n=analogRead(A1); //Puerto A1 es el sistema
  e_n=Ref-s_n; //Calcula el error
  y_n=a0*e_n+a1*e_n_1+a2*e_n_2+b0*y_n_1; //Formula general para los controladores

  y_n_PWM=y_n;
  
  // Limites para y(n)
  if(y_n>=1023){ //Condicion que limita el PWM para no quemar la tarjeta
      y_n=1023; //Limita el valor maximo de y_n para evitar esperar
  }

  // Limites para el PWM
  if(y_n_PWM>=0 && y_n_PWM<=255){
    analogWrite(PWM,y_n_PWM); //Genera el PWM
  }
  else if(y_n_PWM<0){
    analogWrite(PWM,0);
  }
  else{
    analogWrite(PWM,255);
  }

  // Actualizacion de datos
  e_n_2=e_n_1;
  e_n_1=e_n;
  y_n_1=y_n;

  // Graficaras
  Serial.println(Ref); //Grafica la señal de la referencia
  Serial.println(s_n); //Grafica la señal del sensor
  Serial.println(medicionPWM); //Grafica la señal del PWM

  delay(100);  // Periodo de muestro en milisegundos
}
