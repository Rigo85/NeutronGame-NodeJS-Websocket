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

#include <vector>

#include <Move.h>
#include <PieceKind.h>

class FullMove {
public:
    FullMove(std::initializer_list<Move *> moves, int score);

    ~FullMove();

    std::string kind2Name(PieceKind &pieceKind) const;

    bool empty() const;

    friend std::ostream &operator<<(std::ostream &ostr, const FullMove &fullMove);

    void cloneMoves(std::vector<Move *> *moves);

    FullMove *clone();

    std::vector<Move *> *moves = nullptr;
    int score;
};
