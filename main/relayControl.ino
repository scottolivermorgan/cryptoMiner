//Functions to switch two between relays.
void solar(){
  digitalWrite(sourceB, LOW);
  delay(1500);
  digitalWrite(sourceA, HIGH); 
}

void mains(){
  digitalWrite(sourceA, LOW);
  delay(1500);
  digitalWrite(sourceB, HIGH); 
}
