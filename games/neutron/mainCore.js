'use strict';

const util = require('util');

const path = require('path');
const jsonfile = require('jsonfile');
const moment = require('moment');

const Move = require('./move.js');
const FullMove = require('./fullMove.js');
const { applyFullMove, PieceKind, updateBoard, moves, applyMove, checkGameOver, tableToString } = require('./gameutils');
const { maxValue } = require('./minimax');

const { NativeMinimax } = require('./bindings');

/**
 * Pieces rotation.
 */
const rotation = [PieceKind.NEUTRON, PieceKind.WHITE];

/**
 * Get the player in turn.
 */
function getWhoMove(userData) {
    return rotation[userData.whoMove];
}

/**
 * Update player's turn.
 */
function updateWhoMove(userData) {
    userData.whoMove = (userData.whoMove + 1) % 2;
}

/**
 * Cell click event.
 * @param {*} row 
 * @param {*} col 
 */
exports.onCellClicked = function (row, col, userData) {
    return new Promise(async (resolve, reject) => {
        let endGame;
        if (userData.board[row][col] === getWhoMove(userData)) {
            updateBoard([], userData.board);
            const move = new Move(row, col, getWhoMove(userData));
            const m = moves(move, userData.board);
            updateBoard(m.concat(move), userData.board);
            userData.selectedChip = move;
        } else if (userData.board[row][col] === PieceKind.SCELL) {
            applyMove(userData.selectedChip, new Move(row, col, userData.selectedChip.kind), userData.board);
            updateBoard([], userData.board);

            if (getWhoMove(userData) === PieceKind.WHITE) {
                endGame = checkGameOver(userData.neutronTo, PieceKind.WHITE, userData.board);
                userData.movements.push(new FullMove([userData.neutronFrom, userData.neutronTo, userData.selectedChip, new Move(row, col, userData.selectedChip.kind)], 0));

                if (!endGame.success) {
                    const objStr = await NativeMinimaxPromise(userData.board);
                    const obj = JSON.parse(objStr);
                    const machineFullMove = new FullMove(obj.moves, obj.score);
                    if (!machineFullMove.empty()) {
                        userData.movements.push(machineFullMove);
                        userData.neutronTo = machineFullMove.moves[1];
                        applyFullMove(machineFullMove, userData.board);
                        updateBoard([], userData.board);
                        endGame = checkGameOver(machineFullMove.moves[1], PieceKind.BLACK, userData.board);
                    } else {
                        endGame = { success: true, kind: PieceKind.WHITE };
                    }
                }
            } else {
                userData.neutronFrom = userData.selectedChip;
                userData.neutronTo = new Move(row, col, userData.selectedChip.kind);
                endGame = checkGameOver(userData.neutronTo, PieceKind.WHITE, userData.board);
            }

            updateWhoMove(userData);
            userData.selectedChip = undefined;

        } else {
            updateBoard([], userData.board);
            userData.selectedChip = undefined;
        }
        resolve({ board: userData.board, moves: userData.movements, endgame: endGame || { success: false } });
    });
}

function NativeMinimaxPromise(board) {
    return new Promise((resolve, reject) => {
        NativeMinimax(JSON.stringify({ board: board }), false, (error, result) => {
            if (error) return reject(error);
            return resolve(result);
        });
    });
}