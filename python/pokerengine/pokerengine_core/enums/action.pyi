from __future__ import annotations

from enum import Enum

class ActionEnum(Enum):
    FOLD = 0
    CHECK = 1
    CALL = 2
    BET = 3
    RAISE = 4
    ALLIN = 5
    NO_ACTION = 6
