#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include "gobang.h"

EMSCRIPTEN_BINDINGS(gobang_binding)
{
    emscripten::class_<GoBang>("GoBang")
        .constructor<>()
        .function("placePiece", &GoBang::placePiece)
        .function("checkWinner", &GoBang::checkWinner)
        .function("getBoardLine", &GoBang::getBoardLine)
        .function("predictNext", &GoBang::predictNext);

    emscripten::register_vector<int32_t>("I32Vec");
}