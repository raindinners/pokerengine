from __future__ import annotations

from typing import List

from pokerengine.pokerengine_core.engine.player import Players
from pokerengine.pokerengine_core.engine.positions import Positions
from pokerengine.pokerengine_core.engine.pot import PotRake01
from pokerengine.pokerengine_core.engine.round import Round
from pokerengine.pokerengine_core.enums.action import ActionEnum
from pokerengine.pokerengine_core.enums.position import PositionEnum

class EngineTraits:
    sb_bet: int
    bb_bet: int
    bb_mult: int
    min_raise: int

    def __init__(self, sb_bet: int, bb_bet: int, bb_mult: int) -> None: ...

class EngineRake01:
    traits: EngineTraits

    def __init__(self, traits: EngineTraits) -> None: ...
    @property
    def actions(self) -> List[PlayerAction]: ...
    @property
    def players_manager(self) -> Players: ...
    @property
    def positions_manager(self) -> Positions: ...
    @property
    def pot_manager(self) -> PotRake01: ...
    @property
    def round_manager(self) -> Round: ...
    def in_terminal_state(self) -> bool: ...
    def is_showdown(self) -> bool: ...
    def start(self, new_game: bool = False) -> None: ...
    def stop(self) -> None: ...
    def execute(self, action: PlayerAction) -> None: ...

class PlayerAction:
    amount: int
    action: ActionEnum
    position: PositionEnum

    def __init__(
        self,
        amount: int,
        action: ActionEnum,
        position: PositionEnum,
    ) -> None: ...
    def __eq__(self, other: PlayerAction) -> bool: ...
