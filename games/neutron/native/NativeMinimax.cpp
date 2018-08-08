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
#include <chrono>

#include <nan.h>

#include <minimax.h>
#include <gameutils.h>

NAN_METHOD(NativeMinimax){
    // TODO add parameter validation.
    std::string input(*Nan::Utf8String(info[0])); 
    auto *board = new Board(getTable(input));
    auto t1 = std::chrono::system_clock::now();
    auto *fm = maxValue(board, 5, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), PieceKind::BLACK);
    auto t2 = std::chrono::system_clock::now();
    auto fullMoveJson = Nan::New(fm->toJson()).ToLocalChecked();

    std::cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << "s" << std::endl;

    delete fm;
    delete board;

    info.GetReturnValue().Set(fullMoveJson);
}

NAN_MODULE_INIT(Initialize){
    NAN_EXPORT(target, NativeMinimax);
}

NODE_MODULE(NativeMinimax, Initialize);

