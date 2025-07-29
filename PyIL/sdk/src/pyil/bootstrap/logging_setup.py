__all__ = ("setup_logger",)

import builtins

from rich import console, print as rich_print
from rich.traceback import install as install_traceback

console = console.Console()

def setup_logger() -> None:
    builtins.print = rich_print
    install_traceback(show_locals=True)
