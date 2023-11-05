from __future__ import annotations

from pokerengine.pokerengine_core.enums.round import RoundEnum

class Round:
    flop_dealt: bool
    round: RoundEnum

    def __init__(self, round: RoundEnum = RoundEnum.PREFLOP, flop_dealt: bool = False) -> None: ...
    def set_next_game(self) -> None: ...
    def set_next_round(self) -> None: ...
