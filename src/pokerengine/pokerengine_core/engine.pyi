from __future__ import annotations

from typing import List, Tuple

from pokerengine.pokerengine_core.card.cards import Cards
from pokerengine.pokerengine_core.enums.action import ActionE
from pokerengine.pokerengine_core.enums.position import PositionE
from pokerengine.pokerengine_core.enums.round import RoundE
from pokerengine.pokerengine_core.enums.state import StateE
from pokerengine.pokerengine_core.evaluation.result import Result

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

class _EngineDetail: ...

class _Engine:
    def __init__(self, traits: EngineTraits) -> None: ...
    @property
    def actions(self) -> _Actions:
        """
        Use this method to get actions manager.

        :return: Actions manager
        """
    @property
    def players(self) -> _Players:
        """
        Use this method to get players manager.

        :return: Players manager
        """
    @property
    def positions(self) -> _Positions:
        """
        Use this method to get positions manager.

        :return: Positions manager
        """
    @property
    def pot(self) -> _Pot:
        """
        Use this method to get pot manager.

        :return: Pot manager
        """
    @property
    def round(self) -> _Round:
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

class _Actions(_EngineDetail):
    def __init__(self, engine: _Engine) -> None: ...
    @property
    def actions(self) -> List[PlayerAction]:
        """
        Use this method to get possible actions for current player.

        :return: List of possible actions
        """
    def execute(self, action: PlayerAction) -> None:
        """
        Use this method to execute player action.

        :param action: Action to execute
        :return: :class:`None`
        """

class _Players(_EngineDetail):
    """
    Game players manager.
    """

    def __init__(self, engine: _Engine) -> None: ...
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
    def add_player(self, stack: int) -> None:
        """
        Use this method to add player in the game.

        :param stack: Player stack in the game
        :return: :class:`None`
        """
    def remove_player(self, index: int) -> None:
        """
        Use this method to add player in the game.

        :param index: Player index
        :return: :class:`None`
        """

class _Positions(_EngineDetail):
    """
    Game positions manager.
    """

    current: PositionE = PositionE.NONE

    def __init__(self, engine: _Engine) -> None: ...
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
    def actionable(self) -> int:
        """
        Use this method to get number actionable players.

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

class _Pot(_EngineDetail):
    """
    Game pot manager.
    """

    def __init__(self, engine: _Engine) -> None: ...
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
    def pay(self, cards: Cards) -> List[Tuple[Result, int]]:
        """
        Use this method to pay pot for players (with cards).

        :param cards: Game cards, can be generated via :class:`pokerengine.card.CardGenerator`
        :return: List of hand straight and pot
        """
    def pay_noshowdown(self) -> List[int]:
        """
        Use this method to pay pot for players (only one player left, other players are outed).

        :return: List of pot
        """
    def pot(self) -> int:
        """
        Use this method to get game pot.

        :return: Pot
        """
    def pot_rake(self) -> int:
        """
        Use this method to get game pot (rake adjusted).

        :return: Pot (rake adjusted)
        """

class _Round(_EngineDetail):
    """
    Game round manager.
    """

    flop_dealt: bool = False
    round: RoundE = RoundE.NONE

    def __init__(self, engine: _Engine) -> None: ...
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

class EngineRake01(_Engine): ...
class ActionsRake01(_Actions): ...
class PlayersRake01(_Players): ...
class PositionsRake01(_Positions): ...
class PotRake01(_Pot): ...
class RoundRake01(_Round): ...
