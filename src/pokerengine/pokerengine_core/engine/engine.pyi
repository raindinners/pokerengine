from __future__ import annotations

from typing import List

from pokerengine.pokerengine_core.engine.action import PlayerAction
from pokerengine.pokerengine_core.engine.player import Players
from pokerengine.pokerengine_core.engine.positions import Positions
from pokerengine.pokerengine_core.engine.pot import PotRake01
from pokerengine.pokerengine_core.engine.round import Round

class EngineTraits:
    """
    Game create settings.
    """

    sb_bet: int
    """Small blind bet."""
    bb_bet: int
    """Big blind bet."""
    bb_mult: int
    """Big blind multiplication, used in the formula: stack_to_join=bb_bet*bb_mult"""
    min_raise: int
    """Game minimal raise size."""

    def __init__(self, sb_bet: int, bb_bet: int, bb_mult: int) -> None: ...

class EngineRake01:
    """
    Game class.

    Settings:
        - rake size: from 0 to 1
    """

    traits: EngineTraits

    def __init__(self, traits: EngineTraits) -> None: ...
    @property
    def actions(self) -> List[PlayerAction]:
        """
        Use this method to get possible actions for current player.

        :return: List of possible actions
        """
    @property
    def players_manager(self) -> Players:
        """
        Use this method to get players manager.

        :return: Players manager
        """
    @property
    def positions_manager(self) -> Positions:
        """
        Use this method to get positions manager.

        :return: Positions manager
        """
    @property
    def pot_manager(self) -> PotRake01:
        """
        Use this method to get pot manager.

        :return: Pot manager
        """
    @property
    def round_manager(self) -> Round:
        """
        Use this method to get round manager.

        :return: Round manager
        """
    def start(self, new_game: bool = False) -> None:
        """
        Use this method to start/restart game.

        :param new_game: Is need to reset round
        :return: :class:`None`
        """
    def stop(self) -> None:
        """
        Use this method to stop game.

        :return: :class:`None`
        """
    def execute(self, action: PlayerAction) -> None:
        """
        Use this method to execute player action.

        :param action: Action to execute
        :return: :class:`None`
        """
