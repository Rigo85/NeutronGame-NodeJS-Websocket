'use strict';

const jsonfile = require('jsonfile');
const FullMove = require('./games/neutron/fullMove');
const Move = require('./games/neutron/move');

function readGame(path) {
    return new Promise((resolve, reject) => {
        jsonfile.readFile(path, (err, obj) => {
            if (err) {
                reject(err);
                return;
            }
            // obj.board
            // obj.movements
            // obj.whoMove;
            // obj.selectedChip;
            // obj.neutronFrom;
            // obj.neutronTo;
            resolve(obj.movements);
        })
    });

}

const path = "/mnt/c/Users/rigo/Downloads/neutron-game-2018-07-25T15-39-43-05-00.json";

readGame(path)
.then(movs => movs.map(mov => new FullMove(mov.moves.map(m => new Move(m.row, m.col, m.kind)), mov.score).toString()).join('\n'))
.then(console.log)
.catch(console.error);

