#include <SPI.h>
#include <Ethernet.h>
#include <DS1307.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 0, 0, 2); 
EthernetServer server(80);
EthernetClient client;
String HTTP_req;

DS1307 rtc(A4, A5);

String horarioAmanhecer = "00:00";
String duracaoDia = "00:00";
String duracaoIrrigacao = "00:00";
String intervaloIrrigacao = "00:00";

void setup()
{
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.begin(9600);
    //rtc.halt(false);
    //rtc.setTime(05,04,00);
}

void loop()
{
    client = server.available();
    
    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) 
        {
            if (client.available()) 
            { 
                char c = client.read();
                HTTP_req += c;
                
                if (c == '\n' && currentLineIsBlank) 
                {
                    //Envia um header http padrão.
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();

                    if(HTTP_req.indexOf("hora_amanhecer") > -1)
                    {
                        getHorarioAmanhecer(client);
                    }
                    else if(HTTP_req.indexOf("duracao_dia") > -1)
                    {
                        getDuracaoDia(client);
                    }
                    else if(HTTP_req.indexOf("duracao_irrigacao") > -1)
                    {
                        getDuracaoIrrigacao(client);
                    }
                    else if(HTTP_req.indexOf("intervalo_irrigacao") > -1)
                    {
                        getIntervaloIrrigacao(client);
                    }
                    else if(HTTP_req.indexOf("campo1") > -1)
                    {
                        setHorarioAmanhecer(client, HTTP_req.substring(12,17));
                    }
                    else if(HTTP_req.indexOf("campo2") > -1)
                    {
                        setDuracaoDia(client, HTTP_req.substring(12,17));
                    }
                    else if(HTTP_req.indexOf("campo3") > -1)
                    {
                        setDuracaoIrrigacao(client, HTTP_req.substring(12,17));
                    }
                    else if(HTTP_req.indexOf("campo4") > -1)
                    {
                        setIntervaloIrrigacao(client, HTTP_req.substring(12,17));
                    }
                    else 
                    {
                        sendBrowserApplication();
                    }
                    Serial.print(HTTP_req);
                    HTTP_req = "";
                    break;
                }

                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        
        delay(1);
        client.stop();
    }
    Serial.println(rtc.getTimeStr());
    delay(100);
}

void getHorarioAmanhecer(EthernetClient cl)
{
    cl.println(horarioAmanhecer);

}
void getDuracaoDia(EthernetClient cl)
{
    cl.println(duracaoDia);

}
void getDuracaoIrrigacao(EthernetClient cl)
{
    cl.println(duracaoIrrigacao);

}
void getIntervaloIrrigacao(EthernetClient cl)
{
    cl.println(intervaloIrrigacao);
}

void setHorarioAmanhecer(EthernetClient cl, String HorarioAmanhecer)
{
    horarioAmanhecer = HorarioAmanhecer.substring(0,5);
    cl.println(horarioAmanhecer);
}
void setDuracaoDia(EthernetClient cl, String DuracaoDia)
{
    duracaoDia = DuracaoDia.substring(0,5);
    cl.println(duracaoDia);
}
void setDuracaoIrrigacao(EthernetClient cl, String DuracaoIrrigacao)
{
    duracaoIrrigacao = DuracaoIrrigacao.substring(0,5);
    cl.println(duracaoIrrigacao);
}
void setIntervaloIrrigacao(EthernetClient cl, String IntervaloIrrigacao)
{
    intervaloIrrigacao = IntervaloIrrigacao.substring(0,5);
    cl.println(intervaloIrrigacao);
}

void sendBrowserApplication()
{
    client.println("<head>");
    client.println("<meta charset=\"utf-8\">");
    client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<title>Sistema de Controle</title>");
    client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">");                       
    
    client.println("<script type=\"text/javascript\">");
    client.println("function startTime(){var today=new Date(); var h=today.getHours(); var m=today.getMinutes(); var s=today.getSeconds(); h=checkTime(h); m=checkTime(m); s=checkTime(s); document.getElementById('time').innerHTML=h+\":\"+m+\":\"+s; var t=setTimeout(function(){startTime()},500);}function checkTime(i){if (i<10){i=\"0\" + i}; return i;}");
    client.println("function getHoraAmanhecer() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('horaAmanhecer').value=this.responseText;}}");
    client.println("  request.open(\"GET\", \"hora_amanhecer\");");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function getDuracaoDia() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('duracaoDia').value=this.responseText;}}");
    client.println("  request.open(\"GET\", \"duracao_dia\");");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function getDuracaoIrrigacao() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('duracaoIrrigacao').value=this.responseText;}}");
    client.println("  request.open(\"GET\", \"duracao_irrigacao\");");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function getIntervaloIrrigacao() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('intervaloIrrigacao').value=this.responseText;}}");
    client.println("  request.open(\"GET\", \"intervalo_irrigacao\");");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function getAllData() {");
    client.println("  startTime();");
    client.println("  getHoraAmanhecer();");
    client.println("  getDuracaoDia();");
    client.println("  getDuracaoIrrigacao();");
    client.println("  getIntervaloIrrigacao();");
    client.println("}");
    
    client.println("function setHoraAmanhecer() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('horaAmanhecer').value=this.responseText;}}");
    client.println("  request.open(\"GET\",\"campo1=\"+document.getElementById('horaAmanhecer').value);");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function setDuracaoDia() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('duracaoDia').value=this.responseText;}}");
    client.println("  request.open(\"GET\",\"campo2=\"+document.getElementById('duracaoDia').value);");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function setDuracaoIrrigacao() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('duracaoIrrigacao').value=this.responseText;}}");
    client.println("  request.open(\"GET\",\"campo3=\"+document.getElementById('duracaoIrrigacao').value);");
    client.println("  request.send(null);");
    client.println("}");
    client.println("function setIntervaloIrrigacao() {");
    client.println("  nocache = \"&nocache=\"+ Math.random() * 1000000;");
    client.println("  var request = new XMLHttpRequest();");
    client.println("  request.onreadystatechange = function () {");
    client.println("  if (request.readyState==4){document.getElementById('intervaloIrrigacao').value=this.responseText;}}");
    client.println("  request.open(\"GET\",\"campo4=\"+document.getElementById('intervaloIrrigacao').value);");
    client.println("  request.send(null);");
    client.println("}");                        
    client.println("function setAllData() {");
    client.println("  setHoraAmanhecer();");
    client.println("  setDuracaoDia();");
    client.println("  setDuracaoIrrigacao();");
    client.println("  setIntervaloIrrigacao();");
    client.println("}");
    client.println("</script>");
    client.println("</head>");
    //Head-----------------------------------------------------------------------------------------------------------------------
    
    //Body-----------------------------------------------------------------------------------------------------------------------
    client.println("<body onload=\"getAllData()\">");
    client.println("    <div class=\"container\">");
    client.println("        <div class=\"panel panel-default\">                                                               ");
    client.println("            <div class=\"panel-heading text-center\">                                                     ");
    client.println("                <h3>SISTEMA EMBARCADO DE CONTROLE</h3><h3 id=\"time\"></h3>                               ");
    client.println("            </div>                                                                                        ");
    client.println("            <div class=\"panel-body\">                                                                    ");
    client.println("                <form>                                                                                    ");
    client.println("                    <div class=\"row\" style=\"margin-bottom: 10px\">                                     ");
    client.println("                        <div class=\"col-md-3 col-md-offset-3\">                                          ");
    client.println("                            <label>Hora do Amanhecer:</label>                                             ");
    client.println("                            <label><small>(00:00 até 23:59)</small></label>                               ");
    client.println("                        </div>                                                                            ");
    client.println("                        <div class=\"col-md-3\">                                                          ");
    client.println("                            <input class=\"form-control\" type=\"text\" id=\"horaAmanhecer\">             ");
    client.println("                        </div>                                                                            ");
    client.println("                    </div>                                                                                ");
    client.println("                    <div class=\"row\" style=\"margin-bottom: 10px\">                                     ");
    client.println("                        <div class=\"col-md-3 col-md-offset-3\">                                          ");
    client.println("                            <label>Duração do Dia:</label>                                                ");
    client.println("                            <label><small>(00:00 até 23:59)</small></label>                               ");
    client.println("                        </div>                                                                            ");
    client.println("                        <div class=\"col-md-3\">                                                          ");
    client.println("                            <input class=\"form-control\" type=\"text\" id=\"duracaoDia\">                ");
    client.println("                        </div>                                                                            ");
    client.println("                    </div>                                                                                ");
    client.println("                    <div class=\"row\" style=\"margin-bottom: 10px\">                                     ");
    client.println("                        <div class=\"col-md-3 col-md-offset-3\">                                          ");
    client.println("                            <label>Duração da Irrigação:</label>                                          ");
    client.println("                            <label><small>(00:00 até 23:59)</small></label>                               ");
    client.println("                        </div>                                                                            ");
    client.println("                        <div class=\"col-md-3\">                                                          ");
    client.println("                            <input class=\"form-control\" type=\"text\" id=\"duracaoIrrigacao\">          ");
    client.println("                        </div>                                                                            ");
    client.println("                    </div>                                                                                ");
    client.println("                    <div class=\"row\" style=\"margin-bottom: 10px\">                                     ");
    client.println("                        <div class=\"col-md-3 col-md-offset-3\">                                          ");
    client.println("                            <label>Intervalo de Irrigação:</label>                                        ");
    client.println("                            <label><small>(00:00 até 23:59)</small></label>                               ");
    client.println("                        </div>                                                                            ");
    client.println("                        <div class=\"col-md-3\">                                                          ");
    client.println("                            <input class=\"form-control\" type=\"text\" id=\"intervaloIrrigacao\">        ");
    client.println("                        </div>                                                                            ");
    client.println("                    </div>                                                                                ");
    client.println("                    <div class=\"row\" style=\"margin-bottom: 10px\">                                     ");
    client.println("                        <div class=\"col-md-6 col-md-offset-3\">                                          ");
    client.println("                            <input class=\"btn btn-success btn-block\"type=\"button\"value=\"Salvar\"onclick=\"setAllData()\">");
    client.println("                        </div>                                                                            ");
    client.println("                    </div>                                                                                ");
    client.println("                </form>                                                                                   ");
    client.println("            </div>                                                                                        ");
    client.println("        </div>                                                                                            ");
    client.println("    </div>");
    client.println("</body>");
    //Body-----------------------------------------------------------------------------------------------------------------------                        
}