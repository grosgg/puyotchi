String getPage() {
  String page = "";
  page += "HTTP/1.1 200 OK\r\n";
  page += "Content-Type: text/html\r\n\r\n";
  page += "<!DOCTYPE HTML>";
  page += "<html>";
  page += "  <head>";
  page += "    <meta charset='UTF-8'>";
  page += "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "    <link href='https://fonts.googleapis.com/css?family=Press+Start+2P' rel='stylesheet'>";
  page += "    <link href='https://unpkg.com/nes.css/css/nes.min.css' rel='stylesheet'>";
  page += "    <style>";
  page += "      body { margin: 5px; }";
  page += "    </style>";
  page += "  </head>";
  page += "  <body>";
  page += "    <header>";
  page += "      <h1>Puyotchi</h1>";
  page += "    </header>";
  page += "    <section class='nes-container with-title'>";
  page += "      <h2 class='title'>Actions</h2>";
  page += "      <div>";
  page += "        <a href='/roll'><button type='button' class='nes-btn is-success'>Roll</button></a>";
  page += "      </div>";
  page += "    </section>";
  page += "  </body>";
  page += "</html>";

  return page;
}
