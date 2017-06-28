'use strict';
let shell = require('shelljs');
let rede = require('./rede.js');

const SEGMENTO = "segmento";
const PACOTE = "pacote";

// Ags
// 0 - node (ignorar)
// 1 - rede.cliente.js (ignorar)
// 2 - protocolo
// 3 - porta de origem
// 4 - ip destino
// 5 - porta destino
if (process.argv.length < 6) {
	console.log("REDES - Parâmetros insuficientes!");
	console.log("node rede_cliente.js protocolo porta_origm(fis) ip_destino porta_destino(fis)");
	process.exit()
}

function sleep(milliseconds) {
	var start = new Date().getTime();
	for (var i = 0; i < 1e7; i++) {
		if ((new Date().getTime() - start) > milliseconds){
			break;
		}
	}
	start = new Date().getTime();
	for (var i = 0; i < 1e7; i++) {
		if ((new Date().getTime() - start) > milliseconds){
			break;
		}
	}
}

var protocolo = process.argv[2];
var ip_origem = shell.exec("ip route show default | grep 'src' | awk '{print $9}' | head -1", { silent: true }).stdout.replace(/\n/, "");
var porta_origem = process.argv[3];
var ip_destino = process.argv[4];
var porta_destino = process.argv[5];
var pacotes = {};

rede.montaPacote();
rede.destroi(SEGMENTO);
console.log("REDES - Chama camada fisica e espera resposta...");
shell.exec('./fis_client.sh ' + protocolo + ' ' + porta_origem + ' ' + ip_destino + ' ' + porta_destino);
rede.montaSegmento();
rede.destroi(PACOTE);
