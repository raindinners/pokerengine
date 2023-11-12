from __future__ import annotations

from typing import List

from pokerengine.pokerengine_core.enums.state import StateE

class Player:
    """
    Represents player in the game.
    """

    is_left: bool
    stack: int
    """Player stacksize."""
    behind: int
    """How mane chips left in the game."""
    front: int
    """Player bet for the game."""
    round_bet: int
    state: StateE

    def __init__(
        self,
        is_left: bool,
        stack: int,
        behind: int,
        front: int,
        round_bet: int,
        state: StateE,
    ) -> None: ...
    def __eq__(self, other: Player) -> bool: ...

class Players:
    """
    Game players manager.
    """

    def __init__(self) -> None: ...
    @property
    def players(self) -> List[Player]:
        """
        Use this method to get players.

        :return: Players
        """
    def get_player(self, index: int) -> Player:
        """
        Use this method to get player by their index.

        :param index: Player index
        :return: Player
        """
    def set_players(self, players: List[Player]) -> None:
        """
        Use this method to reset players

        :param players: New players
        :return: :class:`None`
        """
    def add_player(self, stack: int, state: StateE = StateE.INIT) -> None:
        """
        Use this method to add player in the game.

        :param stack: Player stack in the game
        :param state: Player state (default - init state)
        :return: :class:`None`
        """
    def remove_player(self, index: int) -> None:
        """
        Use this method to add player in the game.

        :param index: Player index
        :return: :class:`None`
        """
