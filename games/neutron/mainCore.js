'use strict';

const util = require('util');

const path = require('path');
const jsonfile = require('jsonfile');
const moment = require('moment');

const Move = require('./move.js');
const FullMove = require('./fullMove.js');
const { applyFullMove, PieceKind, updateBoard, moves, applyMove, checkGameOver } = require('./gameutils');
const { maxValue } = require('./minimax');

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
 * Update players's turn.
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
    return new Promise((resolve, reject) => {
        let endGame;
        if (userData.board[row][col] === getWhoMove(userData)) {
            updateBoard([], userData.board);
            const move = new Move(row, col, getWhoMove(userData));
            const m = moves(move, userData.board);
            updateBoard(m.concat(move), userData.board);
            userData.selectedChip = move;
        } else if (userData.board[row][col] == PieceKind.SCELL) {

            applyMove(userData.selectedChip, new Move(row, col, userData.selectedChip.kind), userData.board);
            updateBoard([], userData.board);

            if (getWhoMove(userData) === PieceKind.WHITE) {
                endGame = checkGameOver(userData.neutronTo, PieceKind.WHITE, userData.board);
                userData.movements.push(new FullMove([userData.neutronFrom, userData.neutronTo, userData.selectedChip, new Move(row, col, userData.selectedChip.kind)], 0));

                if (!endGame.success) {
                    
                    const machineFullMove = maxValue(userData.board,3,Number.MIN_SAFE_INTEGER,Number.MAX_SAFE_INTEGER,PieceKind.BLACK);

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
