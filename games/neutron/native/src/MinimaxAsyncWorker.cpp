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

#include <chrono>

#include <minimax.h>
#include <gameutils.h>
#include <MinimaxAsyncWorker.h>

MinimaxAsyncWorker::MinimaxAsyncWorker(std::string input, bool throwsError, Nan::Callback *callback) : Nan::AsyncWorker(
        callback) {
    this->throwsError = throwsError;
    this->input = input;
}

void MinimaxAsyncWorker::Execute() {
    if (throwsError) {
        this->SetErrorMessage("An error occured!");
        return;
    }

    auto *board = new Board(getTable(this->input));
    auto t1 = std::chrono::system_clock::now();
    auto *fm = maxValue(board, 5, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), PieceKind::BLACK);
    auto t2 = std::chrono::system_clock::now();
    this->output = fm->toJson();

    std::cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << "s" << std::endl;

    delete fm;
    delete board;
}

void MinimaxAsyncWorker::HandleOKCallback() {
    Nan::HandleScope scope;
    v8::Local <v8::Value> argv[] = {
            Nan::Null(), // no error occured
            Nan::New(this->output).ToLocalChecked()
    };
    Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 2, argv);
}

void MinimaxAsyncWorker::HandleErrorCallback() {
    Nan::HandleScope scope;
    v8::Local <v8::Value> argv[] = {
            Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
            Nan::Null()
    };
    Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 2, argv);
}

