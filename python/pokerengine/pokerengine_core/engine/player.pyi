from __future__ import annotations

from typing import List

from pokerengine.pokerengine_core.enums.state import StateEnum

class Player:
    is_left: bool
    stack: int
    behind: int
    front: int
    round_bet: int
    state: StateEnum

    def __init__(
        self,
        is_left: bool,
        stack: int,
        behind: int,
        front: int,
        round_bet: int,
        state: StateEnum,
    ) -> None: ...
    def __eq__(self, other: Player) -> bool: ...

class Players:
    def __init__(self) -> None: ...
    @property
    def players(self) -> List[Player]: ...
    def get_player(self, index: int) -> Player: ...
    def set_players(self, players: List[Player]) -> None: ...
    def add_player(self, stack: int, state: StateEnum = StateEnum.INIT) -> None: ...
    def remove_player(self, index: int) -> None: ...
