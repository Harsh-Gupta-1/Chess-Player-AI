#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "chess_ai.h"
#include <string>

namespace UCI {
    void loop();
    std::string moveToString(const Move& m);
    Move parseMove(Board& board, Color color, const std::string& moveStr);
}

#endif // UCI_H
