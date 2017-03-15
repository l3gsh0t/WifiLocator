/*********************************************************************
Author: Matthew Waddell
Use: ESP SSID Fun
*********************************************************************/

//==============================================================================================
int * iterateNetworks(int networkCount) {
  static int pos[6];

  for (int k = 0; k < 6; k++) { pos[k] = -1; }
  
  if (networkCount > 0) {
    for (int j = 0; j < 6; j++) {
      for (int i = 0; i < networkCount; i++) {
        if (i != pos[0] && i != pos[1] && i != pos[2] && i != pos[3] && i != pos[4]) {
          if (pos[j] == -1 || WiFi.RSSI(pos[j]) < WiFi.RSSI(i)) {
            pos[j] = i;
          }
        }
      }
    }
  }
  return pos;
}

//==============================================================================================
void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}
