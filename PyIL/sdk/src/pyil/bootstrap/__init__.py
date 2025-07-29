from .logging_setup import *

def _set_cwd(path: str) -> None:
    import os
    os.chdir(path)