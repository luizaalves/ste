int led = 13;
int sensorPin = A0;
float sensorValue,temperatura;
float a,b,c;

void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  digitalWrite(led,LOW);

  sensorValue = 0;
  temperatura = 0;
  a = 1.112272865e-3;
  b = 2.375809102e-4;
  c = 0.6852858650e-7;
}

void loop() {
  led_on(led);
  le_temp(sensorPin);
  led_off(led);
  imprime_res();
  delay(1000);
}

void led_on(int pin){
  digitalWrite(led,HIGH);
}

void led_off(int pin){
  digitalWrite(led,LOW);
}

void le_temp(int pin){
  sensorValue = analogRead(pin);
  calculo_temp();
}

void imprime_res(){
  Serial.print("temperatura(°C): ");
  Serial.println(temperatura);
}

void calculo_temp(){
  float vout = sensorValue;
  float val_normalizado = 5* vout /1023;
  float r = 10e3*((5/val_normalizado)-1);
  float pot = pow(log(r),3);
  float temp_kelvin = 1/(a+(b*log(r))+(c*pot));
  temperatura = temp_kelvin - 273.15;
}
