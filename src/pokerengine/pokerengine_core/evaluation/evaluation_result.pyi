from __future__ import annotations

from typing import List, Tuple

from pokerengine.pokerengine_core.card.cards import Cards
from pokerengine.pokerengine_core.evaluation.result import Result

def get_evaluation_result(cards: Cards, players: List[int]) -> List[Tuple[Result, int]]:
    """
    Use this function to get hand results.

    :param cards: Game cards, can be generated via :class:`pokerengine.card.CardGenerator`
    :param players: Eligible player indexes
    :return: List of winner result and player index
    """

def get_evaluation_result_one(cards: Cards, for_: int) -> Result:
    """
    Use this function to get hand result.

    :param cards: Game cards, can be generated via :class:`pokerengine.card.CardGenerator`
    :param for_: Player indexes in :method:`pokerengine.card.CardGenerator.get_hands`
    :return: Hand result for player
    """
