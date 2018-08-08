#include <iostream>
#include <chrono>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <regex>
#include <iterator>

#include <minimax.h>
#include <gameutils.h>

FullMove *processInput(std::string &input);

int column2Int(char c);

int row2Int(char r);

int main()
{
    //auto table = (std::string) "{\"board\":[[1,1,1,1,1],[4,4,4,4,4],[4,4,3,4,4],[4,4,4,4,4],[2,2,2,2,2]]}";
    auto table = (std::string) "{\"board\":[[2,4,4,1,2],[3,4,2,4,4],[2,1,4,4,4],[4,4,4,2,4],[4,4,1,1,1]]}";
    auto *board = new Board(getTable(table));

    std::string fullMovesStr = 
                            //     "c3 b4 b1 b3\n"
                            // //    "b4 d2 c5 c2\n"
                            //    "d2 e2 d1 d4\n"
                            // //    "e2 e4 b5 e2\n"
                            //    "e4 e3 d4 c5\n"
                            // //    "e3 e4 c2 d3\n"
                            //    "e4 e3 b3 b5\n"
                            // //    "e3 e4 d3 b1\n"
                            //    "e4 a4 a1 d4\n"
                            // //    "a4 c4 e2 d1\n"
                            //    "c4 a2 c1 c4\n"
                            // //    "a2 b3 a5 a1\n"
                            //    "b3 e3 e1 a5\n"
                            // //    "e3 a3 b1 c1\n"
                            //    "a3 e3 c5 a3\n"
                            // //    "e3 d2 e5 e1\n"
                            //    "d2 a2 d4 d2\n"
                            // //    "a2 b3 a1 b1\n"
                            //    "b3 a2 b5 b2\n"
                            // //    "a2 b3 b1 a2\n"
                            //    "b3 c2 b2 e5\n"
                            // //    "c2 a4 a2 b3\n"
                               "a4 b4 c4 b5\n";
                            //    "b4 a4 d5 d3";

    std::regex ws("\\s+");
    std::regex eol("\\n");
    for (
        auto it = std::sregex_token_iterator(fullMovesStr.begin(), fullMovesStr.end(), eol, -1);
        it != std::sregex_token_iterator();
        ++it)
    {
        std::string line = *it;

        printf("---------------------------------\n");
        std::cout << *board << std::endl;
        printf("---------------------------------\n");

        auto *fm = processInput(line);
        board->applyFullMove(fm);

        printf("---------------------------------\n");
        std::cout << line << std::endl;
        std::cout << *fm << std::endl;
        std::cout << *board << std::endl;
        printf("---------------------------------\n");

        auto t1 = std::chrono::system_clock::now();
        auto *fm_max = maxValue(board, 5, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), PieceKind::BLACK);
        auto t2 = std::chrono::system_clock::now();
        board->applyFullMove(fm_max);

        printf("---------------------------------\n");
        std::cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << "s" << std::endl;
        // std::cout << *fm << std::endl;
        std::cout << *fm_max << std::endl;
        std::cout << *board << std::endl;
        printf("---------------------------------++++++++++++++++++++++++++\n");

        delete fm;
        delete fm_max;
    }

    delete board;

    return EXIT_SUCCESS;
}

FullMove *processInput(std::string &input)
{
    std::regex ws("\\s+");

    auto it = std::sregex_token_iterator(input.begin(), input.end(), ws, -1);
    std::string token = *it;
    auto *m1 = new Move(row2Int(token[1]), column2Int(token[0]), PieceKind::NEUTRON);
    ++it;

    token = *it;
    auto *m2 = new Move(row2Int(token[1]), column2Int(token[0]), PieceKind::NEUTRON);
    ++it;

    token = *it;
    auto *m3 = new Move(row2Int(token[1]), column2Int(token[0]), PieceKind::WHITE);
    ++it;

    token = *it;
    auto *m4 = new Move(row2Int(token[1]), column2Int(token[0]), PieceKind::WHITE);
    ++it;

    return new FullMove({m1, m2, m3, m4}, 0);
}

int column2Int(char c)
{
    if (c == 'a')
        return 0;
    if (c == 'b')
        return 1;
    if (c == 'c')
        return 2;
    if (c == 'd')
        return 3;
    if (c == 'e')
        return 4;

    return 0;
}

int row2Int(char r)
{
    if (r == '5')
        return 0;
    if (r == '4')
        return 1;
    if (r == '3')
        return 2;
    if (r == '2')
        return 3;
    if (r == '1')
        return 4;

    return 0;
}
