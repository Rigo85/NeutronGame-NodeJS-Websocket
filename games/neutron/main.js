'use strict';

const core = require('./mainCore.js');
const UserData = require('./userData');

// TODO, guardar el tablero de cada cliente...!
const data = {};

exports.processEvent = (message, ws, sid) => {

	createUserData(sid);

	let action = message.event;

	const actionHandlers = {
		'reload': () => { reloadEvent(message.content, ws, sid); },
		'game:save': () => {
			console.log('saving game!');
			// data[sid].saveGame();
		},
		'game:new': () => { newGameEvent(ws, sid); },
		'cell:click': () => { cellClickEvent(message.content, ws, sid); },
		'default': () => {
			// TODO ver q hacer!
		}
	};

	if (!actionHandlers[action]) {
		action = 'default';
	}

	actionHandlers[action]();
};

function reloadEvent(content, ws, sid) {
	ws.send(createMessage('board:updated', { board: data[sid].board, moves: data[sid].movements, endgame: { success: false } }));
}

function newGameEvent(ws, sid) {
	//data[sid].newGame();
	data[sid] = new UserData();
	ws.send('board:updated', { board: data[sid].board, moves: data[sid].movements, endgame: { success: false } });
}

function cellClickEvent({ id }, ws, sid) {
	const m = /(.*)-(\d)-(\d)/g.exec(id);
	core.onCellClicked(parseInt(m[2]), parseInt(m[3]), data[sid])
		.then(obj => ws.send(createMessage('board:updated', obj)))
		.catch(e => console.error(e));
}

function createMessage(event, content) {
	return JSON.stringify({ event: event, content: content });
}

function createUserData(sid) {
	if (!data[sid]) {
		data[sid] = new UserData();
	}
}
