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

#include <functional>
#include <limits>

#include <cstdlib>

#include <Board.h>
#include <PieceKind.h>
#include <cleaners.h>

Board::Board(Table *table) {
    this->table = table;
}

Board::~Board() {
    delete table;
}

Move *Board::_find(Table *b, int r, int c) {
    if ((*b)[r][c] == PieceKind::NEUTRON)
        return new Move(r, c, PieceKind::NEUTRON);
    if (r == 5) return nullptr;
    auto _c = (c + 1u) % 5u;
    auto _r = r + (!_c ? 1u : 0u);

    return _find(b, _r, _c);
}

Move *Board::findNeutron() {
    return _find(table, 0, 0);
}

std::vector<Move *> *Board::findPieces(PieceKind pieceKind) {
    auto *pos = new std::vector<Move *>();
    for (auto row = 0u; row < 5u; row++) {
        for (auto col = 0u; col < 5u; col++) {
            if ((*table)[row][col] == pieceKind) {
                pos->push_back(new Move(row, col, pieceKind));
            }
        }
    }

    return pos;
}

int Board::getRowMove(Direction direction) {
    if (direction == Direction::NORTH) return -1;
    if (direction == Direction::SOUTH) return 1;
    if (direction == Direction::NORTHEAST) return -1;
    if (direction == Direction::NORTHWEST) return -1;
    if (direction == Direction::SOUTHEAST) return 1;
    if (direction == Direction::SOUTHWEST) return 1;

    return 0;
}

int Board::getColMove(Direction direction) {
    if (direction == Direction::WEST) return -1;
    if (direction == Direction::EAST) return 1;
    if (direction == Direction::NORTHEAST) return 1;
    if (direction == Direction::NORTHWEST) return -1;
    if (direction == Direction::SOUTHEAST) return 1;
    if (direction == Direction::SOUTHWEST) return -1;

    return 0;
}

bool Board::inBounds(int value, int inc) {
    return value + inc >= 0 && value + inc < 5;
}

Move *Board::_check(int row, int col, int incR, int incC, Table *table) {
    if (!inBounds(row, incR) ||
        !inBounds(col, incC) ||
        (*table)[row + incR][col + incC] != PieceKind::CELL)
        return new Move(row, col, (*table)[row][col]);
    return _check(row + incR, col + incC, incR, incC, table);
}

Move *Board::checkMove(Move *move, Direction direction) {
    auto _move = _check(move->row, move->col, getRowMove(direction), getColMove(direction), table);
    auto _row = _move->row;
    auto _col = _move->col;
    delete _move;
    return move->row == _row && move->col == _col ? nullptr : new Move(_row, _col, move->kind);
}

std::vector<Move *> *Board::moves(Move *startPoint) {
    auto *result = new std::vector<Move *>();

    auto directions = {
            Direction::NORTH, Direction::SOUTH,
            Direction::EAST, Direction::WEST,
            Direction::NORTHEAST, Direction::NORTHWEST,
            Direction::SOUTHEAST, Direction::SOUTHWEST};

    for (auto &d : directions) {
        auto m = checkMove(startPoint, d);
        if (m) {
            result->push_back(m);
        }
    }

    return result;
}

void Board::applyMove(Move *from, Move *to) {
    (*table)[to->row][to->col] = to->kind;
    if (from->col * 5 + from->row != to->col * 5 + to->row)
        (*table)[from->row][from->col] = PieceKind::CELL;
}

void Board::applyFullMove(FullMove *fullMove, bool apply) {
    applyMove(fullMove->moves->at(apply ? 0 : 3), fullMove->moves->at(apply ? 1 : 2));
    applyMove(fullMove->moves->at(apply ? 2 : 1), fullMove->moves->at(apply ? 3 : 0));
}

std::vector<FullMove *> *Board::allMoves(PieceKind pieceKind) {
    Move *neutron = this->findNeutron();
    Move *lastNeutron = neutron->clone();
    auto playerHome = pieceKind == PieceKind::BLACK ? 0 : 4;
    auto opponentHome = pieceKind == PieceKind::BLACK ? 4 : 0;

    auto *neutronMoves = new std::vector<Move *>();
    auto *_neutronMoves = this->moves(neutron);
    for (auto &_neutronMove : *_neutronMoves) {
        if (_neutronMove->row != opponentHome) {
            neutronMoves->push_back(_neutronMove->clone());
        }
    }

    clean(_neutronMoves);

    std::sort(neutronMoves->begin(), neutronMoves->end(), [opponentHome, playerHome](auto o1, auto o2) {
        return (o2->row == playerHome || o1->row == opponentHome);
//        if (o1->row == playerHome || o2->row == opponentHome) return -1;
//        if (o2->row == playerHome || o1->row == opponentHome) return 1;
//        return 0;
    });

    auto *pieces = this->findPieces(pieceKind);

    auto allFullMoves = new std::vector<FullMove *>();

    for (auto &neutronMove : *neutronMoves) {
        this->applyMove(lastNeutron, neutron);
        this->applyMove(neutron, neutronMove);
        clean(lastNeutron);
        lastNeutron = neutronMove->clone();

        for (auto &piece: *pieces) {
            auto moves = this->moves(piece);
            for (auto &pieceMove: *moves) {
                allFullMoves->push_back(
                        new FullMove({neutron->clone(), neutronMove->clone(), piece->clone(), pieceMove->clone()}, 0));
            }

            clean(moves);
        }
    }

    this->applyMove(lastNeutron, neutron);

    clean(pieces);
    clean(neutron);
    clean(lastNeutron);
    clean(neutronMoves);

    return allFullMoves;
}

std::ostream &operator<<(std::ostream &ostr, const Board &board) {
    std::string str;

    for (auto &row : *board.table) {
        ostr << "||";
        for (auto &e : row) {
            ostr << board.pieceToString(e) << "|";
        }
        ostr << "|" << std::endl;
    }

    return ostr;
}

std::string Board::pieceToString(PieceKind pieceKind) const {
    if (pieceKind == PieceKind::BLACK) return "B";
    if (pieceKind == PieceKind::WHITE) return "W";
    if (pieceKind == PieceKind::NEUTRON) return "N";
    return " ";
}