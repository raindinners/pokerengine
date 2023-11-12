from __future__ import annotations

from pokerengine.pokerengine_core.enums.round import RoundE

class Round:
    """
    Game round manager.
    """

    flop_dealt: bool
    round: RoundE

    def __init__(self, round: RoundE = RoundE.PREFLOP, flop_dealt: bool = False) -> None: ...
    @property
    def showdown(self) -> bool:
        """
        Use this method to get is game is showdown.

        :return: Is game is showdown
        """
    def reset(self) -> None:
        """
        Use this method to reset round.

        :return: :class:`None`
        """
