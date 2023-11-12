from __future__ import annotations

from pokerengine.pokerengine_core.enums.action import ActionE
from pokerengine.pokerengine_core.enums.position import PositionE

class PlayerAction:
    """
    Represents player action for execute.
    """

    amount: int
    action: ActionE
    position: PositionE

    def __init__(
        self,
        amount: int,
        action: ActionE,
        position: PositionE,
    ) -> None: ...
    def __eq__(self, other: PlayerAction) -> bool: ...
    def __str__(self) -> str: ...
