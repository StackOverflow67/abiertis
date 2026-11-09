// ============================================================================
// ESTE PROGRAMA NO BUSCA CLAVES, NO SALTEA NINGUN TIPO DE CIFRADO,
// JUAN PEDRO NO SE HACE RESPONSABLE DEL USO PARA PIRATERIA.
// EL USO DE ESTE PROGRAMA PARA ABRIR STREAMS DE ABERTIS SIN PERMISO EXPLICITO VA CONTRA LA LEY
// La TDT es gratis. No la piratees.
// ============================================================================
//
// abiertis.c - Por Juan Pedro (@StackOverflow67@masto.es)
//
// Hace de proxy entre un PID exterior cifrado de Abertis (Antiguo Cellnex) y un TS interior claro destinado a repetidores de TDT.
// Lee el TS exterior por stdin, abre el PID pedido y escribe el TS interior
// por stdout. Pensado para concadenar con otros programas.
//
//   cat /dev/dvb/adapter0/dvr1 | ./abiertis --pid 0x0001 --key ............ | vlc -
//   ./abiertis --pid 0x0001 --key ............... < captura_transpondedor_abertis_hw30.ts > salida_limpia.ts

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAM_PAQUETE_TS 188 // Abertis solo 188, variable en el formato MPEG-TS real, pero abiertis, eso, solo 188 siempre.

#define TAM_BLOQUE_LECTURA (188 * 4096) // 770 KB o asi
#define VACIAR_TRAS_BYTES (188 * 256)   // 48 KB (Flushear antes????)

#include "sbox_csa.h" // SBOX_CSA.H ESTA GENERADO 100% CON IA. Se parece mucho al codigo de VideoLan. Probado.

// SIN IMPLEMENTAR
// COSAS PARA HACER BACKUP DE TELETEXTO.

// int guarda_teletexto = 0;
// char *bin_teletextio_ext = "teletextio-DBG";
// int teletexto_autodect = 1;
// include <teletextio_externo.h>;
// buffertxto BUFFER_TELETEXTIO[8192];
//void guardar_ttxto(paquete) {
//	char *teletextio_paquete_raw = teletextio_det(paquete);
//}

static inline int pidPaquete(const uint8_t *paquete) {
	return (paquete[1] & 0x1f) * 256 + paquete[2];
}

static inline int afcPaquete(const uint8_t *paquete) {
	return (paquete[3] >> 4) & 3;
}

static inline size_t payloadPaquete(const uint8_t *paquete, size_t *desplazamiento) {
	size_t inicio = 4;
	int afc = afcPaquete(paquete);

	if (!(afc & 1)) { return 0; }

	if (afc & 2) {
		inicio = 5 + (size_t)paquete[4];
		if (inicio > TAM_PAQUETE_TS) { return 0; }
	}

	*desplazamiento = inicio;
	return TAM_PAQUETE_TS - inicio;
}

// buffer que crece solo
typedef struct {
	uint8_t *buf;
	size_t len, cap;
} BufDinamico;

// Si esto pasa en Enigma el deco crashea completo. (Igual detectar si estamos en enigma y resetear?)
static void errorFatal(const char *mensaje) {
	fprintf(stderr, "abiertis: %s\n", mensaje);
	exit(2);
}

static void crecerBuffer(BufDinamico *buffer, size_t adicional) {
	size_t necesario = buffer->len + adicional;

	if (necesario <= buffer->cap) { return; }

	size_t capNueva = buffer->cap;
	if (!capNueva) { capNueva = 188 * 16384; }
	while (capNueva < necesario) {
		capNueva *= 2;
	}

	uint8_t *nuevoBloque = realloc(buffer->buf, capNueva);
	if (!nuevoBloque) { errorFatal("out of memory"); }

	buffer->buf = nuevoBloque;
	buffer->cap = capNueva;
	//printf("DEVUELTO OK");
	// Habia un bug gordisimo en la cadena de mandar paquetes aqui. Era que estaba llamando buffer-> cap con buffer->cap = buffer->cap directo.
}

static void agregarBuffer(BufDinamico *buffer, const void *origen, size_t cantidad) {
	crecerBuffer(buffer, cantidad);
	//printf("LLEGA TRAS CRECER BUFFER");
	memcpy(buffer->buf + buffer->len, origen, cantidad);
	buffer->len += cantidad;
}

static int pidBuscado = -1;

// restos de una version anterior
static int pidViejo = -1;
static int modoVerbose = 0;

//static int modoMasVerbose = 0;
//static int modoTODAVIAMasVerbose = 0;


static struct {
	unsigned long long bytes_entrada, paquetes_entrada, basura_entrada, resto_entrada;
	unsigned long long paquetes_objetivo, abiertos, sin_carga_util;
	unsigned long long paquetes_internos, paquetes_tei, filtrados;
	unsigned long long emitidos, duplicados, cc_arreglados, bytes_resincro;
} estadisticas;

static int modoSilencio = 0;

static void mostrarStats(void) {
	if (modoSilencio) { return; } // TODO: Dejar de contar estadisticas en modo silencio.
	// Aunque como, he mencionado en otras ocsaiones, en HiSillicon cualquier logging es mejor que nada xd.

	fprintf(stderr,
		"\tabiertis: Entrada: %llu paquetes (%.1f MB) totales, del PID 0x%04X: %llu paquetes"
		" (%llu abiertos)\n"
		"\tPaquetes internos: %llu, han salido:  %llu (%.1f MB),"
		" duplicados: %llu; continuidad arreglada: %llu; Resincronizaciones requeridas: %llu B\n",
		estadisticas.paquetes_entrada, estadisticas.bytes_entrada / 1e6, pidBuscado, estadisticas.paquetes_objetivo,
		estadisticas.abiertos, estadisticas.paquetes_internos, estadisticas.emitidos,
		estadisticas.emitidos * 188 / 1e6, estadisticas.duplicados, estadisticas.cc_arreglados, estadisticas.bytes_resincro
	);
}

static BufDinamico bufferSalida;

// Salir bien (Al cerrar bien).
// (Arregla el bug del bridge enviando cosas raras al log si peta)
// Solo para Enigma2
// SI OCURRE EN LINUX NORMAL DE PC, pues escribes 'reset' en el terminal bruh.
static void flushSalida(void) {
	size_t escrito = 0;

	while (escrito < bufferSalida.len) {
		ssize_t cuantos = write(STDOUT_FILENO, bufferSalida.buf + escrito, bufferSalida.len - escrito);

		if (cuantos < 0) {
			if (errno == EINTR) { continue; }
			if (errno == EPIPE) { exit(0); }
			errorFatal("NO SE PUEDE ESCRIBIR A STDOUT");
		}

		escrito += (size_t)cuantos;
	}

	bufferSalida.len = 0;
}

static void mandarPaquete(const uint8_t *paquete) {
	agregarBuffer(&bufferSalida, paquete, TAM_PAQUETE_TS);
	//printf("LLEGA TRAS AGREGAR BUFFER");
	estadisticas.emitidos++;

	if (bufferSalida.len >= VACIAR_TRAS_BYTES) { flushSalida(); }
}

// payloads de 184 pegados: hay que ir buscando el sync
static BufDinamico bufferInterior;
static size_t posInterior = 0;
static int syncInterior = 0;

static uint8_t ccVisto[8192];
static uint8_t ccUltimo[8192];
static uint8_t pidSeleccionado[8192];
static uint8_t pidVisto[8192];
static uint8_t ultimoPaquetePid[8192][TAM_PAQUETE_TS];

static int filtrando = 0;
static int arreglarCc = 1; // SI POR DEFECTO
static int tirarDuplicados = 1;

static void sacar_internos(uint8_t *paquete) {
	int pid, afc, cc;

	estadisticas.paquetes_internos++;

	if (paquete[1] & 0x80) {
		estadisticas.paquetes_tei++;
		return;
	}

	pid = pidPaquete(paquete);

	if (filtrando && !pidSeleccionado[pid]) {
		estadisticas.filtrados++;
		return;
	}

	afc = afcPaquete(paquete);

	// EL TEI PUEDE JODER ALGUNAS COSAS SI NO SE HACE BIEN LO DEL CC-DEDUPE
	if ((afc & 1) && pid != 0x1FFF && pidVisto[pid] &&
		memcmp(paquete, ultimoPaquetePid[pid], TAM_PAQUETE_TS) == 0) {
		estadisticas.duplicados++;
		if (tirarDuplicados) { return; }
	}

	memcpy(ultimoPaquetePid[pid], paquete, TAM_PAQUETE_TS);
	pidVisto[pid] = 1;

	// Reescribir CC si esta raro (Mas bajo o alto del que espero), ESTO CONSUME MUCHISIMA CPU EN MI DECO (Dinobot Twin 4K), PERO SIN ARREGALR LOS CC PUEDE PETAR A LA PRIMERA DE CAMBIO
	// (En PC esta bien)
	if (afc & 1) {
		cc = paquete[3] & 0x0f;

		if (ccVisto[pid]) {
			int esperado = (ccUltimo[pid] + 1) & 0x0f;

			if (cc != ccUltimo[pid]) {
				if (cc != esperado) { estadisticas.cc_arreglados++; }

				ccUltimo[pid] = esperado;
				if (arreglarCc) { paquete[3] = (uint8_t)((paquete[3] & 0xf0) | esperado); }
}
		} else {
			ccVisto[pid] = 1;
			ccUltimo[pid] = cc;
		}
	}

	mandarPaquete(paquete);
	//printf("LLEGA TRAS MANDAR PAQUETE");
}

static inline int esSync(const uint8_t *p) {
	return p[0] == 0x47;
}

// offset en vez de memmove: en Enigma2 el memmove se nota mucho por que los decos suelene estar faltitos de memoria.
static void procesar_interno(int finEntrada) {
	size_t posicion = posInterior;

	while (posicion + TAM_PAQUETE_TS <= bufferInterior.len) {
		if (!esSync(bufferInterior.buf + posicion)) {
			syncInterior = 0;
			estadisticas.bytes_resincro++;
			posicion++;
			continue;
		}

		if (!syncInterior) {
			if (posicion + TAM_PAQUETE_TS < bufferInterior.len && !esSync(bufferInterior.buf + posicion + TAM_PAQUETE_TS)) {
				estadisticas.bytes_resincro++;
				posicion++;
				continue;
			}

			if (posicion + TAM_PAQUETE_TS == bufferInterior.len && !finEntrada) { break; }

			syncInterior = 1;
		}

		sacar_internos(bufferInterior.buf + posicion);
		posicion += TAM_PAQUETE_TS;
	}

	posInterior = posicion;

	if (posInterior >= (188 * 4096) || posInterior == bufferInterior.len) {
		size_t resto = bufferInterior.len - posInterior;
		if (resto > 0) { memmove(bufferInterior.buf, bufferInterior.buf + posInterior, resto); }
		bufferInterior.len = resto;
		posInterior = 0;
	}
}

// TODO: rebasar el PTS. A medias, se desborda con streams largos
//static int rebasar_PTS(void) {
//	return 0; // EN PROGRESO
//}
// Tras varias pruebas con otras herramientas, no voy a rebasar el PTS, la mayoria de reproductores pueden perfectamente soportar rollbacks y etc. (Simulado) En VLC, FFPLAY, GStreamer, MPV, y VLC en Android se vuelve a cero y no ocurre NADA. Aunque en
// VLC hay un pequeño bug que hace que el teletexto... Deje de funcionar? No se, literalmente deja de existir en cuando el PTS pasa de un numero superior a cero a cero.


static CsaCipher controlWord;
static int hayClave = 0;

static int hexANibble(int caracter) {
	if (caracter >= '0' && caracter <= '9') { return caracter - '0'; }
	if (caracter >= 'a' && caracter <= 'f') { return caracter - 'a' + 10; }
	if (caracter >= 'A' && caracter <= 'F') { return caracter - 'A' + 10; }
	return -1;
}

static void instalarClave(const char *texto) {
	size_t longitud = strlen(texto);
	uint8_t clave[8];

	if (longitud != 12 && longitud != 16) {
		fprintf(stderr, "abiertis: --key tiene que ser hex de 16 dígitos (o 12 si es de 48 bits)!!!\n");
		exit(2);
	}

	for (size_t indice = 0; indice < longitud; indice++) {
		if (hexANibble(texto[indice]) < 0) {
			fprintf(stderr, "abiertis: la clave tiene un carácter que no vale (0-9, A-F)!!!\n");
			exit(2);
		}
	}

	if (longitud == 16) {
		for (int indice = 0; indice < 8; indice++)
			clave[indice] = (uint8_t)(hexANibble(texto[2 * indice]) * 16 + hexANibble(texto[2 * indice + 1]));
		loadControlWord(&controlWord, clave, 0);
	} else {
		// 12 digitos: reconstruir como el deco (bytes 3 y 7)
		uint8_t datos[6];

		for (int indice = 0; indice < 6; indice++)
			datos[indice] = (uint8_t)(hexANibble(texto[2 * indice]) * 16 + hexANibble(texto[2 * indice + 1]));

		clave[0] = datos[0]; clave[1] = datos[1]; clave[2] = datos[2];
		clave[4] = datos[3]; clave[5] = datos[4]; clave[6] = datos[5];
		clave[3] = (uint8_t)(clave[0] + clave[1] + clave[2]);
		clave[7] = (uint8_t)(clave[4] + clave[5] + clave[6]);

		loadControlWord(&controlWord, clave, 1);
	}

	hayClave = 1;
}

static uint8_t packetBatch[PACKET_LANES][TAM_PAQUETE_TS];
static int batchCount = 0;

// Esto lo he copiado casi 1:1 de stackoverflow lol
static void bs_flush(void) {
	BatchCipher S;
	uint8_t *ptrs[PACKET_LANES];
	uint64_t iv[8][8];

	if (!batchCount) { return; }

	set_batch_key(&S, &controlWord.st);
	memset(iv, 0, sizeof(iv));

	for (int k = 0; k < batchCount; k++) {
		ptrs[k] = packetBatch[k];

		for (int j = 0; j < 8; j++) {
			for (int b = 0; b < 8; b++) {
				if ((packetBatch[k][4 + j] >> b) & 1) { iv[j][b] |= (uint64_t)1 << k; }
			}
		}
	}

	seedBatchCipher(&S, iv);

	xor_batch_bytes(&S, ptrs, 12, 176, batchCount);

	for (int k = 0; k < batchCount; k++) {
		decipherBlockChain(&controlWord.blk, packetBatch[k] + 4, 184);
		estadisticas.abiertos++;
		agregarBuffer(&bufferInterior, packetBatch[k] + 4, 184);
		procesar_interno(0);
	}

	batchCount = 0;
}

static void procesarExt(uint8_t *paquete) {
	int pid, control;
	size_t desplazamiento, longitud;

	estadisticas.paquetes_entrada++;

	if (paquete[1] & 0x80) { return; }

	pid = pidPaquete(paquete);
	if (pid != pidBuscado) { return; }

	estadisticas.paquetes_objetivo++;

	longitud = payloadPaquete(paquete, &desplazamiento);
	if (!longitud) {
		estadisticas.sin_carga_util++;
		return;
	}

	control = (paquete[3] >> 6) & 3;
	if (control == 2 || control == 3) {
		if (!hayClave) {
			fprintf(stderr, "abiertis: el PID 0x%04X va cifrado y no hay --key\n", pidBuscado);
			exit(1);
		}

		if (longitud == 184 && desplazamiento == 4) {
			memcpy(packetBatch[batchCount], paquete, TAM_PAQUETE_TS);
			if (++batchCount == PACKET_LANES) { bs_flush(); }
			return;
		}

		bs_flush();
		decipher_payload(&controlWord, paquete + desplazamiento, longitud);
		estadisticas.abiertos++;
	} else {
		bs_flush();
	}

	agregarBuffer(&bufferInterior, paquete + desplazamiento, longitud);
	procesar_interno(0);
}

static void uso(FILE *destino) { // O poner siempre a stderr, no se.
	fprintf(destino,
		"Uso: abiertis --pid PID --key CW [opciones] < externo.ts > interno.ts\n"
		"\n"
		"Abre y desencripta UN PID externo de Abertis/Cellnex de TDTSAT (Hispasat) y devuelve un DVB TS limpio.\n"
		"Ejemplo:\n"
		"\tcat /dev/dvb/adapter0/dvr1 | ./abiertis --pid 0x000 --key 000000000000 | ffplay -\n"
		"\n"
		"Opciones:\n"
		"\t--pid PID        PID externo (OBLIGATORIO)\n"
		"\t--key CW         Clave DVB-CSA2, 'BISS', 16 o 12 digitos hex\n"
		"\t--seleccionar-pid PID  Solo sacar este PID (Acepta mas de uno)\n"
		"\t\tOPCIONES AVANZADAS (No toques esto si no sabes lo que haces)\n"
		"\t--no-fix-cc      No re-escribir los CC (Contadores de Continuidad).\n"
		"\t--keep-dups      Mantener paquetes duplicados\n"
		"\t--quiet          No mandar estadisticas al STDERR\n"
		"\t\t(Las estadisticas se siguen contando, pero no se muestran)\n"
		"\t-a, --ayuda      Este mensaje\n");
}

int main(int argc, char **argv) {
	static const struct option opciones[] = {
		{ "pid",       required_argument, 0, 'p' },
		{ "key",       required_argument, 0, 'k' },
		{ "seleccionar-pid", required_argument, 0, 'i' },
		{ "no-fix-cc", no_argument,       0, 'C' },
		{ "keep-dups", no_argument,       0, 'D' },
		{ "quiet",     no_argument,       0, 'q' },
		{ "ayuda",     no_argument,       0, 'a' },
		{ 0, 0, 0, 0 }
	};
	const char *argumentoClave = NULL;
	int opcion;

	// si cierran la tuberia no queremos morir a mitad de un write
	signal(SIGPIPE, SIG_IGN);

	while ((opcion = getopt_long(argc, argv, "p:k:i:a", opciones, NULL)) != -1) {
		switch (opcion) {
			case 'p': {
				char *fin = NULL;
				long valor = strtol(optarg, &fin, 0);
				if (!fin || *fin || valor < 0 || valor > 8191) {
					fprintf(stderr, "abiertis: --pid malo\n");
					return 2;
				}
				pidBuscado = (int)valor;
				break;
			}
			case 'k':
				argumentoClave = optarg;
				break;
			case 'i': {
				char *fin = NULL;
				long valor = strtol(optarg, &fin, 0);
				if (!fin || *fin || valor < 0 || valor > 8191) {
					fprintf(stderr, "abiertis: --seleccionar-pid malo\n");
					return 2;
				}
				pidSeleccionado[valor] = 1;
				filtrando = 1;
				break;
			}
			case 'C':
				arreglarCc = 0;
				break;
			case 'D':
				tirarDuplicados = 0;
				break;
			case 'q':
				modoSilencio = 1;
				break;
			case 'a':
				uso(stdout);
				return 0;
			default:
				uso(stderr);
				return 2;
		}
	}

	if (pidBuscado < 0) {
		fprintf(stderr, "abiertis: falta --pid\n\n");
		uso(stderr);
		return 2;
	}

	if (argumentoClave) { instalarClave(argumentoClave); }

	atexit(mostrarStats); // BUG NEWLINE CUANDO SE USA CON EL BRIDGE DE PYTHON!!!!!

	crecerBuffer(&bufferInterior, 188 * 16384);

	uint8_t *bufEntrada = malloc(TAM_BLOQUE_LECTURA + TAM_PAQUETE_TS);
	if (!bufEntrada) { errorFatal("NO SE PUEDE ALOCAR MEMORIA INICIAL."); } // SON LITERALMENTE 700 PUTOS KILOBYTES. SI TE FALLA ESTO PROBABLEMENTE TIENES COMO 2MB DE RAM O ALGO POR DIOS

	size_t disponible = 0;
	uint8_t paquete[TAM_PAQUETE_TS];

	while (1) {
		ssize_t leidos = read(STDIN_FILENO, bufEntrada + disponible, TAM_BLOQUE_LECTURA);

		if (leidos < 0) {
			if (errno == EINTR) { continue; }
			break;
		}
		if (leidos == 0) { break; }

		disponible += (size_t)leidos;

		size_t desplazamiento = 0;
		while (desplazamiento + TAM_PAQUETE_TS <= disponible) { // Personalmente no me ha dado mucha guerra 
			// byte a byte hasta el siguiente sync
			if (!esSync(bufEntrada + desplazamiento)) {
				estadisticas.basura_entrada++;
				desplazamiento++;
				continue;
			}

			memcpy(paquete, bufEntrada + desplazamiento, TAM_PAQUETE_TS);
			desplazamiento += TAM_PAQUETE_TS;
			estadisticas.bytes_entrada += TAM_PAQUETE_TS;
			procesarExt(paquete);
		}

		// mover el resto al principio
		if (desplazamiento) {
			memmove(bufEntrada, bufEntrada + desplazamiento, disponible - desplazamiento);
			disponible -= desplazamiento;
		}

		bs_flush();
	}

	estadisticas.resto_entrada = disponible;
	bs_flush();
	procesar_interno(1);
	flushSalida(); // Esto me estaba volviendo loco por que ponia "flush salidad" con d al final y me estaba matando intentando ver por que el compilador no encontrada "flush salida", pero no estaba leyendo la d al final xd

	free(bufEntrada);
	free(bufferInterior.buf);
	free(bufferSalida.buf);
	return 0;
}
