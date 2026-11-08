# abiertis
Abiertis es un pequeño programa que vive entre la entrada de satelite y reproductor final. Este programa extrae y decodifica paquetes de TDTSAT en Hispasat, usando claves proporcionadas por el usuario, devolviendo un paquete .TS limpio interno. Estos son los mismos paquetes que llegan a los repetidores de TDT de casi toda España (Menos Andalucia y Pais Vasco (Estos usan repetidores basados en redes terrestres en vez de satelites))

# Declaración de uso de IA
El cifrado CSA2/Nucleo se reimplemento con asistencia de IA (DeepSeek vía OpenCode); el resto esta escrito a mano (Y copiado de internet, como Dios manda 👍).

# ¿De que se compone Abiertis?
Abiertis tiene 3 componentes principales;
1. Abiertis (herramienta de linea de comando)
2. Nucleo DVB-CSA reimpl.
3. Puentis (Puente Abiertis) (Que lanzare pronto)

Cada una de estas herramientas tiene un uso, y cada una se usa en un sitio (Fisicamente hablando) diferente.

Para un decodificador basado en Linux, necesitas tanto Nucleo Abiertis+Nucleo CSA+Puente, en combinacion con Abiertis (CLI).

Para PC solo necesitas Nucleo CSA + Abiertis (CLI) (Aunque pronto usare libdvbcsa nativo en x86). El puente sirve sobre todo para enviar datos en un mini servidor HTTP a Enigma2.

# ¿Que puede hacer Abiertis?
- Abrir paquetes Abertis
- Abrir otros paquetes T2-MI (Aun no lo he probado, lo tengo pendiente cuando apunte una antena a 5-Oeste (Los Italianos))
- Filtrar PIDs
- Decodificar flujos cifrados con DVB-CSA con claves proporcionadas por el usuario final

# ¿Que NO puede hacer Abiertis?
- NO puede crackear, bruteforcear o saltear ningun tipo de clave.
- NO puede re-escribir PAT/PMT.
- NO puede decodificar otros tipos de cifrado.
- NO puede cocinar pizza (Sin probar)

# ¿Para que (dispositivo) es Abiertis?
Abiertis se puede usar tanto en PCs como en decodificadores de satelite basados en Enigma2 como en "x", donde "x" es cualquier sitio que soporte operaciones basicas de procesador. El enfoque principal esta puesto tanto en PCs con Linux (AMD64/I386) o decodificadores HiSilicon y Broadcom. (Aunque los decos Broadcom funcionan suficientemente bien con "astra-sm"+"abertis"+"oscam")

El proyecto se crea de la necesidad de abrir paquetes Abertis en PC

# COMO USAR ABIERTIS (En PC)
Como Abiertis esta pensado como herramienta intermediaria, el uso en PC es super simple. Puedes ponerla en medio de algo como;
```bash
cat /dev/dvb/adapterX/dvrY | abiertis --key 001122334455 --pid 0x1234 | vlc -
```
Pero no tienes por que usarlo en directo. Puedes tambien ver grabaciones;
```bash
abiertis --key 001122334455 --pid 0x1234 < captura_abertis.ts | vlc -
```
O guardar la salida;
```bash
cat /dev/dvb/adapterX/dvrY | abiertis --key 001122334455 --pid 0x1234 > salida.ts
```

# COMO USAR ABIERTIS (En Decos Enigma2)
Mejor, mira el manual (Pronto) en /docs/e2.md

# Vale, ¿Pero que arregla esto?
La falta de software. Y la estabilidad en HiSilicon.

Al no tocar el Tuner ni el dispositivo fisico de ninguna forma, como hace OSCAM/Astra-SM/Abertis, no se causan bloqueos y se vuelve mucho mas estable y facil de mantener en HiSilicon.

Ademas de eso, no habia ninguna solucion (practica) para ver TDTSAT en PC y queria ofrecer una.

# TODO
Lo que queda por hacer es;

- Puente (Actualmente no lo puedo publicar por que tengo que separar claves y etc)
- Implementar soporte LibCSADVB (De Videolan)

## Opciones

```
abiertis --pid PID --key CLAVE [opciones] < externo.ts > interno.ts
```

| Opción | Corta | Argumento | Obligatoria | Descripción |
|--------|:-----:|-----------|:-----------:|-------------|
| `--pid` | `-p` | PID (decimal o `0x` hex) | Sí | PID externo que se abre. Rango 0–8191. |
| `--key` | `-k` | 12 o 16 dígitos hex | Solo si el PID va cifrado | Clave DVB-CSA2. Con 12 dígitos se calcula el resto de la clave (clave de 48 bits). |
| `--seleccionar-pid` | `-i` | PID (decimal o `0x` hex) | No | Saca solo ese PID interno. Se puede repetir para varios. Sin esta opción salen todos. |
| `--no-fix-cc` | | - | No | No reescribe los contadores de continuidad (CC). |
| `--keep-dups` | | - | No | Mantiene los paquetes duplicados en vez de descartarlos. |
| `--quiet` | | - | No | No muestra estadísticas por stderr (se siguen contando). |
| `--ayuda` | `-a` | - | No | Muestra la ayuda y sale. |

Por defecto se corrigen los CC y se descartan los duplicados. `--no-fix-cc` y `--keep-dups` son opciones avanzadas: no las toques si no sabes para que lo estas haciendo.

### Códigos de salida

| Código | Significado |
|:------:|-------------|
| 0 | Fin normal (o `--ayuda`). |
| 1 | El PID va cifrado y no se indicó `--key`. |
| 2 | Argumento incorrecto (PID fuera de rango, clave con longitud o dígitos no válidos, opción desconocida). |

Al terminar, `abiertis` imprime por stderr las estadísticas (paquetes de entrada, paquetes abiertos, duplicados, CC corregidos y resincronizaciones), salvo que uses `--quiet

# Reconocimientos
Una parte de este codigo esta "fuertemente inspirada" por Astra-SM. Tambien es codigo abierto

# ¿Que claves uso para este programa?/Aviso legal
No puedo, no quiero, y no pienso ofrecer ninguna clave relacionada con Abertis. Debes tener y saber las claves exactas que necesitas para tus usos.

Abrir el stream de TDTSAT sin permiso explicito constituye un delito de pirateria.

Esta herramienta esta pensada para usuarios legitimos que quieran ver TDTSAT en dispositivos no oficiales.

Este programa no esta relacionado con Abertis/Cellnex

La TDT es gratis. No la piratees.
