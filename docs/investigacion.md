# INVESTIGACIONES HISPASAT/ABERTIS

Redactado en Google Docs. Exportado a 

Por Juan Pedro ([@StackOverflow67@masto.es](https://masto.es/@StackOverflow67))
Códigos disponibles en GitHub (Mismo @)

[**Prefacio	2**](#prefacio)

[**Abertis (Formato T2-MI propietario y desempaquetado)	3**](#abertis-\(formato-t2-mi-y-desempaquetado\))

[Apertura de Abertis (Pasos)	3](#apertura-de-abertis-\(pasos\))

[**Comprobaciones rápidas	5**](#comprobaciones-rápidas)

[Verificados localmente (Hardware DVB-S2 estándar)	5](#verificados-localmente-\(hardware-dvb-s2-estándar\))

[Pipeline de pruebas locales	7](#pipeline-de-pruebas-locales)

[EJEMPLOS DE USO	7](#ejemplos-de-uso)

[Vision en vivo de los canales	7](#vision-en-vivo-de-los-canales)

[Grabación	7](#grabación)

[**Listas de canales y transpondedores (Verificados)	9**](#listas-de-canales-y-transpondedores-\(verificados\))

[Feed \- Demux \- TS interno (Abierto)	9](#feed---demux---ts-interno-\(abierto\))

[Anotaciones	15](#anotaciones)

[**Sin comprobar/Fallidos	16**](#sin-comprobar/fallidos)

# Prefacio {#prefacio}

Por motivos legales, se han redactado las claves BISS relacionadas.

La mayoría de muxes de Abertis, contrario a lo que dice en internet, son transpondedores normales y corrientes DVB-S2 sin nada elegante, había asumido que mi antigua tarjeta (Hauppauge WinTV HVR 4000\) no me serviría para ver TDTSAT. Asumí que necesitaba una tarjeta con DVB-S2X, PLS, MultiStream, etc, había comprado un Dinobot 4K TWIN (HiSilicon, save allts, /proc/msp, etc. Ver abajo). ESA ASUNCIÓN ERA INCORRECTA. En Hispasat 30°W los carriers de la señal son DVB-S2, 8PSK, 10-30 MSym/s, FEC 3/4, la Hauppauge WinTV-HVR-4000 que tengo instalada en mi ordenador (Conexant CX24116 DVB-S2, /dev/dvb/adapter0/frontend0) es perfectamente capaz de bloquear en casi todos los transpondedores. Sin SSH, sin Enigma2, sin save allts, sin /proc/msp: haciendo el tuning con sat-zap and read the full transport stream from /dev/dvb/adapter0/dvr0.  
En el Dinobot, había un offset del LNB de unos 3 megahercios hacia arriba, para enganchar 11222H, necesitaba apuntar a 11225H. Con esta tarjeta supongo que se corrige automáticamente el offset, pues puedo enganchar directamente al IF 1472H (11222H).

Toda esta documentación asume que tienes permiso explicito para ver contenidos proporcionados por Abertis (Asumiendo que has comprado un deco oficial tras declarar zona sin cobertura y has hecho reverse-engineering de las claves por tu propia cuenta). La TDT es gratis. No la piratees.

Abertis, Cellnex y los nombres de los canales de TDT son propiedad intelectual de sus respectivos dueños.

Este documento esta redactado con fines investigativos.

**Esta guía está DESACTUALIZADA, un día antes de la actualización/inicio de transición a DVB-T2 en España. El documento se redactó durante los días 3 y 4 de noviembre de 2026\. Ha habido cambios grandes en muchos muxes/TPs.**

**En algún momento actualizaré la información desfasada.**

# Abertis (Formato T2-MI y desempaquetado) {#abertis-(formato-t2-mi-y-desempaquetado)}

Un PID de distribución de Abertis lleva una trama DVB completa dentro de los paquetes de 184 bytes externos, con enmascaramiento DVB a nivel de transporte (sin sistema de acceso condicional).  
Estos paquetes se rellenan con mucho stuffing por parte del operador (Abertis), pero la herramienta descarta estos paquetes.  
Hay dos capas en un paquete Abertis estándar, tal que;

- **Capa Externa** (Transpondedor Satelital): Es una portadora estándar DVB-S2 (con su frecuencia, tasa de símbolos y polarización). Dentro de este flujo general de satélite, Abertis reserva identificadores específicos (PIDs externos, por ejemplo, el 0x7E9). Cada PID lleva un mux completo en formato .TS  
- **Capa Interna** (TS Terrestre): Dentro de los paquetes de ese PID interno viaja vídeo y audio, un flujo DVB-TS entero e independiente (exactamente el mismo múltiplex que se emite por la antena terrestre de una región concreta, con sus propias tablas PAT/PMT, sus canales de TV, radios, teletexto, subtítulos, EPG, Hbb-TV...).

Los datos que viajan dentro del PID externos están revueltos mediante el algoritmo estándar de la industria DVB-CSA (Digital Video Broadcasting \- Common Scrambling Algorithm). Los PIDs internos (Video/Audio/...) **NO** están cifrados.  
Abertis protege estos PIDs externos con **claves BISS estáticas de 48 bits**.

## Apertura de Abertis (Pasos) {#apertura-de-abertis-(pasos)}

1. **Sintonización del Portador:** El sintonizador del satélite se fija a la frecuencia exacta (ej. 11222 MHz), polarización y tasa de símbolos para enganchar (*lock*) el transpondedor DVB-S2 de Abertis.  
2. **Filtrado del Contenedor (PID Externo):** De todo el tráfico que baja del satélite, el sistema descarta la morralla y aísla únicamente el PID externo que corresponde a la región o múltiplex que se quiere abrir (por ejemplo, el paquete de contenidos para Extremadura o Cataluña).  
3. **Descifrado Criptográfico (Des-scrambling):** A los paquetes de ese PID se les aplica el algoritmo DVB-CSA utilizando la clave BISS correspondiente a ese canal/frecuencia. Esto elimina el estado *scrambled* y deja los bytes internos en texto claro (aunque siguen empaquetados dentro de la estructura de Abertis).  
4. **Desencapsulado DVB**: Se retiran las cabeceras exteriores del satélite y se extrae el flujo de transporte terrestre puro. Como al trocear y meter ese flujo en el satélite se pierden las alineaciones de bytes, el sistema busca los patrones de sincronización estándar (el byte 0x47 (“G”) que marca el principio de absolutamente todos los paquetes de Transport Stream) para reconstruir el Transport Stream original.  
5. **Realineación de paquetes:** Si se encuentran errores de alineación, se deben corregir los paquetes, por ejemplo, estamos viendo “Servicio 1” en “Transpondedor 1”, si llega paquete \#1791, luego \#1793 y por último \#1792, debemos mantener una lista y unir los paquetes.

Tras este proceso, el resultado es un flujo de TDT (Over the Air Digital Televisión) convencional igual byte a byte que el que recibirías con un sintonizador DVB-T, cualquier programa que acepte este tipo de archivos entenderá el paquete abierto.  
Los paquetes que nos entran en T2-MI son (Generalmente, al menos los de Hispsat/Abertis) 188 bytes, con 184 de esos siendo el stream “real” encapsulado dentro.

# Comprobaciones rápidas {#comprobaciones-rápidas}

### Verificados localmente (Hardware DVB-S2 estándar) {#verificados-localmente-(hardware-dvb-s2-estándar)}

| Comprobación | Resultado |
| :---- | :---- |
| 11222 H (IF 1472 MHz, 18 V, 22 NO) | LOCK, TSID 0xEA74 EXTERNO |
| 12671 V (IF 2071 MHz, 13 V, 22 SI) | LOCK, TSID 0xEA61 EXTERNO (Mediaset/Atresmedia/RTVE Cataluña mux) |
| Captura 14s 11222 H | 608,000 paquetes / 114,304,000 B, 65.317 Mbps, 0 bad sync, 0 TEI, 0 CC gaps, 0.3 % null (Relleno hasta 70mbps) |
| sat-zap 11222H \> abiertis \--pid 0x7E9 | TSID 0x0455 INTERNO, La 1 HD 1920×1080 H.264 \+ E-AC-3,Sub DVB, Teletexto |
| 11222H (Los 6 feeds) | TSIDs 0x0455, 0x03F3, 0x03F5, 0x03F8, 0x041B, 0x0001; abren y decodificar |
| 12671V (Los 4 feeds) | TSIDs 0x0001, 0x0012, 0x0010, 0x03F0; ALERTA, Atresmedia, Mediaset y RTVE Cataluña. NOTA SOBRE ALERTA: “ALERTA” (Con “Provider” Cellnex) es un video en bucle que avisa sobre el cierre de los canales SD. Este video se puede encontrar en la página de [RTVE](https://www.rtve.es/television-alta-definicion/) ([Enlace directo](https://rtvemvodlote7.rtve.es/tdt2024/tdt_2024_hq.mp4)). |
| HEVC 0x80C | TSID 0x0001, HEVC 1920x540, AAC/AAC-LATM.Canales: La 1 HEVC, La 2 HEVC, 24H HEVC.Estos canales no se encuentran en ningún sitio que haya podido encontrar. Aparentemente son squished de 1920x1080. Además de eso, estos canales son 540**p**, no entrelazados. |
| Todos los transpondedores Abertis | **11 carriers enganchan, 38 feeds Abertis decodificados a TS claro. Más detalles y problemas se encuentran abajo.** |
| 12583 V 0x1F48 | La 1 UHD HEVC 3840×2160, Clan HD, DKISS \+ 8 radios |
| Captura 12s  12671V | 521,216 paquetes / 97,988,608 B, **65.326 Mbps**, 0 bad sync, 0 TEI, 0 CC gaps (Relleno hasta los 70mbps) |

### 

# Pipelines y explicaciones

### Pipeline de pruebas locales {#pipeline-de-pruebas-locales}

Se han usado unos programas (mezcla de propios con públicos) para el pipeline de lock \-\> captura \-\> deconv \-\> reproducción  
“sat-zap” es un pequeñísimo programa escrito en C que simplemente engancha la señal del satélite y espera a morir. Dejando abierto /dev/dvb/adapter0/dvr0  
“abiertis” es un extractor de paquetes Abertis. (+ dec DVB-CSA, ahorrando la necesidad de “OSCAM” o otros emuladores de CAM)

sat-zap no escribe el TS a ningún archivo, envía los comando de sintonización al frontend y prepara el demux para el TS completo (PID 0x2000).

#### EJEMPLOS DE USO {#ejemplos-de-uso}

##### Vision en vivo de los canales {#vision-en-vivo-de-los-canales}

(term 1\)  
sat-zap \-f 11222000 \-l 9750000 \-s 11700000 \-P H \-r 30000000 \-e 3/4 \-S  
(MANTENER VIVO)  
(term 2\)  
abiertis  \--pid 0x80c \--key (CLAVE REDACTADA DEL DOCUMENTO ORIGINAL) \< /dev/dvb/adapter0/dvr0 \| vlc \-

##### Grabación {#grabación}

(term 1\)  
sat-zap \-f 11222000 \-l 9750000 \-s 11700000 \-P H \-r 30000000 \-e 3/4 \-S  
(MANTENER VIVO)  
(term 2\)  
abiertis  \--pid 0x80c \--key (CLAVE REDACTADA DEL DOCUMENTO ORIGINAL) \< /dev/dvb/adapter0/dvr0 \> grabacion\_hevc.ts

Después puedes usar el stream normalmente

ffprobe grabacion\_hevc.ts  
vlc grabacion\_hevc.ts  
ffmpeg \-i grabacion\_hevc.ts \-map 0:p:1 \-f mpegts \-copy\_unknown \| ffplay \-

Nota sobre unidades: sat-zap hace \-r (Symbol Rate) en BAUDIOS, no KILOBAUDIOS. 30000 KSym/s son 30000000 Sym/s. Dandole 30000 (30 ksym/s) hace que FE\_SET\_PROPERTY falle con EINVAL y no haga tuning. boxstream \--sr, abertis tune \--sr usan ksym/s.

Los 6 feeds de 11222H 0x7E9 hasta 0x7EB más 0x802 usan (CLAVE 1 REDACTADA); 0x7EC y 0x80C usan (CLAVE 2 REDACTADA).

# Listas de canales y transpondedores (Verificados) {#listas-de-canales-y-transpondedores-(verificados)}

Todos estos carriers engancharon en la **HVR-4000** y se capturaron usando sat-zap. Cada pid luego se abrió con abiertis y los resultados se analizaron con tsanalyze, en casos especiales y ffprobe, en casos donde la PMT/PAT estaba correcta.

Banda baja (LO 9750, 22 kHz off) es cualquier cosa menor a 11700 MHz; banda alta (LO 10600, 22 kHz on) es mayor.

| FREQ | Polaridad(Horizontal/ Vertical) | Tasa de símbolos (SR) | FEC | TSID EXTERNO | Feeds |
| :---- | :---- | ----: | :---- | :---- | ----: |
| 11222 MHz | H | 30000 | 3/4 | `0xEA74` | 6 |
| 11302 MHz | H | 30000 | 3/4 | `0xEA76` | 5 |
| 11347 MHz | H | **20858** | 3/4 | `0xEA77` | 5 |
| 11382 MHz | H | 30000 | 3/4 | `0xEA79` | 5 |
| 11502 MHz | V | **10200** | 3/4 | `0xEA7C` | 1 |
| 11653 MHz | H | **19680** | 3/4 | `0xEA63` | 2 |
| 11675 MHz | H | **15750** | 3/4 | `0xEA64` | 3 |
| 12548 MHz | V | **29600** | 3/4 | `0xEA67` | 3 |
| 12583 MHz | V | **16900** | 3/4 | `0xEA68` | 1 |
| 12631 MHz | V | 30000 | 3/4 | `0xEA62` | 3 |
| 12671 MHz | V | 30000 | 3/4 | `0xEA61` | 4 |

Al principio había intentado hacer lock con todos los SR en 30000, por eso, los que no son 30000, se han marcado en negrita.

#### Feed \- Demux \- TS interno (Abierto) {#feed---demux---ts-interno-(abierto)}

Las claves son BISS 1 de 8-bytes. En los archivos de keys normales, los labels son decimales, así que PID 8000 es en realidad 0x1F40, 8008 es 0x1F48 y etc.  
Los TSID internos repetidos son de esperar. RTVE emite desconexiones por región, con los mismos TSID. (Por ejemplo, 0x041B es el TSID de tdp/24h/Radio 4/La 2, que llevan 3 carriers).  
No todas las desconexiones regionales comparten TSID, por ejemplo, A3Media también emite canales alternativos para Canarias, con feeds de video/audio completamente diferentes (Anuncios sobre todo, que se adaptan a negocios locales de las islas)  
El mux de canarias se encuentra en 11653 H

| TP | PID EXTERNO | CLAVE (REDACTADAS) | TSID interno | anales/servicios |
| :---- | :---- | :---- | :---- | :---- |
| 11222 H | 0x07E9 |  | 0x0455 | tdp HD, 24h HD, La 1 HD, RNE Extremadura, Radio 5 RNE, Ràdio 4 RNE, La 2 HD |
| 11222 H | 0x07EA |  | 0x03F3 | La 1 HD, RNE La Rioja, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11222 H | 0x07EB |  | 0x03F5 | tdp HD, 24h HD, La 1 HD, RNE Murcia, Radio 5 RNE, Ràdio 4 RNE, La 2 HD |
| 11222 H | 0x07EC |  | 0x03F8 | La 1 HD, RNE Valencia, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11222 H | 0x0802 |  | 0x041B | tdp HD, 24h HD, Radio 4 RNE, La 2 HD |
| 11222 H | 0x080C |  | 0x0001 | 24hHD HEVC Sat, La1HD HEVC Sat, La2HD HEVC Sat (HEVC 1920×540) |
| 11302 H | 0x08DE |  | 0x041B | tdp HD, 24h HD, Radio 4 RNE, La 2 HD |
| 11302 H | 0x08DF |  | 0x03F6 | La 1 HD, RNE Navarra, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11302 H | 0x08E0 |  | 0x03EA | La 1 HD, RNE Asturias, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11302 H | 0x08E1 |  | 0x044D | La 1 HD, RNE Aragón, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11302 H | 0x08E2 |  | 0x03EE | La 1 HD, RNE Cantabria, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11347 H | 0x08FE |  | 0x03F1 | Canal Extremadura TV, Canal Extremadura Radio |
| 11347 H | 0x08FF |  | 0x1450 | A7, A8, R. Langreo, RPA (MPEG-2 720×576) |
| 11347 H | 0x0901 |  | 0x0001 | TVG TDT-Sat (H.264 1280×720) |
| 11347 H | 0x0902 |  | 0x0001 | CyL7 (H.264 1920×1080) |
| 11347 H | 0x0904 |  | 0x0065 | TV3, SX3/33, 3CatInfo, Esport3, IB3 Global, CatRadio, CatMusic, iCat.cat |
| 11382 H | 0x09D8 |  | 0x041B | tdp HD, 24h HD, Ràdio 4 RNE |
| 11382 H | 0x09D9 |  | 0x03EB | La 1 HD, RNE Baleares, Radio 5 RNE, La 2 HD, tdp HD, 24h HD, Ràdio 4 RNE |
| 11382 H | 0x09DA |  | 0x03ED | La 1 HD, RNE Castilla La Mancha, Radio 5 RNE, La 2 HD, tdp HD, 24h HD, Ràdio 4 RNE |
| 11382 H | 0x09DB |  | 0x03E8 | La 1 HD, RNE Andalucía, Radio 5 RNE, La 2 HD, tdp HD, 24h HD, Ràdio 4 RNE |
| 11382 H | 0x09DC |  | 0x03EC | La 1 HD, RNE Castilla y León, Radio 5 RNE, La 2 HD, tdp HD, 24h HD, Ràdio 4 RNE |
| 11502 V | 0x0321 |  | 0x1450 | A7, A8, R. Langreo, RPA (H.264 1920×1080) |
| 11653 H | 0x012D |  | 0x03EF | tdp HD, 24h HD, La 1 HD, La 2 HD, RNE Canarias, Radio 5 RNE Tenerife, Radio 5 RNE Las Palmas, Ràdio 4 RNE |
| 11653 H | 0x012F |  | 0x000F | Antena 3, laSexta, neox, nova |
| 11675 H | 0x01A4 |  | 0x041B | tdp HD, 24h HD, Radio 4 RNE, La 2 HD |
| 11675 H | 0x01A5 |  | 0x03F7 | La 1 HD, RNE País Vasco, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 11675 H | 0x01A7 |  | 0x03F2 | La 1 HD, RNE Galicia, Radio 5 RNE, tdp HD, 24h HD, Ràdio 4 RNE, La 2 HD |
| 12548 V | 0x02BD |  | 0x000E | SQUIRREL, SQUIRREL2, DMAX, VEO 7, BOM RADIO, RADIO MARCA, CADENA 100, RADIO MARIA |
| 12548 V | 0x02BE |  | 0x000F | Antena 3, laSexta, neox, nova |
| 12548 V | 0x02BF |  | 0x0010 | FDF, Divinity, Telecinco HD, Cuatro HD |
| 12583 V | 0x1F48 |  | 0x9C40 | La 1 UHD (HEVC 3840×2160), Clan HD, DKISS, Radio Clasica HQ RNE, Radio3 HQ RNE, Radio Exterior RNE, Kiss FM, HIT FM, esRadio, SER, LOS40, DIAL |
| 12631 V | 0x1F41 |  | 0x03F4 | tdp HD, 24h HD, La 1 HD, La 2 HD, Radio Nacional de España, Radio 5 RNE, Ràdio 4 RNE |
| 12631 V | 0x1F42 |  | 0x1838 | Telemadrid HDR, Telemadrid HD, LA OTRA HD, BOM, ONDA MADRID |
| 12631 V | 0x1F43 |  | 0x0011 | MEGA, TRECE, Energy, Boing, ONDA CERO, EUROPA FM, Melodia FM, COPE, Rock FM |
| 12671 V | 0x1F40 |  | 0x0001 | **ALERTA (MPEG-2 720×576)** |
| 12671 V | 0x1F44 |  | 0x0012 | A3Series, BeMad TV HD, Real Madrid TV HD, TEN, LOS40 Classic, LOS40 Urban, Radiolé |
| 12671 V | 0x1F45 |  | 0x0010 | FDF, Divinity, Telecinco HD, Cuatro HD |
| 12671 V | 0x1F46 |  | 0x03F0 | tdp HD, 24h HD, La 1 HD, La 2 HD, RNE Cataluña, Radio 5 RNE, Ràdio 4 RNE |

## Anotaciones {#anotaciones}

El mecanismo interno es igual para absolutamente todo. DVB-S2 normal. PIDs en abierto. Con los paquetes interiores cerrados.

Se asume que los PIDs/TP fallidos usan otros sistemas, S2X, MS, PLS, etc...

# Sin comprobar/Fallidos {#sin-comprobar/fallidos}

Transpondedores que NO he podido bloquear o no han funcionado en esta combinación de hardware actual (WinTV HVR 4000 (PCI, no Express) normal en Linux)  
Estos transpondedores están marcados como *NO PROBADOS*, en algún momento voy a volver a esto e intentar sintonizarlos con otro hardware.

| Transpondedor | Parámetros (Supuestos) | Diagnóstico/Notas |
| :---- | :---- | :---- |
| 12599.76 V (8009) | 10200, 3/4 | No hay lock en 12599760, 12599000 o 12600000, el servicio 8009 no se puede recuperar aquí. ¿MIS/PLS? |
| 11342 H | 30000, 3/4 | **No lock**, según internet es Abertis ¿Probablemente MIS/PLS? |
| 12084 V (old 8008\) | 16900, 3/4 | **Carrier vacío. Hace lock**, pero no se detecta nada en la señal. TSANALYZE devuelve los 40mbps de “stuffing”. |
| 11330 H | 9141, 3/4 | MIS/PLS (Aragón). |
| 11491 V | 7522, 8/9 | MIS/PLS (CMM-TV). |
| 11496 H | 7884, 8/9 | MIS/PLS (RTPA). |
| 11422 H | 30000, 3/4, 16APSK | ¿Puede que ya no exista? |
| 11510 V | 10000, 3/4 | ¿Sin señal? |

