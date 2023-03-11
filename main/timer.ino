//Retrive current  datetime and return as string.
String getTime() {
  dt = clock.getDateTime();
  String someString="";
  someString = someString+ String(dt.day)+'/'+String(dt.month)+'/'+String(dt.year)+','+String(dt.hour)+':'+String(dt.minute)+':'+String(dt.second);
  return someString;
}
