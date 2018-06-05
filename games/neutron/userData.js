'use strict';

const { PieceKind } = require('./gameutils');

class UserData {

	constructor() {
		this.board = [
			[PieceKind.BLACK, PieceKind.BLACK, PieceKind.BLACK, PieceKind.BLACK, PieceKind.BLACK],
			[PieceKind.CELL, PieceKind.CELL, PieceKind.CELL, PieceKind.CELL, PieceKind.CELL],
			[PieceKind.CELL, PieceKind.CELL, PieceKind.NEUTRON, PieceKind.CELL, PieceKind.CELL],
			[PieceKind.CELL, PieceKind.CELL, PieceKind.CELL, PieceKind.CELL, PieceKind.CELL],
			[PieceKind.WHITE, PieceKind.WHITE, PieceKind.WHITE, PieceKind.WHITE, PieceKind.WHITE]];
		this.movements = [];
		this.whoMove = 0;
		this.selectedChip = undefined;
		this.neutronFrom = undefined;
		this.neutronTo = undefined;
	}
}

module.exports = UserData;
