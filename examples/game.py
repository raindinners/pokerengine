from __future__ import annotations

import random
import time
from argparse import ArgumentParser, Namespace

from pokerengine.card import CardGenerator, Cards
from pokerengine.engine import EngineRake01, EngineTraits
from pokerengine.enums import ActionEnum, PositionEnum, RoundEnum, StateEnum


class GameEndedError(Exception):
    ...


def get_board_cards_to_print(round: RoundEnum) -> int:  # noqa
    if round == RoundEnum.PREFLOP:
        return 0
    if round == RoundEnum.FLOP:
        return 3
    if round == RoundEnum.TURN:
        return 4
    if round == RoundEnum.RIVER:
        return 5

    return 5  # Showdown


def game_state(engine: EngineRake01) -> str:
    to_return = (
        f"Status: {engine.round_manager.round.name}, "
        f"Pot: {engine.pot_manager.pot(engine.round_manager.flop_dealt)}, "
        f"Minraise {engine.traits.min_raise}\n"
    )

    for index in range(len(engine.players_manager.players)):
        player = engine.players_manager.players[index]
        to_return += f"Player[{index}], game bet {player.front}, round bet {player.round_bet}, stack {player.behind}, state {player.state.name}\n"

    return to_return


def core(engine: EngineRake01, cards: Cards) -> None:
    print(
        "The active player is:",
        engine.positions_manager.current.name,
        "(bot)" if engine.positions_manager.current != PositionEnum.SB else "(this is you)",
    )

    if engine.positions_manager.current == PositionEnum.SB:
        print("Your cards are:", cards.hands[PositionEnum.SB.value].as_string())

        board_cards_to_print = get_board_cards_to_print(engine.round_manager.round)
        if not board_cards_to_print:
            print("There is no board yet")
        else:
            print("The board is:", " ".join(card.as_string() for card in cards.board_n(5)))

        print("Select any of the actions")
        for index in range(0, len(engine.actions)):
            action = engine.actions[index]
            print("[", index, "]", "action", action.action.name, ", amount", action.amount)

        while True:
            try:
                user_action = int(input("Enter action index: "))
            except ValueError:
                print("Invalid action")
                continue

            try:
                action = engine.actions[user_action]
            except IndexError:
                print("Invalid action")
                continue

            if action.action == ActionEnum.RAISE:
                try:
                    amount = int(
                        input(f"Enter amount from {engine.traits.min_raise} to {action.amount}: ")
                    )
                except ValueError:
                    print("Invalid amount")
                    continue

                action.amount = amount

            engine.execute(action)
            break

    else:
        action = random.choice(engine.actions)
        if action.action != ActionEnum.NO_ACTION:
            print("Opponent action", action.action.name, "amount", action.amount)

            time.sleep(4)
            engine.execute(action)

    if engine.in_terminal_state():
        print("The hand has been ended")
        print(game_state(engine=engine))

        chips = (
            engine.pot_manager.pay(cards=cards, flop_dealt=engine.round_manager.flop_dealt)
            if engine.is_showdown()
            else engine.pot_manager.pay_noshowdown(flop_dealt=engine.round_manager.flop_dealt)
        )

        for index in range(len(chips)):
            if engine.is_showdown():
                print(
                    f"Combination {chips[index][0].as_string_long()},",
                    f"Player[{index}],",
                    "Got chips",
                    chips[index][1],
                )
            else:
                print(f"Player[{index}],", "Got chips", chips[index])

        raise GameEndedError


def __main__(args: Namespace) -> None:
    random.seed(args.seed)

    engine = EngineRake01(traits=EngineTraits(sb_bet=500, bb_bet=1000, bb_mult=20))
    card_generator = CardGenerator(seed=random.randint(0, 10000))

    for _ in range(args.players):
        engine.players_manager.add_player(20000, state=StateEnum.INIT)

    engine.start(False)

    while True:
        cards = Cards(cards=card_generator.generate_v(5 + args.players * 2))

        while True:
            print(game_state(engine))

            try:
                core(engine=engine, cards=cards)
            except GameEndedError:
                time.sleep(10)

                try:
                    engine.start(True)
                except RuntimeError:
                    print("====================================================================")
                    print("Game was ended")
                    exit(0)
                break


def main() -> None:
    parser = ArgumentParser(prog="Poker", description="The poker game")
    parser.add_argument(
        "-p", "--players", help="How many players in the game", default=7, type=int
    )
    parser.add_argument(
        "-s",
        "--seed",
        help="Seed for card generator",
        default=int(time.time()),
        type=int,
    )

    args = parser.parse_args()
    __main__(args)


if __name__ == "__main__":
    main()
