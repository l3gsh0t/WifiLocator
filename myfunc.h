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
String encryptType(uint8_t type) {
    switch(type) {
        case ENC_TYPE_NONE:
            return "None";
        case ENC_TYPE_WEP:
            return "WEP";
        case ENC_TYPE_TKIP:
            return "WPA";
        case ENC_TYPE_CCMP:
            return "WPA2";
        case ENC_TYPE_AUTO:
            return "AUTO";
        default:
            return "Unknown";
    }
}
