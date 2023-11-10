# Iserv-Captive-Portal

Ein Beispiel, wie man ein Captive Portal von Iserv vortäuschen kann um die Login Daten von Usern zu phishen.

Benötigte Hardware:
1. ESP32
2. SSD1306 OLED Display zur Anzeige (optional)

Software seits muss zum kompilieren VS Code mit Platformio als addon installiert werden und dann der Ordner in dieser Repo als Projekt geöffnet werden. Anschließend muss die Task "Upload Filesystem Image" ausgeführt werden und danach muss in der unteren Leiste nur auf Upload gedrückt werden. Danach entsteht ein WLAN Netz mit dem Namen "FHG-FreeWifi" und alle eingegebenen Login-Daten werden sowohl über den Serial Port als auch über das OLED Display angezeigt. Desweiteren wäre es Trivial diese Zugangsdaten im Dateisystem zu speichern und diese anschließend wieder auszulesen, was aber für Demonstrationszwecke nicht von nöten ist. Nach der erfolgreichen Anmeldung erhält der User die Nachricht, dass ein Fehler unterlaufen sei und er sich doch bitte erneut mit dem WLAN verbinden solle und nochmals auf gut Glück sein Passwort eingeben soll.





https://github.com/MertX2/Iserv-Captive-Portal/assets/94470734/8e7b324f-816d-4a05-8474-c17e2e730f73

![IMG_20231110_175047](https://github.com/MertX2/Iserv-Captive-Portal/assets/94470734/9eee3c5a-cddc-4dbc-b8a2-dea8880dcf9f)
