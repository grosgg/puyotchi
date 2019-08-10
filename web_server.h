void sendWebPage() {
  Serial.println("sendWebPage");
  File webPage = SPIFFS.open("/web_page.html", "r");
  server.streamFile(webPage, "text/html");
  webPage.close();
}

void setupWebServer() {
  server.on("/", HTTP_GET, sendWebPage);
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}
