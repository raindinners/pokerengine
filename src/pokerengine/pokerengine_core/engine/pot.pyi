from __future__ import annotations

from typing import List, Tuple

from pokerengine.pokerengine_core.card.cards import Cards
from pokerengine.pokerengine_core.engine.player import Players
from pokerengine.pokerengine_core.evaluation.result import Result

class PotRake01:
    """
    Game pot manager.
    """

    def __init__(self, players: Players) -> None: ...
    @property
    def highest_bet(self) -> int:
        """
        Use this method to get the highest bet in the whole game.

        :return: Highest bet
        """
    @property
    def round_highest_bet(self) -> int:
        """
        Use this method to get the highest bet in the current round.

        :return: Highest round bet
        """
    def pay(self, cards: Cards, flop_dealt: bool) -> List[Tuple[Result, int]]:
        """
        Use this method to pay pot for players (with cards).

        :param cards: Game cards, can be generated via :class:`pokerengine.card.CardGenerator`
        :param flop_dealt: Is flop dealt
        :return: List of hand straight and pot
        """
    def pay_noshowdown(self, flop_dealt: bool) -> List[int]:
        """
        Use this method to pay pot for players (only one player left, other players are outed).

        :param flop_dealt: Is flop dealt
        :return: List of pot
        """
    def pot(self, flop_dealt: bool) -> int:
        """
        Use this method to get game pot.

        :param flop_dealt: Is flop dealt
        :return: Pot
        """
    def pot_rake(self, flop_dealt: bool) -> int:
        """
        Use this method to get game pot (rake adjusted).

        :param flop_dealt: Is flop dealt
        :return: Pot (rake adjusted)
        """
