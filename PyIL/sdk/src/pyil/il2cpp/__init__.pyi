def select_assembly(name: str) -> None:
    """
    Selects the assembly to be used by PyIL.

    Args:
        name (str): The name of the assembly to select (e.g "Assembly-CSharp").
    """

def get_assembly() -> str | None:
    """
    Returns the selected target assembly.
    """

def get_assemblies() -> list[str]:
    """
    Returns a list of all loaded assemblies.
    """