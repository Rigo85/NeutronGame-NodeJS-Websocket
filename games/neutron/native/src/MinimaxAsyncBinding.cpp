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

#include <MinimaxAsyncBinding.h>
#include <MinimaxAsyncWorker.h>

NAN_MODULE_INIT(MinimaxAsyncBinding::Init) {
    Nan::SetMethod(target, "NativeMinimax", NativeMinimax);
}

NAN_METHOD(MinimaxAsyncBinding::NativeMinimax) {
    Nan::AsyncQueueWorker(new MinimaxAsyncWorker (
            std::string(*Nan::Utf8String(info[0])),
            Nan::To<bool>(info[1]).FromJust(),
            new Nan::Callback(info[2].As<v8::Function>())
    ));
}
