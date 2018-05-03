# To-do options
## I. Bridge

Propojení více různých zařízení a služeb skrze jeden server.

#### Actual options

##### 1. Services

*Apple Homekit*

- https://www.apple.com/ios/home/
- nabízí propojení mnoha zařízení 
- intuitvní a jednoduché ovládání přímo z iPhonu nebo jiného Apple zařízení
- vymezené jen pro uživatele Applu

*IFTTT*

- https://ifttt.com/
- propojuje vícero různých produktů i služeb (včetně např. kalendáře, tweeteru, ...) a umožnuje uživateli automatizaci

*Google Assistant*

- https://assistant.google.com/#?modal_active=none
- ovládání prvků chytré domácnosti příkazy
- příkazy lze užívat i pro jiné aplikace i třetích stran

##### 2. Products

*GeekLink*

- http://en.geeklink.com.cn/
- nabízí propojení různých ne smart zařízení přes IR
- ovládání přes vlastní aplikaci

## II. Device
Naším unikátem je zachování designu jak vypínače, tak světla.

### Posible physical positions

| place           | chip     | out | in  | power source | problem    |
| --------------- | -------- | --- | --- | :----------: | ---------- |
| za vypínačem    | ESP32    | ano | ano | el. síť      | chybí nula |
|                 | jednočip | ne  | ano | baterka      |            |
| u světla        | ESP32    | ano | ne  | el. síť      |            |
| mimo dosah WiFi | jednočip | ano | ano | el. síť      |
|                 | jednočip | ne  | ano | baterka      |

##### Legenda
out - spínání světla/světelného obvodu  
in - senzor pro spínač

#### Chalanges
- zajištění bezpečnosti komunikace jednočipů po radiové síti
- zajištění dlouhodobé výdrže u jednočipu

#### Actual options

*Philips Hue*

- https://www2.meethue.com/en-us
- hlavně osvětlení v podobně žárovek nebo samotných svítívých zařízení (led pásek, lampa, ...)
- nabízí i vypínač na baterky
- vše je propojedo do severu, který je připojený na síť
- ovládání přes aplikaci (+ podpora HomeKitu i IFTTT)

*Dalen*

- https://dalen-tech.cz/cs/
- nabízí lustry s nastavitelnou barvou
- ovládání ovladačem

*IKEA TRADFRI*

- https://www.ikea.com/cz/cs/catalog/categories/departments/lighting/smart_lighting/
- žárovka + ovládací prvek
- možnou dokoupit most