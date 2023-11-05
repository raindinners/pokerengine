from __future__ import annotations

from enum import Enum

class RoundEnum(Enum):
    PREFLOP = 0
    FLOP = 1
    TURN = 2
    RIVER = 3
    SHOWDOWN = 4
