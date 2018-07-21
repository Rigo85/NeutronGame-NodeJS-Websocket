/**
 * Authors:
 * Rigoberto Leander Salgado Reyes <rlsalgado2006@gmail.com>
 *
 * Copyright 2018 by Rigoberto Leander Salgado Reyes.
 *
 * This program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 */

#pragma once

#include <array>

#include <PieceKind.h>
#include <Direction.h>
#include <Move.h>
#include <FullMove.h>


using Row = std::array<PieceKind, 5>;
using Table = std::array<Row, 5>;

class Board {
public:
    Board(Table* table);

    ~Board();

    Move *findNeutron();

    std::vector<FullMove*>* allMoves(PieceKind pieceKind);

    std::vector<Move *> *moves(Move *startPoint);

    std::vector<Move *> *findPieces(PieceKind pieceKind);

    void applyFullMove(FullMove *fullMove, bool apply = true);

    friend std::ostream &operator<<(std::ostream &ostr, const Board &board);

private:

    int getRowMove(Direction direction);

    int getColMove(Direction direction);

    bool inBounds(int value, int inc);

    Move *checkMove(Move *move, Direction direction);

    Move *_find(Table *b, int r, int c);

    Move *_check(int row, int col, int incR, int incC, Table *table);

    void applyMove(Move *from, Move *to);

    std::string pieceToString(PieceKind pieceKind) const;

    Table *table;
};


