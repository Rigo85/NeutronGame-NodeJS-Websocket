
var WebSocket = require('ws');
var cookie = require('cookie');
var cookieParser = require('cookie-parser');

var Neutron = require('./neutron/main');

class GameServer {
	constructor(server) {
		this.wss = new WebSocket.Server({ server });

		this.wss.on('connection', (ws, req) => {
			const sid = cookieParser.signedCookie(cookie.parse(req.headers.cookie)['connect.sid'], 'secret');

			ws.isAlive = true;
			ws.on('pong', () => { ws.isAlive = true; });

			ws.on('message', msg => {
				const message = JSON.parse(msg);
				let game = message.game;

				const gameHandlers = {
					'neutron': () => { Neutron.processEvent(message, ws, sid); },
					'default': () => {
						//TODO ver que hacer...!
					}
				};

				if (!gameHandlers[game]) {
					game = 'default';
				}

				gameHandlers[game]();
			});

			//ws.send('crear lo necesario para nuevo cliente');

			ws.on('error', (err) => {
				console.warn(`Client disconnected - reason: ${err}`);
			})
		});

		setInterval(() => {
			this.wss.clients.forEach(ws => {
				if (!ws.isAlive) return ws.terminate();

				ws.isAlive = false;
				ws.ping(null, undefined);
			});
		}, 10000);
	}
}

module.exports = GameServer;




