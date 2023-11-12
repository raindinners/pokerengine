from __future__ import annotations

from typing import List, overload

from pokerengine.pokerengine_core.card.card import Card
from pokerengine.pokerengine_core.card.hand import Hand

class Cards:
    """
    Represents game cards.
    """

    @overload
    def __init__(self, board: List[str], hands: List[str]) -> None: ...
    @overload
    def __init__(self, cards: List[Card]) -> None: ...
    def __eq__(self, other: Cards) -> bool: ...
    def __gt__(self, other: Cards) -> bool: ...
    def __lt__(self, other: Cards) -> bool: ...
    def __ne__(self, other: Cards) -> bool: ...
    def __str(self) -> str: ...
    @property
    def board(self) -> List[Card]:
        """
        Use this method to get game board.

        :return: Game board
        """
    @property
    def hands(self) -> List[Hand]:
        """
        Use this method to get players hands.

        :return: Players hands
        """
    def board_n(self, n: int) -> List[Card]:
        """
        Use this method to get n game board.

        :return: N game board
        """
