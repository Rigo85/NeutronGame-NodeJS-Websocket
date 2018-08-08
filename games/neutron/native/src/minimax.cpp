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

#include <iostream>
#include <limits>

#include <minimax.h>
#include <gameutils.h>
#include <PieceKind.h>

FullMove *maxValue(Board *board, int depth, int alpha, int beta, PieceKind player)
{
    auto *neutron = board->findNeutron();

    if (!depth || neutron->row == 0 || neutron->row == 4)
    {
        clean(neutron);
        return new FullMove({}, heuristic(board));
    }

    auto fullMoves = board->allMoves(player);

    auto maxFullMove = new FullMove({}, alpha);

    for (auto &fullMove : *fullMoves)
    {
        board->applyFullMove(fullMove);

        auto minFullMove = minValue(board, depth - 1, maxFullMove->score, beta,
                                    player == PieceKind::BLACK ? PieceKind::WHITE : PieceKind::BLACK);

        if (minFullMove->score > maxFullMove->score)
        {
            maxFullMove->score = minFullMove->score;
            maxFullMove->cloneMoves(fullMove->moves);
        }

        board->applyFullMove(fullMove, false);

        if (maxFullMove->score >= beta)
        {
            fullMove->score = beta;

            auto *fullMoveClone = fullMove->clone();

            clean(maxFullMove);
            clean(fullMoves);
            clean(neutron);
            clean(minFullMove);

            return fullMoveClone;
        }
        clean(minFullMove);
    }

    if (maxFullMove->empty() && !fullMoves->empty())
    {
        auto tmp = new FullMove({}, std::numeric_limits<int>::min());
        for (auto &fullMove : *fullMoves)
        {
            board->applyFullMove(fullMove);
            auto h = heuristic(board);
            board->applyFullMove(fullMove, false);
            fullMove->score = h;

            if (fullMove->score > tmp->score)
            {
                delete tmp;
                tmp = fullMove->clone();
            }
        }

        clean(maxFullMove);
        clean(fullMoves);
        clean(neutron);

        return tmp;
    }
    else
    {
        clean(fullMoves);
        clean(neutron);

        return maxFullMove;
    }
}

FullMove *minValue(Board *board, int depth, int alpha, int beta, PieceKind player)
{
    auto neutron = board->findNeutron();

    if (!depth || neutron->row == 0 || neutron->row == 4)
    {
        clean(neutron);
        return new FullMove({}, heuristic(board));
    }

    auto fullMoves = board->allMoves(player);

    auto minFullMove = new FullMove({}, beta);

    for (auto &fullMove : *fullMoves)
    {
        board->applyFullMove(fullMove);

        auto maxFullMove = maxValue(board, depth - 1, alpha, minFullMove->score,
                                    player == PieceKind::BLACK ? PieceKind::WHITE : PieceKind::BLACK);

        if (maxFullMove->score < minFullMove->score)
        {
            minFullMove->score = maxFullMove->score;
            minFullMove->cloneMoves(fullMove->moves);
        }

        board->applyFullMove(fullMove, false);

        if (alpha >= minFullMove->score)
        {
            fullMove->score = alpha;

            auto *fullMoveClone = fullMove->clone();

            clean(minFullMove);
            clean(fullMoves);
            clean(neutron);
            clean(maxFullMove);

            return fullMoveClone;
        }
        clean(maxFullMove);
    }

    if (minFullMove->empty() && !fullMoves->empty())
    {
        auto tmp = new FullMove({}, std::numeric_limits<int>::min());
        for (auto &fullMove : *fullMoves)
        {
            board->applyFullMove(fullMove);
            auto h = heuristic(board);
            board->applyFullMove(fullMove, false);
            fullMove->score = h;

            if (fullMove->score < tmp->score)
            {
                delete tmp;
                tmp = fullMove->clone();
            }
        }

        clean(minFullMove);
        clean(fullMoves);
        clean(neutron);

        return tmp;
    }
    else
    {
        clean(fullMoves);
        clean(neutron);

        return minFullMove;
    }
}
