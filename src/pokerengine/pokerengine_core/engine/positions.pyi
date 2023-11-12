from __future__ import annotations

from pokerengine.pokerengine_core.engine.player import Player, Players
from pokerengine.pokerengine_core.enums.position import PositionE

class Positions:
    """
    Game positions manager.
    """

    current: PositionE = PositionE.NONE

    def __init__(self, players: Players) -> None: ...
    @property
    def number_alive(self) -> int:
        """
        Use this method to get number alive players.

        :return: Number alive
        """
    @property
    def future_actionable(self) -> int:
        """
        Use this method to get number future actionable players.

        :return: Number future actionable
        """
    @property
    def player(self) -> Player:
        """
        Use this method to get current player.

        :return: Current player
        """
    @property
    def terminal_state(self) -> bool:
        """
        Use this method to get game state.

        :return: Is number alive is greater than one
        """
    def actionable(self, highest_round_bet: int) -> int:
        """
        Use this method to get number actionable players.

        :param highest_round_bet: Highest round bet
        :return: Number actionable
        """
    def set_next_current(self) -> None:
        """
        Use this method to set next player in the round.

        :return: :class:`None`
        """
    def set_next_round_player(self) -> None:
        """
        Use this method to set next player to small blind.

        :return: :class:`None`
        """
