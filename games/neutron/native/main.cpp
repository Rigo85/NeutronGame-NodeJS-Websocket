#include <iostream>
#include <chrono>

#include <minimax.h>
#include <gameutils.h>

int main() {
    auto table = (std::string) "{\"board\":[[1,1,1,1,1],[4,3,4,4,4],[4,2,4,4,4],[4,4,4,4,4],[2,4,2,2,2]]}";
    auto *board = new Board(getTable(table));

    auto t1 = std::chrono::system_clock::now();
    auto *fm = maxValue(board, 5, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), PieceKind::BLACK);
    auto t2 = std::chrono::system_clock::now();

    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
    std::cout << *board << std::endl;
    std::cout << *fm << std::endl;
    std::cout << fm->toJson() << std::endl;

    delete fm;
    delete board;

//    std::cout << static_cast<int>(PieceKind::NEUTRON) << std::endl;

    return 0;
}
