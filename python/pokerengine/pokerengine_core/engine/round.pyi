from __future__ import annotations

from pokerengine.pokerengine_core.enums.round import RoundE

class Round:
    flop_dealt: bool
    round: RoundE

    def __init__(self, round: RoundE = RoundE.PREFLOP, flop_dealt: bool = False) -> None: ...
    def reset(self) -> None: ...
