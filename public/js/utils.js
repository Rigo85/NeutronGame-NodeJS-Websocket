
// $(document).ready(function () {
// 	const a = $('.sidenav');
// 	console.log(a);
// 	console.log(a[0]);
// 	console.log(a.length);
// 	$('.sidenav')[0].sidenav();	
// });

document.addEventListener('DOMContentLoaded', function () {
	var elems = document.querySelectorAll('.sidenav');
	const options = {};
	var instances = M.Sidenav.init(elems, options);
});

var openFile = function (event) {
	var input = event.target;

	var reader = new FileReader();
	reader.onload = function () {
		var text = reader.result;
		const ws = window[Symbol.for('ws.client')];
		if (!ws) return;

		ws.send(`{"game": "${window[Symbol.for('ws.game.id')]}", "event": "game:load", "sender": "${window[Symbol.for('ws.send.id')]}", "content": "${text}"}`);
	};

	reader.readAsText(input.files[0]);
}

function newGame(event) {
	const ws = window[Symbol.for('ws.client')];
	if (!ws) return;
	ws.send(`{"game": "${window[Symbol.for('ws.game.id')]}", "event": "game:new", "sender": "${window[Symbol.for('ws.send.id')]}", "content": "{}"}`);
}

function saveGame(event) {
	const ws = window[Symbol.for('ws.client')];
	if (!ws) return;
	ws.send(`{"game": "${window[Symbol.for('ws.game.id')]}", "event": "game:save", "sender": "${window[Symbol.for('ws.send.id')]}", "content": "{}"}`);
}



