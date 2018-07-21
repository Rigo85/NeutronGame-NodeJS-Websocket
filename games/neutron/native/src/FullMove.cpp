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


#include <FullMove.h>

FullMove::FullMove(std::initializer_list<Move *> moves, int score) {
    this->moves = new std::vector<Move *>();
    for (auto &move : moves) {
        this->moves->push_back(move);
    }
    this->score = score;
}

FullMove::~FullMove() {
    while (!moves->empty()) {
        delete *moves->begin();
        moves->erase(moves->begin());
    }

    delete moves;
}

std::string FullMove::kind2Name(PieceKind &pieceKind) const {
    switch (pieceKind) {
        case PieceKind::BLACK:
            return "BLACK";
        case PieceKind::WHITE:
            return "WHITE";
        case PieceKind::NEUTRON:
            return "NEUTRON";
        default:
            return "NO KIND";
    }
}

bool FullMove::empty() const {
    return moves == nullptr || moves->empty();
}

std::ostream &operator<<(std::ostream &ostr, const FullMove &fullMove) {
    if (!fullMove.empty()) {
        auto piece1Kind = fullMove.kind2Name(fullMove.moves->at(1)->kind);
        auto piece2Kind = fullMove.kind2Name(fullMove.moves->at(3)->kind);
        ostr << piece1Kind << ": " << *fullMove.moves->at(0) << "-" << *fullMove.moves->at(1) << " ";
        ostr << piece2Kind << ": " << *fullMove.moves->at(2) << "-" << *fullMove.moves->at(3);

        return ostr;
    }

    ostr << "EMPTY FULLMOVE with score = " << fullMove.score;
    return ostr;
}

void FullMove::cloneMoves(std::vector<Move *> *moves) {
    while (!this->moves->empty()) {
        delete *this->moves->begin();
        this->moves->erase(this->moves->begin());
    }

    for (auto &move : *moves) {
        this->moves->push_back(move->clone());
    }
}

FullMove *FullMove::clone() {
    auto fm = new FullMove({}, this->score);
    fm->cloneMoves(this->moves);

    return fm;
}