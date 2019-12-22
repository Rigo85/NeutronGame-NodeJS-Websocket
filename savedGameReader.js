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

// const path = "/home/rigo/neutron-game-2019-12-21T11-30-49-05-00.json";
if (process.argv.length > 2)
    readGame(process.argv[2])
        .then(movs => movs.map(mov => new FullMove(mov.moves.map(m => new Move(m.row, m.col, m.kind)), mov.score).toString()).join('\n'))
        .then(console.log)
        .catch(console.error);
else
    console.log("...saved games file!!!");

