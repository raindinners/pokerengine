from __future__ import annotations

from typing import List, overload

import pybind11_stubgen.typing_ext
import typing_extensions
from pokerengine.pokerengine_core.card.card import Card

class Hand:
    """
    Represents texas holdem poker hand.
    """

    @overload
    def __init__(
        self,
        front: Card,
        back: Card,
    ) -> None: ...
    @overload
    def __init__(self, front: int, back: int) -> None: ...
    @overload
    def __init__(self, value: str) -> None: ...
    def __eq__(self, other: Hand) -> bool: ...
    def __gt__(self, other: Hand) -> bool: ...
    def __lt__(self, other: Hand) -> bool: ...
    def __ne__(self, other: Hand) -> bool: ...
    def __str(self) -> str: ...
    def __int__(self) -> int:
        """
        Use this method to get hand as bitset.

        :return: Hand as bitset.
        """
    @property
    def value(
        self,
    ) -> typing_extensions.Annotated[List[Card], pybind11_stubgen.typing_ext.FixedSize(2)]:
        """
        Use this method to get hand.

        :return: Texas holdem poker hand
        """
