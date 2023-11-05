from __future__ import annotations

from typing import List, Tuple

from pokerengine.pokerengine_core.card.cards import Cards
from pokerengine.pokerengine_core.evaluation.result import Result

def get_evaluation_result(cards: Cards, players: List[int]) -> List[Tuple[Result, int]]: ...
def get_evaluation_result_one(cards: Cards, for_: int) -> Result: ...
