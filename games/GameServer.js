
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
				let message, game = 'default', info = 'There is nothing to do!, try to start a new game!';

				try {
					message = JSON.parse(msg);
					game = message.game;
				} catch (error) {
					console.log(error);
					const name = error.name || 'Error';
					const errorMsg = error.message || 'empty error message';
					info = name == 'SyntaxError' ? 'The saved game is corrupted!' : `${name}: ${errorMsg}`;
				}

				const gameHandlers = {
					'neutron': () => { Neutron.processEvent(message, ws, sid); },
					'default': () => {
						ws.send(`{"event":"messages", "content": "${info}"}`);
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
				console.log(`------>>>pinging ${new Date()}`);
			});
		}, 10000);
	}
}

module.exports = GameServer;




